#include "stdafx.h"
#include "BindFunctions.h"
#include "MiniBlinkHostApp.h"
#include <map>
#include <algorithm>

jsValue getJsValueByString(Application* app, jsExecState es, const std::string& jsValueString)
{
	size_t pos = jsValueString.find('|');
	if (pos == std::string::npos || pos == 0)
		return jsUndefined();

	jsType type = (jsType)atoi(jsValueString.substr(0, pos).c_str());
	std::string stringValue = jsValueString.substr(pos + 1);
	switch (type)
	{
	case JSTYPE_NUMBER:		
		if (stringValue.find('.') == std::string::npos)//int
			return jsInt(atoi(stringValue.c_str()));
		else
			return jsDouble(atof(stringValue.c_str()));
	case JSTYPE_STRING:
		return jsString(es, stringValue.c_str());
	case JSTYPE_BOOLEAN:
		return jsBoolean(stringValue == "true");
	case JSTYPE_OBJECT:
	case JSTYPE_ARRAY:
	case JSTYPE_FUNCTION:
		return wkeRunJS(app->window, ("return " + stringValue).c_str(), true);
	case JSTYPE_NULL:
		return jsNull();
	}

	return jsUndefined();
}

std::string makeJsValue(Application* app, jsExecState es, jsValue jv)
{
	/*
	  "jsType|string data"
	*/
	jsType t = jsTypeOf(jv);
	std::string sStringValue = std::to_string((int)t) + "|";
	switch (t)
	{
	case JSTYPE_NUMBER:
	case JSTYPE_STRING:
	case JSTYPE_BOOLEAN:
	case JSTYPE_UNDEFINED:
	case JSTYPE_NULL:
		sStringValue += jsToString(es, jv);
		break;
	case JSTYPE_ARRAY:
	case JSTYPE_OBJECT:
		jsSetGlobal(es, "__JSTYPE_TEMP", jv);
		sStringValue += jsToString(es,
			wkeRunJS(app->window, "JSON.stringify(__JSTYPE_TEMP)", false));
		break;
	case JSTYPE_FUNCTION:
		jsValue jFunc = jsGet(es, jv, "toString");
		jsValue jFuncStr = jsCall(es, jFunc, jv, NULL, 0);
		sStringValue += jsToString(es, jFuncStr);
		break;
	}

	return sStringValue;
}

