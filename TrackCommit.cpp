// TrackCommit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TrackCommit.h"
#include "model.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


#define WM_USER_SHELLICON (WM_APP+1)
#define IDT_TIMERGLOBAL			200
#define IDT_TIMERPING			201
#define IDC_PONG_BUTTON			202
#define IDC_DDD_LABEL			202

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TRACKCOMMIT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}


	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRACKCOMMIT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRACKCOMMIT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TRACKCOMMIT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

void initSystray(HWND hWnd, HINSTANCE hInstance) {
	NOTIFYICONDATA nidApp;
	nidApp.hWnd = hWnd;
	nidApp.uID = IDI_TRACKCOMMIT;
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nidApp.hIcon = (HICON)GetClassLong(hWnd, GCL_HICON);
	nidApp.uCallbackMessage = WM_USER_SHELLICON;
	LoadString(hInstance, IDS_APP_TITLE, nidApp.szTip, MAX_LOADSTRING);

	Shell_NotifyIcon(NIM_ADD, &nidApp);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
      CW_USEDEFAULT, 0, 275, 154, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   initSystray(hWnd, hInstance);

   SetTimer(hWnd,            // handle to main window 
	   IDT_TIMERGLOBAL,		 // timer identifier 
	   1000,                 // 1-seconds interval 
	   (TIMERPROC)NULL);     // no timer callback 

   return TRUE;
}

void CenterWindow(HWND hwndWindow)
{
	RECT rect;
	GetWindowRect(hwndWindow, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//SetWindowPos(hwndWindow, NULL, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, 0);
	MoveWindow(hwndWindow, (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void createGUI(HWND hWnd) {
	HWND button = CreateWindowEx(NULL,
		L"BUTTON",
		L"OK",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_DISABLED | BS_DEFPUSHBUTTON,
		150, 63, 100, 24,
		hWnd,
		(HMENU)IDC_PONG_BUTTON,
		GetModuleHandle(NULL),
		NULL);

	HWND label = CreateWindowEx(NULL,
		L"STATIC",
		L"ding-ding-dong",
		WS_VISIBLE | WS_CHILD,
		10,	15, 100, 24,
		hWnd,
		(HMENU)IDC_DDD_LABEL,
		GetModuleHandle(NULL),
		NULL);

	HFONT defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(label, WM_SETFONT, WPARAM(defaultFont), TRUE);
	SendMessage(button, WM_SETFONT, WPARAM(defaultFont), TRUE);
}


void showWindow(HWND hWnd) {
	SetForegroundWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	CenterWindow(hWnd);
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		createGUI(hWnd);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_PONG_BUTTON:
			KillTimer(hWnd, IDT_TIMERPING);
			model::pong();
			EnableWindow(GetDlgItem(hWnd, IDC_PONG_BUTTON), FALSE);
			ShowWindow(hWnd, SW_HIDE);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		switch (wParam) {
		case IDT_TIMERGLOBAL:
			// if a ping has not already been launched.
			if (IsWindowEnabled(GetDlgItem(hWnd, IDC_PONG_BUTTON)) == FALSE) {
				// get local time.
				SYSTEMTIME currentTime;
				GetLocalTime(&currentTime);
				// if minute is 0, 15, 30 or 45
				if ((currentTime.wMinute % 15) == 0) {
					// get last local time
					LPSYSTEMTIME lastTime = model::getLastLocalTime();
					// if last time is not the same time, launch the thing.
					if (lastTime == 0 ||
						lastTime->wMinute != currentTime.wMinute ||
						lastTime->wHour != currentTime.wHour ||
						lastTime->wDay != currentTime.wDay ||
						lastTime->wMonth != currentTime.wMonth ||
						lastTime->wYear != currentTime.wYear) {

						// ping.
						EnableWindow(GetDlgItem(hWnd, IDC_PONG_BUTTON), TRUE);
						showWindow(hWnd);
						SetTimer(hWnd,            // handle to main window 
							IDT_TIMERPING,		  // timer identifier 
							15000,                // 15-seconds interval 
							(TIMERPROC)NULL);     // no timer callback 
					}
				}
			}
			break;
		case IDT_TIMERPING:
			// 15 seconds have elapsed, no pong.
			KillTimer(hWnd, IDT_TIMERPING);
			model::failPong();
			EnableWindow(GetDlgItem(hWnd, IDC_PONG_BUTTON), FALSE);
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	case WM_CLOSE:
		// don't close, just hide.
		ShowWindow(hWnd, SW_HIDE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		NOTIFYICONDATA nidApp;
		nidApp.hWnd = hWnd;
		nidApp.uID = IDI_TRACKCOMMIT;
		Shell_NotifyIcon(NIM_DELETE, &nidApp);
		PostQuitMessage(0);
		break;
	case WM_USER_SHELLICON:
		switch (LOWORD(lParam))
		{
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
			if (IsWindowVisible(hWnd)) {
				ShowWindow(hWnd, SW_HIDE);
			}
			else {
				showWindow(hWnd);
			}
			//OutputDebugString(L"Hey\n");
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
