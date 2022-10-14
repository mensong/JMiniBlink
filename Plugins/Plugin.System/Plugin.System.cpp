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
		"readText\0"
		"writeText\0"
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