jsValue WKE_CALL_TYPE js_GlobalData(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	//const utf8* da = jsToString(es, jsArg(es, 0));

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{//取
		const utf8* key = jsToString(es, jsArg(es, 0));
		std::string sJsValueString = app->db->get(key);
		return getJsValueByString(app, es, sJsValueString);
	}
	else if (nArgc == 2)
	{//写
		const utf8* key = jsToString(es, jsArg(es, 0));
		jsValue jv = jsArg(es, 1);
		
		if (jsIsUndefined(jv))
		{//删除
			std::string sJsValueString = app->db->get(key);
			jv = getJsValueByString(app, es, sJsValueString);
			app->db->del(key);
		}
		else
		{
			app->db->put(key, makeJsValue(app, es, jv));
		}
		return jv;
	}

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeGetVersion(jsExecState es, void* param)
{
	return jsInt(wkeGetVersion());
}

jsValue WKE_CALL_TYPE js_wkeGetVersionString(jsExecState es, void* param)
{
	return jsString(es, wkeGetVersionString());
}

jsValue WKE_CALL_TYPE js_wkeGC(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		
		int intervalSec = jsToInt(es, jv0);
		wkeGC(app->window, intervalSec);
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetResourceGc(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		
		int intervalSec = jsToInt(es, jv0);
		wkeSetResourceGc(app->window, intervalSec);
		return jsUndefined();		
	}
	return jsThrowException(es, "Parameters error.");
}

//jsValue WKE_CALL_TYPE js_wkeCreateWebView(jsExecState es, void* param)
//{
//	return jsInt((int)wkeCreateWebView());
//}

jsValue WKE_CALL_TYPE js_wkeDestroyWebView(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeDestroyWebView(app->window);

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeSetMemoryCacheEnable(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetMemoryCacheEnable(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetMouseEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetMouseEnabled(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetTouchEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetTouchEnabled(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetNavigationToNewWindowEnable(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetNavigationToNewWindowEnable(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetCspCheckEnable(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetCspCheckEnable(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetNpapiPluginsEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetNpapiPluginsEnabled(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
		
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetHeadlessEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetNpapiPluginsEnabled(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetDragEnable(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetDragEnable(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetDragDropEnable(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetDragDropEnable(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
		
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetLanguage(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetLanguage(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetViewNetInterface(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		
		wkeSetLanguage(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetProxy(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 5)
	{
		wkeProxy proxySetting;
		memset(&proxySetting, 0, sizeof(wkeProxy));

		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		proxySetting.type = (wkeProxyType)jsToInt(es, jv0);

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");
		const utf8* hostName = jsToString(es, jv1);
		strcpy_s(proxySetting.hostname, 100, hostName);

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsThrowException(es, "Parameters error.");
		proxySetting.port = jsToInt(es, jv2);

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsString(jv3))
			return jsThrowException(es, "Parameters error.");
		const utf8* userName = jsToString(es, jv3);
		strcpy_s(proxySetting.username, 50, userName);

		jsValue jv4 = jsArg(es, 4);
		if (!jsIsString(jv4))
			return jsThrowException(es, "Parameters error.");
		const utf8* password = jsToString(es, jv4);
		strcpy_s(proxySetting.password, 50, password);

		wkeSetProxy(&proxySetting);
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetViewProxy(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 5)
	{
		wkeProxy proxySetting;
		memset(&proxySetting, 0, sizeof(wkeProxy));

		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		proxySetting.type = (wkeProxyType)jsToInt(es, jv0);

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");
		const utf8* hostName = jsToString(es, jv1);
		strcpy_s(proxySetting.hostname, 100, hostName);

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsThrowException(es, "Parameters error.");
		proxySetting.port = jsToInt(es, jv2);

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsString(jv3))
			return jsThrowException(es, "Parameters error.");
		const utf8* userName = jsToString(es, jv3);
		strcpy_s(proxySetting.username, 50, userName);

		jsValue jv4 = jsArg(es, 4);
		if (!jsIsString(jv4))
			return jsThrowException(es, "Parameters error.");
		const utf8* password = jsToString(es, jv4);
		strcpy_s(proxySetting.password, 50, password);

		wkeSetViewProxy(app->window, &proxySetting);
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetName(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsString(es, wkeGetName(app->window));
}

jsValue WKE_CALL_TYPE js_wkeSetName(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetName(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetHandle(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetHandle(app->window, (HWND)jsToInt(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetHwndRenderOffset(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		wkeSetHwndRenderOffset(app->window, jsToInt(es, jv0), jsToInt(es, jv1));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetUserAgent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetUserAgent(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetUserAgent(jsExecState es, void* param)
{
	Application* app = (Application*)param;
		
	return jsString(es, wkeGetUserAgent(app->window));
}

jsValue WKE_CALL_TYPE js_wkeShowDevtools(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		const utf8* path = jsToString(es, jv0);
		std::string sPath;
		std::string sPathUpper = path;
		std::transform(sPathUpper.begin(), sPathUpper.end(), sPathUpper.begin(), ::toupper);
		if (sPathUpper.substr(0, strlen("FILE://")) != "FILE://" &&
			sPathUpper.substr(0, strlen("HTTP")) != "HTTP")
		{//处理相对路径的问题
			if (sPathUpper.size() >= 4 && sPathUpper[1] != ':')
			{//相对路径
				char dir[MAX_PATH] = { 0 };
				GetCurrentDirectoryA(MAX_PATH, dir);
				sPath = dir;
				sPath += "\\";
				sPath += path;
			}
			else
			{
				sPath = path;
			}
		}
		else
		{
			sPath = path;
		}

		wkeShowDevtools(app->window, sPath.c_str(), NULL, NULL);
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeLoadUrl(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeLoadUrl(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkePostUrl(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{//url,data
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		const utf8* url = jsToString(es, jv0);

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");
		const utf8* data = jsToString(es, jv1);

		wkePostUrl(app->window, url, data, strlen(data));
		return jsUndefined();
	}
	else if (nArgc == 3)
	{//url, data, Content-Type
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		const utf8* url = jsToString(es, jv0);

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");
		const utf8* data = jsToString(es, jv1);

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsString(jv2))
			return jsThrowException(es, "Parameters error.");
		const utf8* contentType = jsToString(es, jv2);

		wkePostUrl2(app->window, url, data, strlen(data), contentType);
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeLoadHtml(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{//html
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeLoadHtml(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	else if (nArgc == 2)
	{//html, baseUrl
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");

		wkeLoadHtmlWithBaseUrl(app->window, jsToString(es, jv0), jsToString(es, jv1));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeLoadFile(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeLoadFile(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetFrameUrl(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		return jsString(es, wkeGetFrameUrl(app->window, (wkeWebFrameHandle)jsToInt(es, jv0)));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetFrameHandleByUrl(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		return jsInt((int)wkeGetFrameHandleByUrl(app->window, jsToString(es, jv0)));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeStopLoad(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeStopLoad(app->window);

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeReload(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeReload(app->window);

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeGoToHistoryOffset(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeGoToHistoryOffset(app->window, jsToInt(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGoToHistoryIndex(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeGoToHistoryIndex(app->window, jsToInt(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeCanGoBackx(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsBoolean(wkeCanGoBack(app->window));
}

jsValue WKE_CALL_TYPE js_wkeGoBack(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsBoolean(wkeGoBack(app->window));
}

jsValue WKE_CALL_TYPE js_wkeCanGoForward(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsBoolean(wkeCanGoForward(app->window));
}

jsValue WKE_CALL_TYPE js_wkeGoForward(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsBoolean(wkeGoForward(app->window));
}

jsValue WKE_CALL_TYPE js_wkeEditorSelectAll(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorSelectAll(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorUnSelect(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorUnSelect(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorCopy(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorCopy(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorCut(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorCut(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorPaste(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorPaste(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorDelete(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorDelete(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorUndo(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorUndo(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeEditorRedo(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeEditorRedo(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeSetCookieEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetCookieEnabled(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeIsCookieEnabled(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsBoolean(wkeIsCookieEnabled(app->window));
}

jsValue WKE_CALL_TYPE js_wkeSetCookieJarDirectory(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetCookieJarDirectory(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetCookieJarFilePath(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetCookieJarFilePath(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetCookie(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	const utf8* data = wkeGetCookie(app->window);
	return jsString(es, data);
}

jsValue WKE_CALL_TYPE js_wkeSetCookie(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");

		wkeSetCookie(app->window, jsToString(es, jv0), jsToString(es, jv1));
		return jsUndefined();
	}
	
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetLocalStorageDirectory(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetLocalStorageDirectory(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeAddPluginDirectory(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeAddPluginDirectory(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetMediaVolume(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetMediaVolume(app->window, jsToFloat(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetMediaVolume(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	return jsFloat(wkeGetMediaVolume(app->window));
}

jsValue WKE_CALL_TYPE js_wkeFireMouseEvent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 4)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsThrowException(es, "Parameters error.");

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsNumber(jv3))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeFireMouseEvent(app->window, jsToInt(es, jv0), 
			jsToInt(es, jv1), jsToInt(es, jv2), jsToInt(es, jv3)));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeFireMouseWheelEvent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 4)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsThrowException(es, "Parameters error.");

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsNumber(jv3))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeFireMouseWheelEvent(app->window, jsToInt(es, jv0),
			jsToInt(es, jv1), jsToInt(es, jv2), jsToInt(es, jv3)));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeFireKeyUpEvent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeFireKeyUpEvent(app->window, jsToInt(es, jv0),
			jsToInt(es, jv1), false));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeFireKeyDownEvent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeFireKeyDownEvent(app->window, jsToInt(es, jv0),
			jsToInt(es, jv1), false));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeFireKeyPressEvent(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeFireKeyPressEvent(app->window, jsToInt(es, jv0),
			jsToInt(es, jv1), false));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeFireWindowsMessage(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 4)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsBoolean(jv2))
			return jsThrowException(es, "Parameters error.");

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsBoolean(jv3))
			return jsThrowException(es, "Parameters error.");

		LRESULT res = 0;
		return jsBoolean(wkeFireWindowsMessage(app->window, (HWND)jsToInt(es, jv0),
			(UINT)jsToInt(es, jv1), (WPARAM)jsToDouble(es, jv2), (LPARAM)jsToDouble(es, jv3), &res));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetFocus(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeSetFocus(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeKillFocus(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeKillFocus(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeRunJsInAllFrames(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeRunJsInAllFrames(app->window, jsToString(es, jv0), false);
		return jsUndefined();
	}
	else if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeRunJsInAllFrames(app->window, jsToString(es, jv0), jsToBoolean(es, jv1));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetZoomFactor(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");
		wkeSetZoomFactor(app->window, jsToFloat(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetZoomFactor(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	return jsFloat(wkeGetZoomFactor(app->window));
}

jsValue WKE_CALL_TYPE js_wkeSetDragFiles(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 3)
	{//clientPoint.x, clientPoint.y, filePath(string)|filePaths(Array)
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		POINT ptClient;
		ptClient.x = jsToInt(es, jv0);
		ptClient.y = jsToInt(es, jv1);

		POINT ptScreen = ptClient;

		HWND hWin = wkeGetWindowHandle(app->window);
		::ClientToScreen(hWin, &ptScreen);
		
		wkeString* files = NULL;
		int nFilesCount = 0;

		jsValue jv2 = jsArg(es, 2);
		if (jsIsString(jv2))
		{
			nFilesCount = 1;
			files = new wkeString[nFilesCount];
			const utf8* pFileName = jsToString(es, jv2);
			files[0] = wkeCreateString(pFileName, strlen(pFileName));
		}
		else if (jsIsArray(jv2))
		{
			int nFilesCount = jsGetLength(es, jv2);

			files = new wkeString[nFilesCount];

			for (int i = 0; i < nFilesCount; ++i)
			{
				jsValue j = jsGetAt(es, jv2, i);
				const utf8* pFileName = jsToString(es, j);
				files[i] = wkeCreateString(pFileName, strlen(pFileName));
			}			
		}
		else
		{
			return jsThrowException(es, "Parameters error.");
		}
				
		wkeSetDragFiles(app->window, &ptClient, &ptScreen, files, nFilesCount);

		for (int i = 0; i < nFilesCount; ++i)
		{
			wkeDeleteString(files[i]);
		}
		
		return jsUndefined();
	}

	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeIsMainFrame(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{//指定了frame handle
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeIsMainFrame(app->window, (wkeWebFrameHandle)jsToInt(es, jv0)));
	}

	//没有指定，则根据网址找
	const utf8* url = wkeGetUrl(app->window);
	wkeWebFrameHandle frameHandle = wkeGetFrameHandleByUrl(app->window, url);
	if (frameHandle == NULL)
		return jsBoolean(false);

	return jsBoolean(wkeIsMainFrame(app->window, frameHandle));
}

jsValue WKE_CALL_TYPE js_wkeIsWebRemoteFrame(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		return jsBoolean(wkeIsWebRemoteFrame(app->window, (wkeWebFrameHandle)jsToInt(es, jv0)));
	}

	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeWebFrameGetMainFrame(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	return jsInt((int)wkeWebFrameGetMainFrame(app->window));
}

jsValue WKE_CALL_TYPE js_wkeRunJsByFrame(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");

		return wkeRunJsByFrame(app->window, (wkeWebFrameHandle)jsToInt(es, jv0), jsToString(es, jv1), false);
	}
	else if (nArgc == 3)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsBoolean(jv2))
			return jsThrowException(es, "Parameters error.");

		return wkeRunJsByFrame(app->window, (wkeWebFrameHandle)jsToInt(es, jv0), jsToString(es, jv1), jsToBoolean(es, jv2));
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeInsertCSSByFrame(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsThrowException(es, "Parameters error.");

		wkeInsertCSSByFrame(app->window, (wkeWebFrameHandle)jsToInt(es, jv0), jsToString(es, jv1));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeCreateWebWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;
		
	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsObject(jv0))
			return jsThrowException(es, "Parameters error.");
				
		jsValue jUrl = jsGet(es, jv0, "url");
		if (!jsIsString(jUrl))
			return jsThrowException(es, "Parameters error.");

		char filePath[MAX_PATH] = { 0 };
		::GetModuleFileNameA(NULL, filePath, MAX_PATH);
		std::string sCmd = filePath;
		sCmd += " ";
		sCmd += jsToString(es, jUrl);

		jsValue j = jsGet(es, jv0, "hide");
		if (jsIsBoolean(j))
		{
			sCmd += " -hide ";
			sCmd += jsToBoolean(es, j) ? "1" : "0";
		}

		j = jsGet(es, jv0, "tran");
		if (jsIsBoolean(j))
		{
			sCmd += " -tran ";
			sCmd += jsToBoolean(es, j) ? "1" : "0";
		}

		j = jsGet(es, jv0, "width");
		if (jsIsNumber(j))
		{
			sCmd += " -width ";
			sCmd += std::to_string(jsToInt(es, j));
		}

		j = jsGet(es, jv0, "height");
		if (jsIsNumber(j))
		{
			sCmd += " -height ";
			sCmd += std::to_string(jsToInt(es, j));
		}

		j = jsGet(es, jv0, "preload");
		if (jsIsString(j))
		{
			sCmd += " -preload ";
			sCmd += jsToString(es, j);
		}
		sCmd += " -simName ";
		sCmd += app->SimName();

		//j = jsGet(es, jv0, "plugin");
		//if (jsIsArray(j))
		//{
		//	jsGetArrayBuffer
		//}

		WinExec(sCmd.c_str(), SW_SHOW);

		return jsUndefined();
	}

	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeDestroyWebWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	PostQuitMessage(0);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeGetWindowHandle(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	return jsInt((int)wkeGetWindowHandle(app->window));
}

jsValue WKE_CALL_TYPE js_wkeShowWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeShowWindow(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeEnableWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeEnableWindow(app->window, jsToBoolean(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeMoveWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 4)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsThrowException(es, "Parameters error.");

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsNumber(jv3))
			return jsThrowException(es, "Parameters error.");

		wkeMoveWindow(app->window, jsToInt(es, jv0), jsToInt(es, jv1), jsToInt(es, jv2), jsToInt(es, jv3));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeMoveToCenter(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	wkeMoveToCenter(app->window);
	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeResizeWindow(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 4)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsNumber(jv1))
			return jsThrowException(es, "Parameters error.");

		wkeResizeWindow(app->window, jsToInt(es, jv0), jsToInt(es, jv1));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeCreateResizeBorders(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 8)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsBoolean(jv0))
			return jsThrowException(es, "Parameters error.");

		jsValue jv1 = jsArg(es, 1);
		if (!jsIsBoolean(jv1))
			return jsThrowException(es, "Parameters error.");

		jsValue jv2 = jsArg(es, 2);
		if (!jsIsBoolean(jv2))
			return jsThrowException(es, "Parameters error.");

		jsValue jv3 = jsArg(es, 3);
		if (!jsIsBoolean(jv3))
			return jsThrowException(es, "Parameters error.");

		jsValue jv4 = jsArg(es, 4);
		if (!jsIsBoolean(jv4))
			return jsThrowException(es, "Parameters error.");

		jsValue jv5 = jsArg(es, 5);
		if (!jsIsBoolean(jv5))
			return jsThrowException(es, "Parameters error.");

		jsValue jv6 = jsArg(es, 6);
		if (!jsIsBoolean(jv6))
			return jsThrowException(es, "Parameters error.");

		jsValue jv7 = jsArg(es, 7);
		if (!jsIsBoolean(jv7))
			return jsThrowException(es, "Parameters error.");

		wkeCreateResizeBorders(app->window, 
			jsToBoolean(es, jv0), jsToBoolean(es, jv1),
			jsToBoolean(es, jv2), jsToBoolean(es, jv3), 
			jsToBoolean(es, jv4), jsToBoolean(es, jv5), 
			jsToBoolean(es, jv6), jsToBoolean(es, jv7));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeSetWindowTitle(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsString(jv0))
			return jsThrowException(es, "Parameters error.");

		wkeSetWindowTitle(app->window, jsToString(es, jv0));
		return jsUndefined();
	}
	return jsThrowException(es, "Parameters error.");
}

jsValue WKE_CALL_TYPE js_wkeGetTitle(jsExecState es, void* param)
{
	Application* app = (Application*)param;
	return jsString(es, wkeGetTitle(app->window));
}


void InitFunctions(Application* app)
{
	/*
	* GlobalData(key:string) : jsvalue;//取
	* GlobalData(key:string, value:jsvalue) : jsvalue;//存
	* GlobalData(key:string, undefined) : jsvalue;//删
	*/
	wkeJsBindFunction("GlobalData", js_GlobalData, app, -1);

	/*
	* wkeGetVersion() : int;
	*/
	wkeJsBindFunction("wkeGetVersion", js_wkeGetVersion, app, 0);
	/*
	* wkeGetVersionString() : string;
	*/
	wkeJsBindFunction("wkeGetVersionString", js_wkeGetVersionString, app, 0);
	/*
	* wkeGC(intervalSec:int) : undefined;
	*/
	wkeJsBindFunction("wkeGC", js_wkeGC, app, 1);
	/*
	* wkeSetResourceGc(intervalSec:int) : undefined;
	*/
	wkeJsBindFunction("wkeSetResourceGc", js_wkeSetResourceGc, app, 1);
	
	//wkeJsBindFunction("wkeCreateWebView", js_wkeCreateWebView, app, 0);
	/*
	* wkeDestroyWebView() : undefined;
	*/
	wkeJsBindFunction("wkeDestroyWebView", js_wkeDestroyWebView, app, 0);

	/*
	* wkeSetMemoryCacheEnable(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetMemoryCacheEnable", js_wkeSetMemoryCacheEnable, app, 1);
	/*
	* wkeSetMouseEnabled(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetMouseEnabled", js_wkeSetMouseEnabled, app, 1);
	/*
	* wkeSetTouchEnabled(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetTouchEnabled", js_wkeSetTouchEnabled, app, 1);
	/*
	* wkeSetNavigationToNewWindowEnable(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetNavigationToNewWindowEnable", js_wkeSetNavigationToNewWindowEnable, app, 1);
	/*
	* wkeSetCspCheckEnable(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetCspCheckEnable", js_wkeSetCspCheckEnable, app, 1);
	/*
	* wkeSetNpapiPluginsEnabled(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetNpapiPluginsEnabled", js_wkeSetNpapiPluginsEnabled, app, 1);
	/*
	* wkeSetHeadlessEnabled(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetHeadlessEnabled", js_wkeSetHeadlessEnabled, app, 1);
	/*
	* wkeSetDragEnable(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetDragEnable", js_wkeSetDragEnable, app, 1);
	/*
	* wkeSetDragDropEnable(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetDragDropEnable", js_wkeSetDragDropEnable, app, 1);

	/*
	* wkeSetLanguage(newLanguage:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetLanguage", js_wkeSetLanguage, app, 1);
	/*
	* wkeSetViewNetInterface(interface:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetViewNetInterface", js_wkeSetViewNetInterface, app, 1);
	/* 
	* wkeSetProxy(proxyType:int, hostName:string, port:int, username:string, password:string) : undefined;
	*   proxyType  - 参见wkeProxyType的定义
	*/
	wkeJsBindFunction("wkeSetProxy", js_wkeSetProxy, app, 5);
	/*
	* wkeSetViewProxy(proxyType:int, hostName:string, port:int, username:string, password:string) : undefined;
	*   proxyType  - 参见wkeProxyType的定义
	*/
	wkeJsBindFunction("wkeSetViewProxy", js_wkeSetViewProxy, app, 5);

	/*
	* wkeGetName() : string;
	*/
	wkeJsBindFunction("wkeGetName", js_wkeGetName, app, 0);
	/*
	* wkeSetName(name:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetName", js_wkeSetName, app, 1);
	/*
	* wkeSetHandle(handle:int) : undefined;
	*/
	wkeJsBindFunction("wkeSetHandle", js_wkeSetHandle, app, 1);
	/*
	* wkeSetHwndRenderOffset(x:int, y:int) : undefined;
	*/
	wkeJsBindFunction("wkeSetHwndRenderOffset", js_wkeSetHwndRenderOffset, app, 2);
	/*
	* wkeSetUserAgent(ua:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetUserAgent", js_wkeSetUserAgent, app, 1);
	/*
	* wkeGetUserAgent() : string;
	*/
	wkeJsBindFunction("wkeGetUserAgent", js_wkeGetUserAgent, app, 0);

	/*
	* wkeShowDevtools(inspectorFilePath:string) : undefined;
	*/
	wkeJsBindFunction("wkeShowDevtools", js_wkeShowDevtools, app, 1);

	/*
	* wkeLoadUrl(url:string) : undefined;
	*/
	wkeJsBindFunction("wkeLoadUrl", js_wkeLoadUrl, app, 1);
	/*
	* wkePostUrl(url:string, data:string, [contentType:string]) : undefined;
	*/
	wkeJsBindFunction("wkePostUrl", js_wkePostUrl, app, -1);
	/*
	* wkeLoadHtml(url:string, [baseUrl:string]) : undefined;
	*/
	wkeJsBindFunction("wkeLoadHtml", js_wkeLoadHtml, app, -1);
	/*
	* wkeLoadFile(filePath:string) : undefined;
	*/
	wkeJsBindFunction("wkeLoadFile", js_wkeLoadFile, app, 1);
	/*
	* wkeGetFrameUrl(frameId:int) : string;
	*/
	wkeJsBindFunction("wkeGetFrameUrl", js_wkeGetFrameUrl, app, 1);
	/*
	* wkeGetFrameHandleByUrl(url:string) : int;
	*/
	wkeJsBindFunction("wkeGetFrameHandleByUrl", js_wkeGetFrameHandleByUrl, app, 1);

	/*
	* wkeStopLoad() : undefined;
	*/
	wkeJsBindFunction("wkeStopLoad", js_wkeStopLoad, app, 0);
	/*
	* wkeReload() : undefined;
	*/
	wkeJsBindFunction("wkeReload", js_wkeReload, app, 0);

	/*
	* wkeGoToHistoryOffset(offset:int) : undefined;
	*/
	wkeJsBindFunction("wkeGoToHistoryOffset", js_wkeGoToHistoryOffset, app, 1);
	/*
	* wkeGoToHistoryIndex(index:int) : undefined;
	*/
	wkeJsBindFunction("wkeGoToHistoryIndex", js_wkeGoToHistoryIndex, app, 1);
	/*
	* wkeCanGoBack() : boolean;
	*/
	wkeJsBindFunction("wkeCanGoBack", js_wkeCanGoBackx, app, 0);
	/*
	* wkeGoBack() : boolean;
	*/
	wkeJsBindFunction("wkeGoBack", js_wkeGoBack, app, 0);
	/*
	* wkeCanGoForward() : boolean;
	*/
	wkeJsBindFunction("wkeCanGoForward", js_wkeCanGoForward, app, 0);
	/*
	* wkeGoForward() : boolean;
	*/
	wkeJsBindFunction("wkeGoForward", js_wkeGoForward, app, 0);
	
	/*
	* wkeEditorSelectAll() : undefined;
	*/
	wkeJsBindFunction("wkeEditorSelectAll", js_wkeEditorSelectAll, app, 0);
	/*
	* wkeEditorUnSelect() : undefined;
	*/
	wkeJsBindFunction("wkeEditorUnSelect", js_wkeEditorUnSelect, app, 0);
	/*
	* wkeEditorCopy() : undefined;
	*/
	wkeJsBindFunction("wkeEditorCopy", js_wkeEditorCopy, app, 0);
	/*
	* wkeEditorCut() : undefined;
	*/
	wkeJsBindFunction("wkeEditorCut", js_wkeEditorCut, app, 0);
	/*
	* wkeEditorPaste() : undefined;
	*/
	wkeJsBindFunction("wkeEditorPaste", js_wkeEditorPaste, app, 0);
	/*
	* wkeEditorDelete() : undefined;
	*/
	wkeJsBindFunction("wkeEditorDelete", js_wkeEditorDelete, app, 0);
	/*
	* wkeEditorUndo() : undefined;
	*/
	wkeJsBindFunction("wkeEditorUndo", js_wkeEditorUndo, app, 0);
	/*
	* wkeEditorRedo() : undefined;
	*/
	wkeJsBindFunction("wkeEditorRedo", js_wkeEditorRedo, app, 0);
	
	/*
	* wkeSetCookieEnabled(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeSetCookieEnabled", js_wkeSetCookieEnabled, app, 1);
	/*
	* wkeIsCookieEnabled() : boolean;
	*/
	wkeJsBindFunction("wkeIsCookieEnabled", js_wkeIsCookieEnabled, app, 0);
	/*
	* wkeSetCookieJarDirectory(dir:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetCookieJarDirectory", js_wkeSetCookieJarDirectory, app, 1);
	/*
	* wkeSetCookieJarFilePath(filePath:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetCookieJarFilePath", js_wkeSetCookieJarFilePath, app, 1);
	/*
	* wkeGetCookie() : string;
	*/
	wkeJsBindFunction("wkeGetCookie", js_wkeGetCookie, app, 0);
	/*
	* wkeSetCookie(url:string, data:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetCookie", js_wkeSetCookie, app, 2);
	/*
	* wkeSetLocalStorageDirectory(dir:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetLocalStorageDirectory", js_wkeSetLocalStorageDirectory, app, 1);
	/*
	* wkeAddPluginDirectory(dir:string) : undefined;
	*/
	wkeJsBindFunction("wkeAddPluginDirectory", js_wkeAddPluginDirectory, app, 1);
	
	/*
	* wkeSetMediaVolume(value:float) : undefined;
	*/
	wkeJsBindFunction("wkeSetMediaVolume", js_wkeSetMediaVolume, app, 1);
	/*
	* wkeGetMediaVolume() : float;
	*/
	wkeJsBindFunction("wkeGetMediaVolume", js_wkeGetMediaVolume, app, 0);
	
	/*
	* wkeFireMouseEvent(message:int, x:int, y:int, flags:int) : boolean;
	*   message - 可取WM_MOUSELEAVE等Windows相关鼠标消息
	*   flags - 参见wkeMouseFlags，可通过“或”操作并联
	*/
	wkeJsBindFunction("wkeFireMouseEvent", js_wkeFireMouseEvent, app, 4);
	/*
	* wkeFireMouseWheelEvent(x:int, y:int, delta:int, flags:int) : boolean;
	*   flags - 参见wkeMouseFlags，可通过“或”操作并联
	*/
	wkeJsBindFunction("wkeFireMouseWheelEvent", js_wkeFireMouseWheelEvent, app, 4);
	/*
	* wkeFireKeyUpEvent(virtualKeyCode:int, flags:int) : boolean;
	*   virtualKeyCode - https://docs.microsoft.com/zh-cn/windows/desktop/inputdev/virtual-key-codes
	*   flags - 参见wkeMouseFlags，可通过“或”操作并联
	*/
	wkeJsBindFunction("wkeFireKeyUpEvent", js_wkeFireKeyUpEvent, app, 2);
	/*
	* wkeFireKeyDownEvent(virtualKeyCode:int, flags:int) : boolean;
	*   virtualKeyCode - https://docs.microsoft.com/zh-cn/windows/desktop/inputdev/virtual-key-codes
	*   flags - 参见wkeMouseFlags，可通过“或”操作并联
	*/
	wkeJsBindFunction("wkeFireKeyDownEvent", js_wkeFireKeyDownEvent, app, 2);
	/*
	* wkeFireKeyPressEvent(charCode:int, flags:int) : boolean;
	*   charCode - WM_CHAR消息的The character code of the key.见 https://msdn.microsoft.com/en-us/library/windows/desktop/ms646276(v=vs.85).aspx
	*   flags - 参见wkeMouseFlags，可通过“或”操作并联
	*/
	wkeJsBindFunction("wkeFireKeyPressEvent", js_wkeFireKeyPressEvent, app, 2);
	/*
	* wkeFireWindowsMessage(hWnd:int, message:int, wParam:number, lParam:number) : boolean;
	*/
	wkeJsBindFunction("wkeFireWindowsMessage", js_wkeFireWindowsMessage, app, 4);
	
	/*
	* wkeSetFocus() : undefined;
	*/
	wkeJsBindFunction("wkeSetFocus", js_wkeSetFocus, app, 0);
	/*
	* wkeKillFocus() : undefined;
	*/
	wkeJsBindFunction("wkeKillFocus", js_wkeKillFocus, app, 0);
	
	/*
	* wkeSetZoomFactor(factor:float) : undefined;
	*/
	wkeJsBindFunction("wkeSetZoomFactor", js_wkeSetZoomFactor, app, 1);
	/*
	* wkeGetZoomFactor() : float;
	*/
	wkeJsBindFunction("wkeGetZoomFactor", js_wkeGetZoomFactor, app, 0);
	
	/*
	* wkeSetDragFiles(clientPoint_x:int, clientPoint_y:int, filePath:string|filePaths:Array_string) : undefined;
	*/
	wkeJsBindFunction("wkeSetDragFiles", js_wkeSetDragFiles, app, 3);
	
	/*
	* wkeIsMainFrame([frameId:int]) : boolean;
	*/
	wkeJsBindFunction("wkeIsMainFrame", js_wkeIsMainFrame, app, -1);
	/*
	* wkeIsWebRemoteFrame(frameId:int) : boolean;
	*/
	wkeJsBindFunction("wkeIsWebRemoteFrame", js_wkeIsWebRemoteFrame, app, 1);
	/*
	* wkeWebFrameGetMainFrame() : int;
	*/
	wkeJsBindFunction("wkeWebFrameGetMainFrame", js_wkeWebFrameGetMainFrame, app, 0);

	/*
	* wkeRunJsInAllFrames(js:string, [isInClosure:boolean]) : undefined;
	*/
	wkeJsBindFunction("wkeRunJsInAllFrames", js_wkeRunJsInAllFrames, app, -1);
	/*
	* wkeRunJsByFrame(frameId:int, js:string, [isInClosure:boolean]) : js result;
	*/
	wkeJsBindFunction("wkeRunJsByFrame", js_wkeRunJsByFrame, app, -1);
	/*
	* wkeInsertCSSByFrame(frameId:int, css:string) : undefined;
	*/
	wkeJsBindFunction("wkeInsertCSSByFrame", js_wkeInsertCSSByFrame, app, 2);
	
	/*
	* wkeCreateWebWindow(setting:map) : undefined;
	*   setting - {"url" : string, "hide" : boolean, "tran" : boolean, "width" : int, "height" - int, "preload" : string}
	*/
	wkeJsBindFunction("wkeCreateWebWindow", js_wkeCreateWebWindow, app, 1);
	/*
	* wkeDestroyWebWindow() : undefined;
	*/
	wkeJsBindFunction("wkeDestroyWebWindow", js_wkeDestroyWebWindow, app, 0);
	/*
	* wkeGetWindowHandle() : int;
	*/
	wkeJsBindFunction("wkeGetWindowHandle", js_wkeGetWindowHandle, app, 0);
	
	/*
	* wkeShowWindow(show:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeShowWindow", js_wkeShowWindow, app, 1);
	/*
	* wkeEnableWindow(enable:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeEnableWindow", js_wkeEnableWindow, app, 1);	
	/*
	* wkeMoveWindow(x:int, y:int, width:int, height:int) : undefined;
	*/
	wkeJsBindFunction("wkeMoveWindow", js_wkeMoveWindow, app, 4);
	/*
	* wkeMoveToCenter() : undefined;
	*/
	wkeJsBindFunction("wkeMoveToCenter", js_wkeMoveToCenter, app, 0);
	/*
	* wkeResizeWindow(width:int, height:int) : undefined;
	*/
	wkeJsBindFunction("wkeResizeWindow", js_wkeResizeWindow, app, 2);
	/*
	* wkeCreateResizeBorders(bLeft:boolean, bTop:boolean, bRight:boolean, bBottom:boolean, 
	*    bLeftTop:boolean, bLeftBottom:boolean, bRightTop:boolean, bRightBottom:boolean) : undefined;
	*/
	wkeJsBindFunction("wkeCreateResizeBorders", js_wkeCreateResizeBorders, app, 8);
	/*
	* wkeSetWindowTitle(title:string) : undefined;
	*/
	wkeJsBindFunction("wkeSetWindowTitle", js_wkeSetWindowTitle, app, 1);
	/*
	* wkeGetTitle() : string;
	*/
	wkeJsBindFunction("wkeGetTitle", js_wkeGetTitle, app, 0);
}
