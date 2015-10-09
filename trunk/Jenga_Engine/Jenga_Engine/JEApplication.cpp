/*******************************************************************************
Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
filename    JEApplication.cpp
author      Jaeheon Lee
email       jhoney4344@gmail.com
course      GAM250
assignment  Final Submission
due date    12/6/2015

Brief Description:
  This file is managing from application's creation to destroying

*******************************************************************************/
#include "JEApplication.h"
#include "JEGameData.h"

LPCTSTR wndClassName = "Team SOJU Game";

extern JE::GameData gd;


namespace{
	HWND g_hWnd;
	RECT winRect, scrRect;
	unsigned int winWidth, winHeight;
	unsigned int scrWidth, scrHeight;
	const DWORD FULLSCREEN_STYLE = WS_POPUP | WS_VISIBLE;
	const DWORD WINDOWED_STYLE = WS_POPUP | WS_CAPTION;
	DWORD winStyle;
}


/*******************************************************************************
   Function: JE_APPLICATION

Description: This function set the application option.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
JE_APPLICATION::JE_APPLICATION(void)
{
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// Only WHTIE,BLACK,GRAY
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	HANDLE hIcon = LoadImage(NULL, "icon1.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	WndClass.hIcon = (HICON)hIcon;

	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = wndClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;// re-draw window when being changed	
}
/*******************************************************************************
   Function: Init

Description: Make and register the game window.

     Inputs: nCmdShow - this is the data that get from the main.cpp file.

    Outputs: None.
*******************************************************************************/
void JE_APPLICATION::Init(int nCmdShow)
{
	winStyle = WINDOWED_STYLE;
	DEVMODE dm = {0};
	dm.dmSize = sizeof(dm);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

	WndClass.hInstance = data.hInstance;
	int startX;
	int startY;

	RegisterClass(&WndClass);
	InitWinHeight( data.height, data.width);
	
	winRect.left = winRect.top = 0;
	winRect.right = data.width;
	winRect.bottom = data.height;
	//Get window size that user realy want.
	AdjustWindowRect(&winRect,winStyle, FALSE);
	winWidth  = winRect.right - winRect.left;
	winHeight = winRect.bottom - winRect.top;

	scrWidth = data.width;
	scrHeight = data.height;

	gd.padConnected = false;
	gd.padTime = 0.f;
	gd.padAlpha = 0.f;

	startX =( (dm.dmPelsWidth/2 - winWidth/2) );
	startY =( (dm.dmPelsHeight/2 - winHeight/2) );

	g_hWnd = CreateWindow(wndClassName, data.title, 
		winStyle,//ws_overlappedwindow set default
		startX, startY, winWidth, winHeight, NULL, (HMENU)NULL, data.hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	
}

/*******************************************************************************
   Function: Update

Description: Run the game application window. Call the state manager so that
             Game will running by 60 frame per second.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void JE_APPLICATION::Update(void)
{

	JEInput input;
	input.Init();

	JE::StateManager::Init();
	JE::StateManager::Update(g_hWnd);
	JE::StateManager::ShutDown();
}

/*******************************************************************************
   Function: ShutDown

Description: Release the resource.

     Inputs: None.

    Outputs: None.
*******************************************************************************/
void JE_APPLICATION::ShutDown(void)
{
	JE::Debug::CloseConsole();
}

/*******************************************************************************
   Function: WndProc

Description: The core procedure function for window application.

     Inputs: hWnd - handle for this window
			 iMessage - message from user.
			 wParam - w type data.
			 lParam - l type data.

    Outputs: None.
*******************************************************************************/
LRESULT CALLBACK JE_APPLICATION::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
	RECT rec;
	
	GetClientRect(hWnd,&rec);

	switch(iMessage)
	{

	case WM_CREATE:
		JE::Debug::CreateConsole();
		JE::GraphicInit(hWnd, rec.left, rec.right, rec.top, rec.bottom);
		break;
	case WM_KEYDOWN:
		g_Input->SetPressed(JETranslate(wParam), true);
		break;

	case WM_SIZE:
		{
			GetClientRect(hWnd, &scrRect);// Get screen size
			//GetWindowRect(hWnd, &scrRect); // Get window size
			scrWidth  = scrRect.right - scrRect.left;
			scrHeight = scrRect.bottom - scrRect.top;
			break;
		}

	case WM_KEYUP:
		g_Input->SetPressed(JETranslate(wParam), false);
		break;

	case WM_LBUTTONDOWN:
		g_Input->SetPressed(JE_MOUSE_LEFT, true);
		break;

	case WM_LBUTTONUP:
		g_Input->SetPressed(JE_MOUSE_LEFT, false);
		break;

	case WM_RBUTTONDOWN:
		g_Input->SetPressed(JE_MOUSE_RIGHT, true);

		break;

	case WM_RBUTTONUP:
		g_Input->SetPressed(JE_MOUSE_RIGHT, false);
		break;

    case WM_MOUSEMOVE:
		g_Input->SetMouse_x(GET_X_LPARAM(lParam));
		g_Input->SetMouse_y(GET_Y_LPARAM(lParam));
		break;

	case WM_ACTIVATE:

		if (LOWORD(wParam) == WA_INACTIVE) 
		{
			gd.windowInactive = true;
			gd.pause = true;
			if (gd.fullScreen ==true)
				JE::APP::FullScreen(false);
			ShowWindow(hWnd, SW_MINIMIZE);
		}
		else if (LOWORD(wParam) != WA_INACTIVE) // when active
			gd.windowInactive = false;
		
		 break;

	case WM_PAINT:
		hDc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	// The signal of end. It should be checked where PeekMessage is used on.
	//case WM_QUIT:
	//		break;

		//Not release all things.
	case WM_CLOSE:
		JE::GraphicsShutdown();
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd,iMessage,wParam,lParam));
	}
    return 0;
}
/*******************************************************************************
   Function: FullScreen

Description: Change window screen mode.

     Inputs: scrType - bool type for check fullscreen.


    Outputs: None.
*******************************************************************************/
void JE::APP::FullScreen(bool scrType)
{
	RECT rt = { 0, 0, scrWidth, scrHeight };
	if (scrType)
	{
		winStyle = FULLSCREEN_STYLE;

		DEVMODE dmScreenSettings;

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

		// change screen resolution.
		dmScreenSettings.dmPelsWidth = scrWidth;
		dmScreenSettings.dmPelsHeight = scrHeight;
		//dmScreenSettings.dmBitsPerPel = 32; // bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			winStyle = WINDOWED_STYLE;
			ChangeDisplaySettings(NULL, 0);
			//some problems found do something.
		}
		gd.fullScreen = true;
	}
	else
	{
		gd.fullScreen = false;
		ChangeDisplaySettings(NULL, 0);/*If So Switch Back To The Desktop*/
		/*Make sure I am in windows style*/
		winStyle = WINDOWED_STYLE;
		rt.right = GetWinWidth();
		rt.bottom = GetWinHeight();
	}



		// change window style
		SetWindowLong(g_hWnd, GWL_STYLE, winStyle );


		// Adjust center window
		DEVMODE dm = {0};
		memset(&dm, 0, sizeof(dm));
		// Get the size of the screen
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

		
		//Set real screen size from window size.
		AdjustWindowRect(&rt, winStyle, 0);
		winWidth = rt.right - rt.left;
		winHeight = rt.bottom - rt.top;

		//Get start position for center
		int xStart = (dm.dmPelsWidth/2) - (winWidth/2);
		int yStart = (dm.dmPelsHeight/2) - (winHeight/2);
		
		MoveWindow(g_hWnd, xStart, yStart, winWidth, winHeight, TRUE);//last param is repaint
		//This is required after SetWindowLong
		ShowWindow(g_hWnd, SW_SHOWNORMAL);
		//This sets my window to the front
		SetForegroundWindow(g_hWnd);
}