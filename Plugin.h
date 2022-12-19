#pragma once
#include "wke.h"

#define PLUGIN_API extern "C" _declspec(dllexport)
#define ERR_CODE int

//����ʵ�ֵĺ���
// ������
PLUGIN_API ERR_CODE plugin_entry(class Application* app);
// �������
PLUGIN_API const char* plugin_name();
// �������
PLUGIN_API const char* plugin_functions();

//���ʵ�ʲ�������
//CallExtend("�����", "������", ����1, ����2, ����3, ...);
//����� �� ������ ռ����������λ�ã�����ʵ�ʲ�����Ҫ����
inline int Argc(jsExecState* es)
{
	int n = jsArgCount(*es);
	if (n < 2)
		return 0;
	return n - 2;
}
//���ʵ�ʲ���
inline jsValue Argv(jsExecState* es, int argIdx)
{
	int n = jsArgCount(*es);
	if (n < 2 + argIdx + 1)
		return jsUndefined();

	return jsArg(*es, argIdx + 2);
}

//����es��ú����е��õĲ������(js�����еĵ�1��)
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

//����es��ú����е��õĺ�������(js�����еĵ�2��)
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