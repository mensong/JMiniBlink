// MiniBlinkHost.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MiniBlinkHost.h"
#include "MiniBlinkHostApp.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	{
		Application app;
		app.RunApplication();
	}

    return 0;
}
