#include "stdafx.h"
#include "BindEvents.h"
#include "MiniBlinkHostApp.h"
#include "wkeInputBoxW.h"

// 回调：文档加载成功
jsValue WKE_CALL_TYPE js_wkeOnDocumentReady(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnDocumentReady", jFunc);

	return jsBoolean(true);
}
bool g_defaultHtml = true;
void HandleDocumentReady2(wkeWebView webView, void* param, wkeWebFrameHandle frameId)
{
	Application* app = (Application*)param;

#ifdef _DEBUG
	const utf8* url = wkeGetFrameUrl(webView, frameId);
#endif

	// 加载preload 
	std::string sJS = ReadText(app->preloadFile.c_str());
	wkeRunJS(webView, sJS.c_str(), false);

	//执行js定义的事件
	jsExecState es = wkeGlobalExec(webView);
	jsValue jFunc = jsGetGlobal(es, "_wkeOnDocumentReady");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsInt((int)frameId);
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);
		delete[] argvs;
	}

	// 如果是第1次加载则是preload html，加载为preload html 后载加载主页
	if (g_defaultHtml)
	{// 加载主页
		g_defaultHtml = false;
		wkeLoadUrlW(app->window, app->url.c_str());
	}
}

// 回调：创建新的页面，比如说调用了 window.open 或者点击了 <a target="_blank" .../>
jsValue WKE_CALL_TYPE js_wkeOnCreateView(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnCreateView", jFunc);

	return jsBoolean(true);
}
wkeWebView HandleCreateView(wkeWebView webView, void* param, wkeNavigationType navType, const wkeString url, const wkeWindowFeatures* features)
{
	Application* app = (Application*)param;

	jsExecState es = wkeGlobalExec(webView);
	jsValue jFunc = jsGetGlobal(es, "_wkeOnCreateView");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[3];
		argvs[0] = jsInt((int)navType);
		argvs[1] = jsString(es, wkeGetString(url));
		jsValue jFeat = jsEmptyObject(es);
		jsSet(es, jFeat, "x", jsInt(features->x));
		jsSet(es, jFeat, "y", jsInt(features->y));
		jsSet(es, jFeat, "width", jsInt(features->width));
		jsSet(es, jFeat, "height", jsInt(features->height));
		jsSet(es, jFeat, "menuBarVisible", jsBoolean(features->menuBarVisible));
		jsSet(es, jFeat, "statusBarVisible", jsBoolean(features->statusBarVisible));
		jsSet(es, jFeat, "toolBarVisible", jsBoolean(features->toolBarVisible));
		jsSet(es, jFeat, "locationBarVisible", jsBoolean(features->locationBarVisible));
		jsSet(es, jFeat, "scrollbarsVisible", jsBoolean(features->scrollbarsVisible));
		jsSet(es, jFeat, "resizable", jsBoolean(features->resizable));
		jsSet(es, jFeat, "fullscreen", jsBoolean(features->fullscreen));
		argvs[2] = jFeat;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 3);
		delete[] argvs;
		if (jsIsNumber(jRet))
			return (wkeWebView)jsToInt(es, jRet);
		return nullptr;
	}

	//wkeWebView newWindow = wkeCreateWebWindow(WKE_WINDOW_TYPE_POPUP, wkeGetWindowHandle(webView), features->x, features->y, features->width, features->height);
	//wkeShowWindow(newWindow, true);
	//return newWindow;

	char filePath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, filePath, MAX_PATH);

	std::string sCmd = filePath;
	sCmd += " ";
	sCmd += wkeGetString(url);
	sCmd += " -width ";
	sCmd += std::to_string(features->width);
	sCmd += " -height ";
	sCmd += std::to_string(features->height);
	sCmd += " -tran ";
	sCmd += app->transparent ? "1" : "0";
	if (!app->preloadFile.empty())
	{
		sCmd += " -preload ";
		std::vector<char> vctSZ;
		WCharToMByte(app->preloadFile.c_str(), app->preloadFile.length(), &vctSZ, CP_ACP);
		vctSZ.push_back('\0');
		sCmd += vctSZ.data();
	}
	sCmd += " -simName ";
	sCmd += app->simName;

	WinExec(sCmd.c_str(), SW_SHOW);

	return NULL;
}

