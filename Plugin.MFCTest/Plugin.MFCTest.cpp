// Plugin.MFCTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Plugin.MFCTest.h"
#include <Plugin.h>
#include "DlgMyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CPluginMFCTestApp

BEGIN_MESSAGE_MAP(CPluginMFCTestApp, CWinApp)
END_MESSAGE_MAP()


// CPluginMFCTestApp ����

CPluginMFCTestApp::CPluginMFCTestApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPluginMFCTestApp ����

CPluginMFCTestApp theApp;


// CPluginMFCTestApp ��ʼ��

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