#ifndef WKEXE_APP_H
#define WKEXE_APP_H

#include <wke.h>

class Application
{
public:
	Application();

	~Application();


	void PrintHelp();	
	// 创建主页面窗口
	BOOL CreateWebWindow();
	void PrintHelpAndQuit();
	void RunMessageLoop();
	void RunApplication();
	void QuitApplication();

public:
    wkeWebView window;
	
	bool hide;
	bool transparent;
	int width;
	int height;
	class simdb* db;

	const wchar_t* Url() { return url.c_str(); }
	const wchar_t* PrerunCode() { return prerunCode.c_str(); }
	const wchar_t* PreloadFile() { return preloadFile.c_str(); }
	const char* SimName() { return simName.c_str();  }

private:
	BOOL ProcessCommandLine();

private:
	std::wstring url;
	std::wstring preloadFile;
	std::wstring prerunCode;
	std::string simName;
	std::vector<std::string> plugins;
};


void RunApplication(Application* app);

#endif//#ifndef WKEXE_APP_H