jsValue WKE_CALL_TYPE js_wkeOnMouseOverUrlChanged(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnMouseOverUrlChanged", jFunc);

	return jsBoolean(true);
}
void HandleMouseOverUrlChanged(wkeWebView webView, void* param, const wkeString url)
{
	jsExecState es = wkeGlobalExec(webView);	
	jsValue jFunc = jsGetGlobal(es, "_wkeOnMouseOverUrlChanged");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsString(es, wkeGetString(url));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnTitleChanged(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnTitleChanged", jFunc);

	return jsBoolean(true);
}
void HandleTitleChanged(wkeWebView webView, void* param, const wkeString title)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnTitleChanged");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsString(es, wkeGetString(title));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnUrlChanged2(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnUrlChanged2", jFunc);

	return jsBoolean(true);
}
void HandleUrlChanged2(wkeWebView webView, void* param, wkeWebFrameHandle frameId, const wkeString url)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnUrlChanged2");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[2];
		argvs[0] = jsInt((int)frameId);
		argvs[1] = jsString(es, wkeGetString(url));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 2);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnAlertBox(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnAlertBox", jFunc);

	return jsBoolean(true);
}
void HandleAlertBox(wkeWebView webView, void* param, const wkeString msg)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnAlertBox");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsString(es, wkeGetString(msg));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);

		delete[] argvs;
		return;
	}

	std::vector<wchar_t> wv;
	const utf8* pMsg = wkeGetString(msg);
	MByteToWChar(pMsg, strlen(pMsg), &wv, CP_UTF8);
	wv.push_back('\0');
	::MessageBoxW(nullptr, wv.data(), L"Miniblink Alert", MB_OK);
}

jsValue WKE_CALL_TYPE js_wkeOnConfirmBox(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnConfirmBox", jFunc);

	return jsBoolean(true);
}
bool HandleConfirmBox(wkeWebView webView, void* param, const wkeString msg)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnConfirmBox");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsString(es, wkeGetString(msg));

		bool bRet = true;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);
		if (jsIsBoolean(jRet))
			bRet = jsToBoolean(es, jRet);

		delete[] argvs;
		return bRet;
	}

	std::vector<wchar_t> wv;
	const utf8* pMsg = wkeGetString(msg);
	MByteToWChar(pMsg, strlen(pMsg), &wv, CP_UTF8);
	wv.push_back('\0');
	int result = ::MessageBoxW(NULL, wv.data(), L"Miniblink Confirm", MB_OKCANCEL);
	return result == IDOK;
}

jsValue WKE_CALL_TYPE js_wkeOnPromptBox(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnPromptBox", jFunc);

	return jsBoolean(true);
}
bool HandlePromptBox(wkeWebView webView, void* param, const wkeString msg, const wkeString defaultResult, wkeString result)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnPromptBox");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[3];
		argvs[0] = jsString(es, wkeGetString(msg));
		argvs[1] = jsString(es, wkeGetString(defaultResult));
		argvs[2] = jsString(es, wkeGetString(result));

		bool bRet = true;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 3);
		if (jsIsBoolean(jRet))
			bRet = jsToBoolean(es, jRet);

		delete[] argvs;
		return bRet;
	}

	std::vector<wchar_t> wv;
	const utf8* pMsg = wkeGetString(msg);
	MByteToWChar(pMsg, strlen(pMsg), &wv, CP_UTF8);
	wv.push_back('\0');
	int width = INPUT_BOX_WIDTH;
	int height = INPUT_BOX_HEIGHT;
	int parentWidth = 0;
	int parentHeight = 0;
	parentWidth = ::GetSystemMetrics(SM_CXSCREEN);
	parentHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int x = (parentWidth - width) / 2;
	int y = (parentHeight - height) / 2;
	_InitInputBoxW(NULL);
	wchar_t* pRes =  _InputBoxW(wv.data(), L"MiniBlink PromptBox", L"", x, y);	
	wkeSetStringW(result, pRes, wcslen(pRes));

	return true;
}

