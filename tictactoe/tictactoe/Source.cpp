#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include"../../tictactoelogic/tictactoelogic/game.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

VOID OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen      pen(Color(255, 0, 0, 255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwndButton1, hwndButton2;

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

	hwndButton1 = CreateWindow(L"BUTTON", L"New Game with my start", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		900, 500, 170, 30, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	hwndButton2 = CreateWindow(L"BUTTON", L"New Game with pc start", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		900, 530, 170, 30, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

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
/////////////////////////////////////////////////////////
// current game
/////////////////////////////////////////////////////////
game my;
int gsize = 7;
int win = 3;
int psize = 100;
/////////////////////////////////////////////////////////

void funkcia(HWND &hWnd, game &toshow)
{
	InvalidateRect(hWnd, 0, true);

	PAINTSTRUCT  ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen      pen(Color(255, 34, 139, 34));
	
	for (int i = 0; i <= toshow.size(); ++i)graphics.DrawLine(&pen, i*psize, 0, i*psize, psize*toshow.size());
	for (int i = 0; i <= toshow.size(); ++i)graphics.DrawLine(&pen, 0, i*psize, psize*toshow.size(), i*psize);
	
	for (int i = 0; i < toshow.size(); ++i)
	{
		for (int j = 0; j < toshow.size(); ++j)
		{
			if (toshow.owner(i, j) == 1)
			{
				graphics.DrawLine(&pen, j*psize, i*psize, (j + 1)*psize, (i + 1)*psize);
				graphics.DrawLine(&pen, (j + 1)*psize, i*psize, j*psize, (i + 1)*psize);
			}
			if (toshow.owner(i, j) == 2)
			{
				graphics.DrawEllipse(&pen, j*psize , i*psize , psize , psize );
			}
		}
	}

	EndPaint(hWnd, &ps);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//HDC          hdc;
	//PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
	{	
		return 0;
	}
	case WM_LBUTTONUP:
	{	
		int a, b;
		a = HIWORD(lParam);
		b = LOWORD(lParam);

		if (a >= 0 && b >= 0 && a < my.size()*psize && b < my.size()*psize)
		{
			a = ((a / psize));
			b = ((b / psize));

			if (my.move(my.p_id, a, b) && !my.is_over())
			{
				pair<int, int>oko = my.aimove(my.c_id), pom = { -1, -1 };
				if (oko != pom)
				{
					a = oko.first;
					b = oko.second;
				}
			}
			
		}
		funkcia(hWnd, my);
		if (my.is_over())
		{
			if (my.status() == my.p_id)MessageBox(hWnd, L"Win", NULL, MB_OK);
			else if (my.status() == my.c_id)MessageBox(hWnd, L"Loose", NULL, MB_OK);
			else MessageBox(hWnd, L"Draw", NULL, MB_OK);
		}
		return 0;
	}
	case WM_COMMAND:
	{
		if ((HWND)lParam == hwndButton1 || (HWND)lParam == hwndButton2)
		{
			if ((HWND)lParam == hwndButton1)
			{
				my.newgame(gsize, true, win);
				funkcia(hWnd, my);
				//MessageBox(hWnd, L"Ok1 stlacene", NULL, MB_OK);
			}
			else if ((HWND)lParam == hwndButton2)
			{
				my.newgame(gsize, false, win);
				funkcia(hWnd, my);
				//MessageBox(hWnd, L"Ok2 stlacene", NULL, MB_OK);
			}
		}
		
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

/*
int a = 4, b = 4;
graphics.DrawLine(&pen, b*polsize, a*polsize, (b + 1)*polsize, (a + 1)*polsize);
graphics.DrawLine(&pen, (b + 1)*polsize, a*polsize, b*polsize, (a + 1)*polsize);

a = 2; b = 3;
graphics.DrawEllipse(&pen, b*polsize, a*polsize, polsize, polsize);

EndPaint(hWnd, &ps);
*/