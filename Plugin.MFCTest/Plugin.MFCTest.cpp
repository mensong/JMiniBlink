// Plugin.MFCTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Plugin.MFCTest.h"
#include <Plugin.h>
#include "DlgMyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CPluginMFCTestApp

BEGIN_MESSAGE_MAP(CPluginMFCTestApp, CWinApp)
END_MESSAGE_MAP()


// CPluginMFCTestApp 构造

CPluginMFCTestApp::CPluginMFCTestApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPluginMFCTestApp 对象

CPluginMFCTestApp theApp;


// CPluginMFCTestApp 初始化

BOOL CPluginMFCTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


PLUGIN_API ERR_CODE plugin_entry(class Application* app)
{
	return 0;
}
PLUGIN_API const char* plugin_name()
{
	return "MFCTest";
}
PLUGIN_API int plugin_functions(const char** functionsName)
{
	*functionsName = "foo1\0foo2\0";
	return 2;
}

PLUGIN_API jsValue foo1(jsExecState* es, Application* app)
{
	const utf8* plugName = PlugName(es);
	const wchar_t* plugNameW = PlugNameW(es);
	const utf8* funcName = FunctionName(es);
	const wchar_t* funcNameW = FunctionNameW(es);
	int n = Argc(es);
	n = jsToInt(*es, Argv(es, 0));

	CDlgMyDialog dlg;
	int res = dlg.DoModal();

	return jsStringW(*es, dlg.m_sKeyword.GetString());
}

PLUGIN_API jsValue foo2(jsExecState* es, Application* app)
{
	MessageBoxA(0, "foo2", "", 0);
	return jsString(*es, "456");
}