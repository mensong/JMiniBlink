#include "stdafx.h"
#include "MiniBlinkHostApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ArgumentMan.h>

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "BindEvents.h"
#include "BindFunctions.h"
#include "PluginManager.h"


void Application::PrintHelp()
{
	const wchar_t* msg =
		L"命令行参数：\r\n"
		L"  无或-url <url> : 主页url\r\n"
		L"  -hide <0|1> : 是否隐藏窗口，默认不隐藏窗口\r\n"
		L"  -tran <0|1> : 是否为透明窗口，默认不透明\r\n"
		L"  -width <num> : 窗口初始宽度\r\n"
		L"  -height <num> : 窗口初始高度\n"
		L"  -preload <preload.js> : 预加载js文件（默认为同程序目录下的preload.js）\n";
	MessageBoxW(NULL, msg, L"help", MB_OK);
}

BOOL Application::ProcessCommandLine()
{
	Application* app = this;

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	AM::CArgumentMan<std::wstring, wchar_t> am;
	am.SetIgnoreCase(true);
	am.SetMultiValue(true);
	am.AddCmdFlag(L"-");
	am.AddCmdFlag(L"/");
	am.AddCmdFlag(L"--");
	am.Parse(argc, argv);
		
	//Start Url
	if (!am.Has(app->url, L"url"))
		app->url = am.GetArg(L"");

	std::wstring sTemp;
		
	//是否隐藏窗口，默认不隐藏窗口
	am.Has(sTemp, L"hide");
	std::transform(sTemp.begin(), sTemp.end(), sTemp.begin(), ::towupper);
	if (sTemp == L"1" || sTemp == L"TRUE" || sTemp == L"YES")
		app->hide = true;

	//是否透明，默认不透明，只有在窗口不隐藏时有效
	am.Has(sTemp, L"tran");
	std::transform(sTemp.begin(), sTemp.end(), sTemp.begin(), ::towupper);
	if (sTemp == L"1" || sTemp == L"TRUE" || sTemp == L"YES")
		app->transparent = true;

	//宽高
	if (am.Has(sTemp, L"width"))
	{
		int w = _wtoi(sTemp.c_str());
		if (w > 0)
			app->width = w;
	}
	if (am.Has(sTemp, L"height"))
	{
		int h = _wtoi(sTemp.c_str());
		if (h > 0)
			app->height = h;
	}

	//preload js file path
	if (!am.Has(app->preloadFile, L"preload"))
		app->preloadFile = L"preload.js";

	//sim name
	if (am.Has(sTemp, L"simName"))
	{
		if (!sTemp.empty())
		{
			std::vector<char> vctSZ;
			WCharToMByte(sTemp.c_str(), sTemp.size(), &vctSZ, CP_ACP);
			vctSZ.push_back('\0');
			app->simName = vctSZ.data();
		}
	}
	if (app->simName.empty())
		app->simName =  "sim" + std::to_string(::GetTickCount());
	app->db = new simdb(app->simName.c_str(), 1024, 4096);

	//插件
	auto pluginPaths = am.GetArgs(L"plugin");
	for (int i = 0; i < pluginPaths.size(); ++i)
	{
		plugins.push_back(UnicodeToAnsi(pluginPaths[i]));
	}

    return TRUE;
}

LRESULT CALLBACK _staticWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_CLOSE:
		::ShowWindow(hwnd, SW_HIDE);
		::DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
	{
		UINT_PTR wTimerID = wParam;
		if (wTimerID == 1)
		{
			KillTimer(hwnd, wTimerID);
			::ShowWindow(hwnd, SW_HIDE);
			::DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		break;
	}
	}

	return ::DefWindowProcW(hwnd, message, wParam, lParam);
}

// 创建主页面窗口
BOOL Application::CreateWebWindow()
{
	Application* app = this;

	InitFunctions(app);

	if (!InitPlugins(app, plugins))
	{
		MessageBoxW(NULL, L"插件加载失败", L"error", MB_ICONERROR);
	}

	if (!app->hide)
	{//创建窗口模式
		if (app->transparent)
			app->window = wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, NULL, 0, 0, app->width, app->height);
		else
			app->window = wkeCreateWebWindow(WKE_WINDOW_TYPE_POPUP, NULL, 0, 0, app->width, app->height);

		if (!app->window)
			return FALSE;
		
		InitEvents(app);

		wkeSetWindowTitleW(app->window, L"WebView");
		wkeMoveToCenter(app->window);
		wkeShowWindow(app->window, true);

		//wkeLoadUrlW(app->window, app->url.c_str());

		if (app->transparent)
		{
			//可调整宽度,高度
			const bool ENABLE_BORDER = true;
			wkeCreateResizeBorders(app->window, ENABLE_BORDER, ENABLE_BORDER,ENABLE_BORDER, 
				ENABLE_BORDER, ENABLE_BORDER, ENABLE_BORDER, ENABLE_BORDER, ENABLE_BORDER);
		}
	}
	else
	{//创建隐藏无头模式
		const wchar_t* szClassName = L"BgWebView";
		MSG msg = { 0 };
		WNDCLASSW wndClass = { 0 };
		if (!GetClassInfoW(NULL, szClassName, &wndClass))
		{
			wndClass.style = CS_HREDRAW | CS_VREDRAW;
			wndClass.lpfnWndProc = &_staticWindowProc;
			wndClass.cbClsExtra = 200;
			wndClass.cbWndExtra = 200;
			wndClass.hInstance = GetModuleHandleW(NULL);
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hbrBackground = NULL;
			wndClass.lpszMenuName = NULL;
			wndClass.lpszClassName = szClassName;
			RegisterClassW(&wndClass);
		}

		app->window = wkeCreateWebView();
		HWND hWnd = ::CreateWindowEx(
			WS_EX_APPWINDOW, 
			szClassName, 
			NULL, 
			WS_OVERLAPPEDWINDOW,
			0, 0, 
			app->width, app->height, 
			NULL, NULL, 
			::GetModuleHandle(NULL), NULL
		);
		::SetProp(hWnd, L"wke", (HANDLE)app->window);
		wkeSetHandle(app->window, hWnd);

		wkeSetHeadlessEnabled(app->window, true);

		InitEvents(app);

		wkeMoveToCenter(app->window);
		wkeShowWindow(app->window, false);

		//wkeLoadUrlW(app->window, app->url.c_str());
	}

    return TRUE;
}

void Application::PrintHelpAndQuit()
{
    PrintHelp();
    PostQuitMessage(0);
}

void Application::RunMessageLoop()
{
    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void Application::RunApplication()
{
	Application* app = this;
	//MessageBoxA(NULL, "", "", 0);
	
    if (!ProcessCommandLine())
    {
        PrintHelpAndQuit();
        return;
    }

    if (!CreateWebWindow())
    {
        PrintHelpAndQuit();
        return;
    }

    RunMessageLoop();
}

void Application::QuitApplication()
{
	Application* app = this;
    if (app->window)
    {
        wkeDestroyWebWindow(app->window);
        app->window = NULL;
    }
}