jsValue WKE_CALL_TYPE js_wkeOnNavigation(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnNavigation", jFunc);

	return jsBoolean(true);
}
bool HandleNavigation(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnNavigation");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[2];
		argvs[0] = jsInt((int)navigationType);
		argvs[1] = jsString(es, wkeGetString(url));

		bool bRet = true;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 2);
		if (jsIsBoolean(jRet))
			bRet = jsToBoolean(es, jRet);

		delete[] argvs;
		return bRet;
	}

	return true;
}

jsValue WKE_CALL_TYPE js_wkeOnLoadingFinish(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnLoadingFinish", jFunc);

	return jsBoolean(true);
}
void HandleLoadingFinish(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnLoadingFinish");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[3];
		argvs[0] = jsString(es, wkeGetString(url));
		argvs[1] = jsInt((int)result);
		argvs[2] = jsString(es, wkeGetString(failedReason));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 3);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnDownload(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnDownload", jFunc);

	return jsBoolean(true);
}
bool HandleDownload(wkeWebView webView, void* param, const char* url)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnDownload");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[1];
		argvs[0] = jsString(es, url);

		bool bRet = true;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 1);
		if (jsIsBoolean(jRet))
			bRet = jsToBoolean(es, jRet);

		delete[] argvs;
		return bRet;
	}

	return true;
}

jsValue WKE_CALL_TYPE js_wkeOnConsole(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnConsole", jFunc);

	return jsBoolean(true);
}
void HandleConsole(wkeWebView webView, void* param, wkeConsoleLevel level, const wkeString message, 
	const wkeString sourceName, unsigned sourceLine, const wkeString stackTrace)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnConsole");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[5];
		argvs[0] = jsInt((int)level);
		argvs[1] = jsString(es, wkeGetString(message));
		argvs[2] = jsString(es, wkeGetString(sourceName));
		argvs[3] = jsInt((int)sourceLine);
		argvs[4] = jsString(es, wkeGetString(stackTrace));

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 5);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnLoadUrlBegin(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnLoadUrlBegin", jFunc);

	return jsBoolean(true);
}
bool HandleLoadUrlBegin(wkeWebView webView, void* param, const char *url, void *job)
{
	jsExecState es = wkeGlobalExec(webView);

	bool bCancel = false;

	jsValue jFunc = jsGetGlobal(es, "_wkeOnLoadUrlBegin");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[2];
		argvs[0] = jsString(es, url);
		argvs[1] = jsInt((int)job);

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 2);
		if (jsIsBoolean(jRet))
			bCancel = jsToBoolean(es, jRet);

		delete[] argvs;
	}

	return bCancel;
}

jsValue WKE_CALL_TYPE js_wkeOnNetHookRequest(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnNetHookRequest", jFunc);

	return jsBoolean(true);
}
void HandleNetHookRequest(wkeWebView webView, void* param, const char* url, wkeNetJob job, void* buf, int len)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnNetHookRequest");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[4];
		argvs[0] = jsString(es, url);
		argvs[1] = jsInt((int)job);
		argvs[2] = jsString(es, (const utf8*)buf);
		argvs[3] = jsInt(len);

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 4);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnDidCreateScriptContext(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnDidCreateScriptContext", jFunc);

	return jsBoolean(true);
}
void HandleDidCreateScriptContext(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnDidCreateScriptContext");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[4];
		argvs[0] = jsInt((int)frameId);
		argvs[1] = jsInt((int)context);
		argvs[2] = jsInt(extensionGroup);
		argvs[3] = jsInt(worldId);

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 4);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnWillReleaseScriptContext(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnWillReleaseScriptContext", jFunc);

	return jsBoolean(true);
}
void HandleWillReleaseScriptContext(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int worldId)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnWillReleaseScriptContext");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[3];
		argvs[0] = jsInt((int)frameId);
		argvs[1] = jsInt((int)context);
		argvs[2] = jsInt(worldId);

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 3);

		delete[] argvs;
	}
}

