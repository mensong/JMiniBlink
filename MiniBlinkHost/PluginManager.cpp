#include "stdafx.h"
#include "PluginManager.h"
#include "MiniBlinkHostApp.h"

//插件方法原型 
typedef jsValue (*PFN_PluginFunction)(jsExecState* es, Application* app);
//插件入口点
typedef int(*PFN_plugin_entry)(class Application* app);
//插件名称
typedef const char* (*PFN_plugin_name)();
//插件方法列表
typedef const char* (*PFN_plugin_functions)();

//<插件名, <方法名, 方法句柄>>
std::map<std::string, std::map<std::string, PFN_PluginFunction>> g_pluginFunctions;

jsValue WKE_CALL_TYPE js_CallExtend(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc < 2)
		return jsThrowException(es, "Parameters error:CallExtend(PluginName, FunctionName, parameters1, parameters2, ...)");
	
	jsValue jv0 = jsArg(es, 0);
	if (!jsIsString(jv0))
		return jsThrowException(es, "Parameters error:CallExtend(PluginName, FunctionName, parameters1, parameters2, ...)");

	jsValue jv1 = jsArg(es, 1);
	if (!jsIsString(jv1))
		return jsThrowException(es, "Parameters error:CallExtend(PluginName, FunctionName, parameters1, parameters2, ...)");

	std::string plugName = jsToString(es, jv0);
	std::string funcName = jsToString(es, jv1);
	if (funcName.empty())
		return jsThrowException(es, "Parameters error:CallExtend(PluginName, FunctionName, parameters1, parameters2, ...)");

	auto itFinder1 = g_pluginFunctions.find(plugName);
	if (itFinder1 == g_pluginFunctions.end())
		return jsThrowException(es, "Can not found plugin name");

	auto itFinder2 = itFinder1->second.find(funcName);
	if (itFinder2 == itFinder1->second.end())
		return jsThrowException(es, "Can not found function name");

	auto func = itFinder2->second;
	if (func == NULL)
		return jsThrowException(es, "Can not found function name");

	return func(&es, app);	
}


jsValue WKE_CALL_TYPE js_LoadPlugins(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);

	std::vector<std::string> plugins;
	for (int i = 0; i < nArgc; ++i)
	{
		jsValue jv = jsArg(es, i);
		if (!jsIsString(jv))
			continue;
		const utf8* path = jsToString(es, jv);
		std::vector<wchar_t> wv;
		MByteToWChar(path, strlen(path), &wv, CP_UTF8);
		wv.push_back('\0');
		plugins.push_back(UnicodeToAnsi(wv.data()));
	}

	return jsBoolean(AddPlugins(app, plugins));
}

void InitPlugin(class Application* app)
{
	/*
	* CallExtend(pluginName:string, functionName:string, 参数1, 参数2, ...) : js result;
	*/
	wkeJsBindFunction("CallExtend", js_CallExtend, app, 0);

	/*
	* LoadPlugins(pluginPath1:string, pluginPath2:string, ...) : boolean;
	*/
	wkeJsBindFunction("LoadPlugins", js_LoadPlugins, app, 0);
}

bool AddPlugins(class Application* app, const std::vector<std::string>& pluginsFilePath)
{
	for (int i = 0; i < pluginsFilePath.size(); ++i)
	{
		HMODULE h =  LoadLibraryA(pluginsFilePath[i].c_str());
		if (h == NULL)
			return false;

		//入口
		PFN_plugin_entry plugin_entry = (PFN_plugin_entry)GetProcAddress(h, "plugin_entry");
		if (plugin_entry == NULL)
			return false;
		if (plugin_entry(app) != 0)
			return false;

		//插件名
		PFN_plugin_name plugin_name = (PFN_plugin_name)GetProcAddress(h, "plugin_name");
		if (plugin_name == NULL)
			return false;
		const char* pluginName = plugin_name();
		if (pluginName == NULL)
			return false;

		//函数列表
		PFN_plugin_functions plugin_functions = (PFN_plugin_functions)GetProcAddress(h, "plugin_functions");
		if (plugin_functions == NULL)
			return false;
		const char* functionsNames = plugin_functions();
		if (functionsNames == NULL)
			return false;
		const char* p = functionsNames;
		while(1)
		{
			if (p == NULL)
				return false;
			std::string fn(p);
			if (fn.empty())
				return false;
			PFN_PluginFunction plugin_function = (PFN_PluginFunction)GetProcAddress(h, fn.c_str());
			if (plugin_function == NULL)
				return false;

			if (g_pluginFunctions.find(pluginName) == g_pluginFunctions.end())
				g_pluginFunctions.insert(std::make_pair(pluginName, std::map<std::string, PFN_PluginFunction>()));

			if (g_pluginFunctions[pluginName].find(fn) == g_pluginFunctions[pluginName].end())
				g_pluginFunctions[pluginName].insert(std::make_pair(fn, plugin_function));
			else
				g_pluginFunctions[pluginName][fn] = plugin_function;

			p += (int)(strlen(p) + 1);
		}

	}

	return true;
}