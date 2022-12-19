#pragma once
#include "wke.h"

#define PLUGIN_API extern "C" _declspec(dllexport)
#define ERR_CODE int

//必须实现的函数
// 插件入口
PLUGIN_API ERR_CODE plugin_entry(class Application* app);
// 插件名称
PLUGIN_API const char* plugin_name();
// 插件函数
PLUGIN_API const char* plugin_functions();

//获得实际参数数量
//CallExtend("插件名", "函数名", 参数1, 参数2, 参数3, ...);
//插件名 与 函数名 占了两个参数位置，所以实际参数需要减二
inline int Argc(jsExecState* es)
{
	int n = jsArgCount(*es);
	if (n < 2)
		return 0;
	return n - 2;
}
//获得实际参数
inline jsValue Argv(jsExecState* es, int argIdx)
{
	int n = jsArgCount(*es);
	if (n < 2 + argIdx + 1)
		return jsUndefined();

	return jsArg(*es, argIdx + 2);
}

//根据es获得函数中调用的插件名称(js参数中的第1个)
inline const utf8* PlugName(jsExecState* es)
{
	if (jsArgCount(*es) < 1)
		return NULL;

	jsValue jv0 = jsArg(*es, 0);
	if (!jsIsString(jv0))
		return NULL;

	return jsToString(*es, jv0);
}
inline const  wchar_t* PlugNameW(jsExecState* es)
{
	if (jsArgCount(*es) < 1)
		return NULL;

	jsValue jv0 = jsArg(*es, 0);
	if (!jsIsString(jv0))
		return NULL;

	return jsToStringW(*es, jv0);
}

//根据es获得函数中调用的函数名称(js参数中的第2个)
inline const utf8* FunctionName(jsExecState* es)
{
	if (jsArgCount(*es) < 2)
		return NULL;

	jsValue jv1 = jsArg(*es, 1);
	if (!jsIsString(jv1))
		return NULL;

	return jsToString(*es, jv1);
}
inline const wchar_t* FunctionNameW(jsExecState* es)
{
	if (jsArgCount(*es) < 2)
		return NULL;

	jsValue jv1 = jsArg(*es, 1);
	if (!jsIsString(jv1))
		return NULL;

	return jsToStringW(*es, jv1);
}