// 回调：点击了关闭、返回 true 将销毁窗口，返回 false 什么都不做。
jsValue WKE_CALL_TYPE js_wkeOnWindowClosing(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnWindowClosing", jFunc);

	return jsBoolean(true);
}
bool HandleWindowClosing(wkeWebView webView, void* param)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnWindowClosing");
	if (jsIsFunction(jFunc))
	{
		jsValue jRet = jsCallGlobal(es, jFunc, NULL, 0);
		if (jsIsBoolean(jRet))
			return jsToBoolean(es, jRet);
	}

	return true;
}

// 回调：窗口已销毁
jsValue WKE_CALL_TYPE js_wkeOnWindowDestroy(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnWindowDestroy", jFunc);

	return jsBoolean(true);
}
void HandleWindowDestroy(wkeWebView webView, void* param)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnWindowDestroy");
	if (jsIsFunction(jFunc))
	{
		jsValue jRet = jsCallGlobal(es, jFunc, NULL, 0);
	}
	
	Application* app = (Application*)param;
	app->window = NULL;
	PostQuitMessage(0);
}

jsValue WKE_CALL_TYPE js_wkeOnWillMediaLoad(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnWillMediaLoad", jFunc);

	return jsBoolean(true);
}
void HandleWillMediaLoad(wkeWebView webView, void* param, const char* url, wkeMediaLoadInfo* info)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnWillMediaLoad");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[5];
		argvs[0] = jsString(es, url);
		argvs[1] = jsInt(info->size);
		argvs[2] = jsInt(info->width);
		argvs[3] = jsInt(info->height);
		argvs[4] = jsDouble(info->duration);

		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 5);

		delete[] argvs;
	}
}

jsValue WKE_CALL_TYPE js_wkeOnResponse(jsExecState es, void* param)
{
	int nArgc = jsArgCount(es);
	if (nArgc < 1)
		return jsBoolean(false);

	jsValue jFunc = jsArg(es, 0);
	if (!jsIsFunction(jFunc))
		return jsBoolean(false);

	jsSetGlobal(es, "_wkeOnResponse", jFunc);

	return jsBoolean(true);
}
bool HandleResponse(wkeWebView webView, void* param, const char* url, wkeNetJob job)
{
	jsExecState es = wkeGlobalExec(webView);

	jsValue jFunc = jsGetGlobal(es, "_wkeOnResponse");
	if (jsIsFunction(jFunc))
	{
		jsValue* argvs = new jsValue[2];
		argvs[0] = jsString(es, url);
		argvs[1] = jsInt((int)job);

		bool bCancel = false;
		jsValue jRet = jsCallGlobal(es, jFunc, argvs, 2);
		if (jsIsBoolean(jRet))
			bCancel = jsToBoolean(es, jRet);

		delete[] argvs;
		return bCancel;
	}

	return false;
}

