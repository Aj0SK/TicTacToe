#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include"C:\Users\andre_000\Desktop\tictactoe\tictactoelogic\tictactoelogic\game.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen      pen(Color(255, 0, 0, 255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		500,         // x position 
		500,         // y position 
		100,        // Button width
		20,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	ShowWindow(hWnd, SW_MAXIMIZE);//ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;

}
// current game
game my;
int psize = 100;
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
	{
		my.newgame(5, true);
		hdc = BeginPaint(hWnd, &ps);
		Graphics graphics(hdc);
		Pen      pen(Color(255, 34, 139, 34));
		for (int i = 0; i <= my.size(); ++i)graphics.DrawLine(&pen, i*psize, 0, i*psize, psize*my.size());
		for (int i = 0; i <= my.size(); ++i)graphics.DrawLine(&pen, 0, i*psize, psize*my.size(), i*psize);

		/*int a = 4, b = 4;
		graphics.DrawLine(&pen, b*polsize, a*polsize, (b + 1)*polsize, (a + 1)*polsize);
		graphics.DrawLine(&pen, (b + 1)*polsize, a*polsize, b*polsize, (a + 1)*polsize);

		a = 2; b = 3;
		graphics.DrawEllipse(&pen, b*polsize, a*polsize, polsize, polsize);

		EndPaint(hWnd, &ps);
		*/
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//MessageBox(hWnd, L"hPlus was clicked", NULL, MB_OK);
		InvalidateRect(hWnd, 0, 0);
		hdc = BeginPaint(hWnd, &ps);
		//
		Graphics graphics(hdc);
		Pen      pen(Color(255, 34, 139, 34));

		int a, b;
		a = LOWORD(lParam);
		b = HIWORD(lParam);

		if (a >= 0 && b >= 0 && a < my.size()*psize && b < my.size()*psize)
		{
			a = ((a / psize));
			b = ((b / psize));

			if (my.move(my.p_id, a, b))graphics.DrawEllipse(&pen, a*psize, b*psize, psize, psize);
			if (my.is_over())MessageBox(hWnd, L"END", NULL, MB_OK);
			pair<int, int>oko = my.aimove(my.c_id), pom = {-1, -1};
			if (oko != pom)
			{
				a = oko.first;
				b = oko.second;
				graphics.DrawLine(&pen, a*psize, b*psize, (a + 1)*psize, (b + 1)*psize);
				graphics.DrawLine(&pen, (a + 1)*psize, b*psize, a*psize, (b + 1)*psize);
			}
			if (my.is_over())MessageBox(hWnd, L"END", NULL, MB_OK);
		}
		EndPaint(hWnd, &ps);

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}