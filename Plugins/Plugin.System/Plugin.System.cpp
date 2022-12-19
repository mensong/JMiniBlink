// Plugin.System.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Plugin.h"
#include <shellapi.h>
#include "MiniBlinkHost\MiniBlinkHostApp.h"

PLUGIN_API ERR_CODE plugin_entry(class Application* app)
{
	return 0;
}

PLUGIN_API const char* plugin_name()
{
	return "System";
}

PLUGIN_API const char* plugin_functions()
{
	return
		"readText\0"
		"writeText\0"
		"killMe\0"
		"setIcon\0"
		"\0"
		;
}

PLUGIN_API jsValue readText(jsExecState* es, Application* app)
{
	if (Argc(es) == 0)
		return jsUndefined();

	auto jv = Argv(es, 0);
	if (!jsIsString(jv))
		return jsUndefined();

	const utf8* path = jsToString(*es, jv);
	auto path1 = GL::Utf82Ansi(path);
	auto text = FileHelper::ReadText(path1.c_str());
	return jsString(*es, text.c_str());
}

PLUGIN_API jsValue writeText(jsExecState* es, Application* app)
{
	int nArg = Argc(es);
	if (nArg < 2)
		return jsThrowException(*es, "Parameters error.");

	auto jv = Argv(es, 0);
	if (!jsIsString(jv))
		return jsThrowException(*es, "Parameters error.");

	auto jv1 = Argv(es, 1);
	//if (!jsIsString(jv1))
	//	return jsThrowException(*es, "Parameters error.");

	bool bAppend = false;
	if (nArg == 3)
	{
		auto jv2 = Argv(es, 2);
		if (!jsIsBoolean(jv2))
			return jsThrowException(*es, "Parameters error.");
		bAppend = jsToBoolean(*es, jv2);
	}

	std::string filePath = jsToString(*es, jv);
	std::string text = jsToString(*es, jv1);
	
	auto path = GL::Utf82Ansi(filePath.c_str());

	int start = bAppend ? -1 : 0;
	
	auto wlen = FileHelper::WriteFile(path.c_str(), text.c_str(), text.size(), NULL, start, false, true);
	return jsInt(wlen);
}

PLUGIN_API jsValue killMe(jsExecState* es, Application* app)
{
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);
	return jsUndefined();
}

PLUGIN_API jsValue setIcon(jsExecState* es, Application* app)
{
	int nArg = Argc(es);
	if (nArg < 1)
		return jsThrowException(*es, "Parameters error.");

	jsValue jv = Argv(es, 0);
	std::string iconPath = jsToString(*es, jv);

	HICON hicon = (HICON)LoadImageA(
		NULL, //handle of the instance that contains //the image
		iconPath.c_str(),//name or identifier of image
		IMAGE_ICON,//type of image-can also be IMAGE_CURSOR or MAGE_ICON
		0, 0,//desired width and height
		LR_LOADFROMFILE);//load flags
	if (hicon == NULL)
		return jsBoolean(false);
	
	if (nArg == 1)
	{
		SendMessage(wkeGetWindowHandle(app->window), WM_SETICON, TRUE, (LPARAM)hicon);
		SendMessage(wkeGetWindowHandle(app->window), WM_SETICON, FALSE, (LPARAM)hicon);
	}
	else
	{
		jv = Argv(es, 1);
		bool bIsLarge = jsToBoolean(*es, jv);
		SendMessage(wkeGetWindowHandle(app->window), WM_SETICON, (bIsLarge?TRUE:FALSE), (LPARAM)hicon);
	}
	return jsBoolean(true);
}

PLUGIN_API jsValue foo1(jsExecState* es, Application* app)
{
	MessageBoxA(0, "foo1", "", 0);
	const utf8* plugName = PlugName(es);
	const wchar_t* plugNameW = PlugNameW(es);
	const utf8* funcName = FunctionName(es);
	const wchar_t* funcNameW = FunctionNameW(es);
	int n = Argc(es);
	n = jsToInt(*es, Argv(es, 0));
	return jsString(*es, "123");
}

PLUGIN_API jsValue foo2(jsExecState* es, Application* app)
{
	MessageBoxA(0, "foo2", "", 0);
	return jsString(*es, "456");
}