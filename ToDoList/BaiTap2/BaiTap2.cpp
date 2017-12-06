// BaiTap2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BaiTap2.h"
#include <string.h>
#include<windowsx.h>
#include <winuser.h>
#include <commctrl.h>
#include <string.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"

#define MAX_LOADSTRING 100
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void ReadFile();
void WriteFile(HWND hWnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
#define MAX_LOADSTRING 100
int Width, Height;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	HRESULT hr = CoInitialize(NULL);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BAITAP2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BAITAP2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	CoUninitialize();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;//CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BAITAP2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BAITAP2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable
   ReadFile();
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, Width, Height, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

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

#define CreateTextBox(text, left, top, width, height, ID) 

HWND txtToDo;
HWND btAdd;
HWND btDel[10];
HWND txtShow[10];
int nHeight=200;

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	InitializeFramework(hWnd);
	btAdd = CreateWindowEx(0, L"BUTTON", L"ADD",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 150, 40, 40, 
		hWnd, (HMENU)IDC_BUTTON, hInst, NULL);
	SendMessage(btAdd, WM_SETFONT, WPARAM(hFont), TRUE);

	//Create checkbox todolist & button delete
	for (int i = 0; i < 5; i++)
	{
		btDel[i] = CreateWindowEx(0, L"BUTTON", L"DEL",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			500, nHeight, 40, 40,
			hWnd, (HMENU)(i+1), hInst, NULL);
		SendMessage(btDel[i], WM_SETFONT, WPARAM(hFont), TRUE);
		ShowWindow(btDel[i], SW_HIDE);

		txtShow[i] = CreateWindowEx(0, L"BUTTON", L""
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			200, nHeight, 200, 40,
			hWnd, NULL, hInst, NULL);
		SendMessage(txtShow[i], WM_SETFONT, WPARAM(hFont+5), TRUE);
		ShowWindow(txtShow[i], SW_HIDE);
		nHeight += 70;
	}

	nHeight = 200;
	for (int i = 5; i < 10; i++)
	{
		btDel[i] = CreateWindowEx(0, L"BUTTON", L"DEL",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			900, nHeight, 40, 40,
			hWnd, (HMENU)(i + 1), hInst, NULL);
		SendMessage(btDel[i], WM_SETFONT, WPARAM(hFont), TRUE);
		ShowWindow(btDel[i], SW_HIDE);

		txtShow[i] = CreateWindowEx(0, L"BUTTON", L""
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			600, nHeight, 200, 40,
			hWnd, NULL, hInst, NULL);
		SendMessage(txtShow[i], WM_SETFONT, WPARAM(hFont+5), TRUE);
		ShowWindow(txtShow[i], SW_HIDE);
		nHeight += 70;
	}

	//
	HWND hwnd = CreateWindowEx(0, L"STATIC", L"What do you want to do?",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		250, 150, 150, 40,
		hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont+10), TRUE);

	txtToDo = CreateWindowEx(0, L"EDIT", L""
		, WS_CHILD | WS_VISIBLE | WS_BORDER,
		450, 150, 300, 50,
		hWnd, NULL, hInst, NULL);
	SendMessage(txtToDo, WM_SETFONT, WPARAM(hFont), TRUE);

	return true;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR* buffer1 = NULL;
	WCHAR* buffer2 = NULL;
	WCHAR* bufferKQ = NULL;

	char size1;
	char size2;

	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDC_BUTTON:
		size1 = GetWindowTextLength(txtToDo);
		buffer1 = new WCHAR[size1 + 1];

		// Check Todo box empty or not
		if (size1 == 0)
		{
			MessageBox(0, L"Please fill down what you want to do?", L"Warning", 0);
			break;
		}
		GetWindowText(txtToDo, buffer1, size1 + 1);
		bufferKQ = new WCHAR[size1 + 1];
		wsprintf(bufferKQ, L"%s", buffer1);
		// Check coincidence activity
		for (int i = 0; i < 10; i++)
		{
			size2 = GetWindowTextLength(txtShow[i]);
			if (size2 == size1) {
				MessageBox(0, L"You already have this activity in To Do List!", L"Warning", 0);
				break;
			}
			else if (!IsWindowVisible(txtShow[i]))
			{
				SetWindowText(txtShow[i], bufferKQ);
				ShowWindow(txtShow[i], SW_SHOW);
				ShowWindow(btDel[i], SW_SHOW);
				break;
			}
		}

		SetWindowText(txtToDo, L"");

		break;
		//Button delete have defined in resource.h
	case 1:
		ShowWindow(txtShow[0], SW_HIDE);
		ShowWindow(btDel[0], SW_HIDE);
		break;

	case 2:
		ShowWindow(txtShow[1], SW_HIDE);
		ShowWindow(btDel[1], SW_HIDE);
		break;

	case 3:
		ShowWindow(txtShow[2], SW_HIDE);
		ShowWindow(btDel[2], SW_HIDE);
		break;

	case 4:
		ShowWindow(txtShow[3], SW_HIDE);
		ShowWindow(btDel[3], SW_HIDE);
		break;

	case 5:
		ShowWindow(txtShow[4], SW_HIDE);
		ShowWindow(btDel[4], SW_HIDE);
		break;

	case 6:
		ShowWindow(txtShow[5], SW_HIDE);
		ShowWindow(btDel[5], SW_HIDE);
		break;
	case 7:
		ShowWindow(txtShow[6], SW_HIDE);
		ShowWindow(btDel[6], SW_HIDE);
		break;

	case 8:
		ShowWindow(txtShow[7], SW_HIDE);
		ShowWindow(btDel[7], SW_HIDE);
		break;

	case 9:
		ShowWindow(txtShow[8], SW_HIDE);
		ShowWindow(btDel[8], SW_HIDE);
		break;

	case 10:
		ShowWindow(txtShow[9], SW_HIDE);
		ShowWindow(btDel[9], SW_HIDE);
		break;

	}

	if (!buffer1)
		delete[] buffer1;
	if (!buffer2)
		delete[] buffer2;
	if (!bufferKQ)
		delete[] bufferKQ;
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	WriteFile(hwnd);
	DestroyFramework();
	PostQuitMessage(0);
}
void ReadFile() {
	const int BUFFERSIZE = 260;
	WCHAR bufferwidth[BUFFERSIZE];
	WCHAR bufferheight[BUFFERSIZE];

	WCHAR curPath[BUFFERSIZE];
	WCHAR configPath[BUFFERSIZE];

	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\config.ini", curPath);

	GetPrivateProfileString(L"app", L"width", L"width value", bufferwidth, BUFFERSIZE, configPath);
	GetPrivateProfileString(L"app", L"height", L"height value", bufferheight, BUFFERSIZE, configPath);
	Width = _wtoi(bufferwidth);
	Height = _wtoi(bufferheight);
}

void WriteFile(HWND hWnd) {
	const int BUFFERSIZE = 260;
	WCHAR bufferwidth[BUFFERSIZE];
	WCHAR bufferheight[BUFFERSIZE];

	WCHAR curPath[BUFFERSIZE];
	WCHAR configPath[BUFFERSIZE];

	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\config.ini", curPath);
	//Get size of current window
	RECT main;
	GetWindowRect(hWnd, &main);

	int width = main.right - main.left;
	int height = main.bottom - main.top;
	wsprintf(bufferwidth, L"%d", width);
	wsprintf(bufferheight, L"%d", height);
	//Save to file ini
	WritePrivateProfileString(L"app", L"width", bufferwidth, configPath);
	WritePrivateProfileString(L"app", L"height", bufferheight, configPath);
}