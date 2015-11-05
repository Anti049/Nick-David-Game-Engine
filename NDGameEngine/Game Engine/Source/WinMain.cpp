#include "Precompiled.h"
#include "Engine.h"

#define WINDOW_CLASS L"NDGameEngine"
#define WINDOW_TITLE L"Nick and David\'s Game Engine"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

BOOL IsAlreadyRunning(void);
BOOL RegisterWindowClass(HINSTANCE hInstance);
HWND MakeWindow(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Engine* pEngine = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetUnhandledExceptionFilter(ErrorFunction);

	HWND hWnd;
	
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = WINDOW_CLASS;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	DWORD errorCode = GetLastError();
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	wndClass.hIcon = NULL;
	RegisterClassEx(&wndClass);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		WINDOW_CLASS,    // name of the window class
		WINDOW_TITLE,   // title of the window
		WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),    // window style
		CW_USEDEFAULT,    // x-position of the window
		CW_USEDEFAULT,    // y-position of the window
		WINDOW_WIDTH,    // width of the window
		WINDOW_HEIGHT,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL
	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:
	pEngine = new Engine;
	pEngine->Initialize(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT, false);

	// this struct holds Windows event messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// wait for the next message in the queue, store the result in 'msg'
	while (msg.message != WM_QUIT && pEngine->Main() != false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pEngine->Terminate();
	SafeDelete(pEngine);

	// return this part of the WM_QUIT message to Windows
	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hWnd, message, wParam, lParam))
		return 0;
	switch (message)
	{
	case WM_SETCURSOR:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		/*WORD ht = LOWORD(lParam);
		static bool hiddencursor = false;
		if (HTCLIENT == ht && !hiddencursor)
		{
		hiddencursor = true;
		ShowCursor(false);
		}
		else if (HTCLIENT != ht && hiddencursor)
		{
		hiddencursor = false;
		ShowCursor(true);
		}*/
	}
		break;

	case WM_CREATE:
	{
	}
		break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
		break;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
	}
		break;

	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			// Unpause game here
		}
		else
		{
			// Pause game here
		}
	}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT	ps;
		HDC			hdc;

		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_SYSCHAR:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	{
		//Input::SetKeyboardState();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}