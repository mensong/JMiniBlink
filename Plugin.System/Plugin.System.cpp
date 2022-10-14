// Plugin.System.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Plugin.h"

PLUGIN_API ERR_CODE plugin_entry(class Application* app)
{
	return 0;
}

PLUGIN_API const char* plugin_name()
{
	return "System";
}

PLUGIN_API int plugin_functions(const char** functionsName)
{
	*functionsName = 
		"foo1\0"
		"foo2\0"
		;
	return 2;
}

PLUGIN_API jsValue readText(jsExecState* es, Application* app)
{
	if (Argc(es) == 0)
		return jsUndefined();

	auto jv = Argv(es, 0);
	if (!jsIsString(jv))
		return jsUndefined();

	const utf8* p = jsToString(*es, jv);

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