jsValue WKE_CALL_TYPE js_wkeNetHookRequest(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsBoolean(false);

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsBoolean(false);

	wkeNetHookRequest((wkeNetJob)jsToInt(es, jv));

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetSetData(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	wkeNetJob job = NULL;
	utf8* newData = NULL;
	int nDataLen = 0;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{//job, newData
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv1 = jsArg(es, 1);
		newData = const_cast<utf8*>(jsToString(es, jv1));
		nDataLen = strlen(newData);
	}
	else if (nArgc == 3)
	{//job, newData, newDataLen
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv1 = jsArg(es, 1);
		newData = const_cast<utf8*>(jsToString(es, jv1));
		jsValue jv2 = jsArg(es, 2);
		if (!jsIsNumber(jv2))
			return jsBoolean(false);
		nDataLen = jsToInt(es, jv2);
	}

	if (job == NULL)
		return jsBoolean(false);

	wkeNetSetData(job, newData, nDataLen);

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetSetMIMEType(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		wkeNetJob job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv1 = jsArg(es, 1);
		const char* szMimeType = jsToString(es, jv1);
		wkeNetSetMIMEType(job, szMimeType);
		return jsBoolean(true);
	}

	return jsBoolean(false);
}

jsValue WKE_CALL_TYPE js_wkeNetGetMIMEType(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 1)
	{
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		wkeNetJob job = (wkeNetJob)jsToInt(es, jv0);
		const char* pMimeType = wkeNetGetMIMEType(job, nullptr);
		return jsString(es, pMimeType);
	}

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeNetSetHTTPHeaderField(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 3)
	{//0:job；1:object(header fields)；2:isResponse
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		wkeNetJob job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv2 = jsArg(es, 2);
		if (!jsIsBoolean(jv2))
			return jsBoolean(false);
		bool bResponse = jsToBoolean(es, jv2);
		jsValue jv1 = jsArg(es, 1);
		if (!jsIsObject(jv1))
			return jsBoolean(false);
		jsKeys* pKeys = jsGetKeys(es, jv1);
		for (int i = 0; i < pKeys->length; ++i)
		{
			const char* name = pKeys->keys[i];
			if (name[0] == '\0')
				continue;
			jsValue jProp = jsGet(es, jv1, name);
			const utf8* val = jsToString(es, jProp);
			wkeNetSetHTTPHeaderField(job, name, val, bResponse);
		}
		return jsBoolean(true);
	}
	else if (nArgc == 4)
	{//0:job；1:key, 2:value；3:isResponse
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		wkeNetJob job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsBoolean(false);
		const utf8* key = jsToString(es, jv1);
		if (key[0] == '\0')
			return jsBoolean(false);
		jsValue jv2 = jsArg(es, 2);
		if (!jsIsString(jv2))
			return jsBoolean(false);
		const utf8* val = jsToString(es, jv2);
		jsValue jv3 = jsArg(es, 3);
		if (!jsIsBoolean(jv3))
			return jsBoolean(false);
		bool bResponse = jsToBoolean(es, jv3);
		wkeNetSetHTTPHeaderField(job, key, val, bResponse);
		return jsBoolean(true);
	}

	return jsBoolean(false);
}

jsValue WKE_CALL_TYPE js_wkeNetGetHTTPHeaderField(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc == 2)
	{//0:job；1:key
		jsValue jv0 = jsArg(es, 0);
		if (!jsIsNumber(jv0))
			return jsBoolean(false);
		wkeNetJob job = (wkeNetJob)jsToInt(es, jv0);
		jsValue jv1 = jsArg(es, 1);
		if (!jsIsString(jv1))
			return jsBoolean(false);
		const utf8* key = jsToString(es, jv1);
		if (key[0] == '\0')
			return jsUndefined();
		const char* val = wkeNetGetHTTPHeaderField(job, key);
		return jsString(es, val);
	}

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeNetGetRequestMethod(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsUndefined();

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsUndefined();

	wkeRequestType requestType = wkeNetGetRequestMethod((wkeNetJob)jsToInt(es, jv));
	switch (requestType)
	{
	case kWkeRequestTypeInvalidation:
		return jsUndefined();
	case kWkeRequestTypeGet:
		return jsString(es, "GET");
	case kWkeRequestTypePost:
		return jsString(es, "POST");
	case kWkeRequestTypePut:
		return jsString(es, "PUT");
	}

	return jsUndefined();
}

