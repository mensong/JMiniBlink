#ifndef WKEXE_APP_H
#define WKEXE_APP_H

#include <wke.h>
#include <atltime.h>
#include <string>

class Application
{
public:
	Application()
	{
		wkeInitialize();

		window = NULL;

		hide = false;
		transparent = false;
		width = 1024;
		height = 768;
	}

	~Application()
	{
		wkeFinalize();
	}

    wkeWebView window;

	std::wstring url;
	bool hide;
	bool transparent;
	int width;
	int height;
	std::wstring preloadFile;
};


void RunApplication(Application* app);

#endif//#ifndef WKEXE_APP_H