jsValue WKE_CALL_TYPE js_wkeNetContinueJob(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsBoolean(false);

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsBoolean(false);

	wkeNetContinueJob((wkeNetJob)jsToInt(es, jv));

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetGetUrlByJob(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsUndefined();

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsUndefined();

	return jsString(es, wkeNetGetUrlByJob((wkeNetJob)jsToInt(es, jv)));
}

jsValue WKE_CALL_TYPE js_wkeNetCancelRequest(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsBoolean(false);

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsBoolean(false);

	wkeNetCancelRequest((wkeNetJob)jsToInt(es, jv));

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetChangeRequestUrl(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 2)
		return jsBoolean(false);

	jsValue jv0 = jsArg(es, 0);
	if (!jsIsNumber(jv0))
		return jsBoolean(false);

	jsValue jv1 = jsArg(es, 1);
	if (!jsIsString(jv1))
		return jsBoolean(false);
	
	wkeNetChangeRequestUrl((wkeNetJob)jsToInt(es, jv0), jsToString(es, jv1));

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetHoldJobToAsynCommit(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 1)
		return jsBoolean(false);

	jsValue jv = jsArg(es, 0);
	if (!jsIsNumber(jv))
		return jsBoolean(false);

	wkeNetHoldJobToAsynCommit((wkeNetJob)jsToInt(es, jv));

	return jsBoolean(true);
}

jsValue WKE_CALL_TYPE js_wkeNetCreateWebUrlRequest(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 3)
		return jsUndefined();

	jsValue jv0 = jsArg(es, 0);
	if (!jsIsString(jv0))
		return jsUndefined();

	jsValue jv1 = jsArg(es, 1);
	if (!jsIsString(jv1))
		return jsUndefined();

	jsValue jv2 = jsArg(es, 2);
	if (!jsIsString(jv2))
		return jsUndefined();

	wkeWebUrlRequestPtr requestPtr = wkeNetCreateWebUrlRequest(jsToString(es, jv0), jsToString(es, jv1), jsToString(es, jv2));
	return jsInt((int)requestPtr);
}

jsValue WKE_CALL_TYPE js_wkeNetAddHTTPHeaderFieldToUrlRequest(jsExecState es, void* param)
{
	Application* app = (Application*)param;

	int nArgc = jsArgCount(es);
	if (nArgc != 3)
		return jsBoolean(false);

	jsValue jv0 = jsArg(es, 0);
	if (!jsIsNumber(jv0))
		return jsBoolean(false);

	jsValue jv1 = jsArg(es, 1);
	if (!jsIsString(jv1))
		return jsBoolean(false);

	jsValue jv2 = jsArg(es, 2);
	if (!jsIsString(jv2))
		return jsBoolean(false);

	wkeNetAddHTTPHeaderFieldToUrlRequest(
		(wkeWebUrlRequestPtr)jsToInt(es, jv0),
		jsToString(es, jv1),
		jsToString(es, jv2));

	return jsBoolean(true);
}

void InitEvents(Application* app)
{
	/*
	* wkeOnDocumentReady(function(frameId:int){...});
	*/
	wkeJsBindFunction("wkeOnDocumentReady", js_wkeOnDocumentReady, app, 1);
	wkeOnDocumentReady2(app->window, HandleDocumentReady2, app);

	/*
	* wkeOnCreateView(function(navType:int, url:string, features:map){...});
	*   navType - 参见wkeNavigationType的定义；
	*   features - 参见wkeWindowFeatures的定义
	*/
	wkeJsBindFunction("wkeOnCreateView", js_wkeOnCreateView, app, 1);
	wkeOnCreateView(app->window, HandleCreateView, app);

	/*
	* wkeOnMouseOverUrlChanged(function(url:string){...});
	*/
	wkeJsBindFunction("wkeOnMouseOverUrlChanged", js_wkeOnMouseOverUrlChanged, app, 1);
	wkeOnMouseOverUrlChanged(app->window, HandleMouseOverUrlChanged, app);

	/*
	* wkeOnTitleChanged(function(title:string){...});
	*/
	wkeJsBindFunction("wkeOnTitleChanged", js_wkeOnTitleChanged, app, 1);
	wkeOnTitleChanged(app->window, HandleTitleChanged, app);

	/*
	* wkeOnUrlChanged(function(frameId:int, url:string){...});
	*/
	wkeJsBindFunction("wkeOnUrlChanged", js_wkeOnUrlChanged2, app, 1);
	wkeOnUrlChanged2(app->window, HandleUrlChanged2, app);

	/*
	* wkeOnAlertBox(function(msg:string){...});
	*/
	wkeJsBindFunction("wkeOnAlertBox", js_wkeOnAlertBox, app, 1);
	wkeOnAlertBox(app->window, HandleAlertBox, app);

	/*
	* wkeOnConfirmBox(function(msg:string){...});
	*/
	wkeJsBindFunction("wkeOnConfirmBox", js_wkeOnConfirmBox, app, 1);
	wkeOnConfirmBox(app->window, HandleConfirmBox, app);

	/*
	* wkeOnPromptBox(function(msg:string){...});
	*/
	wkeJsBindFunction("wkeOnPromptBox", js_wkeOnPromptBox, app, 1);
	wkeOnPromptBox(app->window, HandlePromptBox, app);

	/*
	* wkeOnNavigation(function(navigationType:int, url:string){...});
	*   navigationType - 参见wkeNavigationType的定义
	*/
	wkeJsBindFunction("wkeOnNavigation", js_wkeOnNavigation, app, 1);
	wkeOnNavigation(app->window, HandleNavigation, app);

	/*
	* wkeOnLoadingFinish(function(url:string, result:int, failedReason:string){...});
	*   result - 参见wkeLoadingResult的定义
	*/
	wkeJsBindFunction("wkeOnLoadingFinish", js_wkeOnLoadingFinish, app, 1);
	wkeOnLoadingFinish(app->window, HandleLoadingFinish, app);

	/*
	* wkeOnDownload(function(url:string){...});
	*/
	wkeJsBindFunction("wkeOnDownload", js_wkeOnDownload, app, 1);
	wkeOnDownload(app->window, HandleDownload, app);

	/*
	* wkeOnConsole(function(level:int, message:string, sourceName:string, sourceLine:int, stackTrace:string){...});
	*   level - 参见wkeConsoleLevel的定义
	*/
	wkeJsBindFunction("wkeOnConsole", js_wkeOnConsole, app, 1);
	wkeOnConsole(app->window, HandleConsole, app);

	/*
	* wkeOnDidCreateScriptContext(function(frameId:int, context:int, extensionGroup:int, worldId:int){...});
	*/
	wkeJsBindFunction("wkeOnDidCreateScriptContext", js_wkeOnDidCreateScriptContext, app, 1);
	wkeOnDidCreateScriptContext(app->window, HandleDidCreateScriptContext, app);

	/*
	* wkeOnWillReleaseScriptContext(function(frameId:int, context:int, worldId:int){...});
	*/
	wkeJsBindFunction("wkeOnWillReleaseScriptContext", js_wkeOnWillReleaseScriptContext, app, 1);
	wkeOnWillReleaseScriptContext(app->window, HandleWillReleaseScriptContext, app);

	/*
	* wkeOnWindowClosing(function(){...});
	*/
	wkeJsBindFunction("wkeOnWindowClosing", js_wkeOnWindowClosing, app, 1);
	wkeOnWindowClosing(app->window, HandleWindowClosing, app);

	/*
	* wkeOnWindowDestroy(function(){...});
	*/
	wkeJsBindFunction("wkeOnWindowDestroy", js_wkeOnWindowDestroy, app, 1);
	wkeOnWindowDestroy(app->window, HandleWindowDestroy, app);

	/*
	* wkeOnWillMediaLoad(function(url:string, size:int, width:int, height:int, duration:int){...});
	*/
	wkeJsBindFunction("wkeOnWillMediaLoad", js_wkeOnWillMediaLoad, app, 1);
	wkeOnWillMediaLoad(app->window, HandleWillMediaLoad, app);

	/*
	* wkeOnResponse(function(url:string, job:int){...});
	*/
	wkeJsBindFunction("wkeOnResponse", js_wkeOnResponse, app, 1);
	wkeOnResponse(app->window, HandleResponse, app);

	/*
	* wkeOnLoadUrlBegin(function(url:string, job:int){...});
	*/
	wkeJsBindFunction("wkeOnLoadUrlBegin", js_wkeOnLoadUrlBegin, app, 1);
	wkeOnLoadUrlBegin(app->window, HandleLoadUrlBegin, app);

	/*
	* wkeOnNetHookRequest(function(url:string, job:int, data:string, dataLen:int){...});
	*/
	wkeJsBindFunction("wkeOnNetHookRequest", js_wkeOnNetHookRequest, app, 1);
	wkeOnNetHookRequest(app->window, HandleNetHookRequest, app);

	/*
	* wkeNetHookRequest(job:int);
	*/
	wkeJsBindFunction("wkeNetHookRequest", js_wkeNetHookRequest, app, 1);

	/*
	* wkeNetSetData(job:int, newData:string, [newDataLen:int]);
	*/
	wkeJsBindFunction("wkeNetSetData", js_wkeNetSetData, app, -1);

	/*
	* wkeNetSetMIMEType(job:int, mimeType:string);
	*/
	wkeJsBindFunction("wkeNetSetMIMEType", js_wkeNetSetMIMEType, app, 2);

	/*
	* wkeNetGetMIMEType(job:int) : string;
	*/
	wkeJsBindFunction("wkeNetGetMIMEType", js_wkeNetGetMIMEType, app, 1);

	/*
	* wkeNetSetHTTPHeaderField(job:int, headerFields:map, isResponse:boolean);
	* wkeNetSetHTTPHeaderField(job:int, key:string, value:string, isResponse:boolean);
	*/
	wkeJsBindFunction("wkeNetSetHTTPHeaderField", js_wkeNetSetHTTPHeaderField, app, -1);

	/*
	* wkeNetGetHTTPHeaderField(job:int, key:string) : string;
	*/
	wkeJsBindFunction("wkeNetGetHTTPHeaderField", js_wkeNetGetHTTPHeaderField, app, 2);

	/*
	* wkeNetGetRequestMethod(job:int) : string;
	*/
	wkeJsBindFunction("wkeNetGetRequestMethod", js_wkeNetGetRequestMethod, app, 1);

	/*
	* wkeNetContinueJob(job:int);
	*/
	wkeJsBindFunction("wkeNetContinueJob", js_wkeNetContinueJob, app, 1);

	/*
	* wkeNetGetUrlByJob(job:int) : string;
	*/
	wkeJsBindFunction("wkeNetGetUrlByJob", js_wkeNetGetUrlByJob, app, 1);

	/*
	* wkeNetCancelRequest(job:int);
	*/
	wkeJsBindFunction("wkeNetCancelRequest", js_wkeNetCancelRequest, app, 1);

	/*
	* wkeNetChangeRequestUrl(job:int, newUrl:string);
	*/
	wkeJsBindFunction("wkeNetChangeRequestUrl", js_wkeNetChangeRequestUrl, app, 2);

	/*
	* wkeNetHoldJobToAsynCommit(job:int);
	*/
	wkeJsBindFunction("wkeNetHoldJobToAsynCommit", js_wkeNetHoldJobToAsynCommit, app, 1);

	
	/* 为了使得主页的wkeOnLoadUrlBegin等事件也能生效，
	 *   在加载主页前加载一个空白的页面，此时的空白页面为主页加载preload
	 */
	g_defaultHtml = true;
	wkeLoadHtml(app->window, "<html><html>");
}
