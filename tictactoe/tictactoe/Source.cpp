#include <windows.h>
#include <objidl.h>
#include <string>
#include <gdiplus.h>

#include "../../tictactoelogic/tictactoelogic/game.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwndButton1, hwndButton2;
HWND TextBox1, TextBox2;

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
		0, 0, 170, 30, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	hwndButton2 = CreateWindow(L"BUTTON", L"New Game with pc start", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		0, 30, 170, 30, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

	TextBox1 = CreateWindow(L"Edit", L"Board Size", 
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		170, 0, 100, 30, hWnd,
		NULL, NULL, NULL);
	TextBox2 = CreateWindow(L"Edit", L"To win",
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		170, 30, 100, 30, hWnd,
		NULL, NULL, NULL);

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
int gsize = 5;
int win = 3;
int psize = 50;
int offsetx = 10, offsety = 90;
/////////////////////////////////////////////////////////

//Function which drawes actual game
void draw(HWND &hWnd, game &toshow)
{
	InvalidateRect(hWnd, 0, true);

	//sets graphic properties
	PAINTSTRUCT  ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	// if you hate my colors :P, you can change it here :
	Pen game_pen(Color(255, 255, 0, 0));
	Pen	circle_pen(Color(255, 0, 0, 0));
	Pen cross_pen(Color(255, 34, 139, 34));
	
	for (int i = 0; i <= toshow.size(); ++i)
	{
		graphics.DrawLine(&game_pen, offsetx + i*psize, offsety + 0, offsetx + i*psize, offsety + psize*toshow.size());//draw vertical lines
		graphics.DrawLine(&game_pen, offsetx + 0, offsety + i*psize, offsetx + psize*toshow.size(), offsety + i*psize);//draws horizontal lines
	}

	for (int i = 0; i < toshow.size(); ++i)
	{
		for (int j = 0; j < toshow.size(); ++j)
		{
			if (toshow.owner(i, j) == 1)//we have to draw cross
			{
				graphics.DrawLine(&cross_pen, offsetx+j*psize, offsety+i*psize, offsetx+(j + 1)*psize, offsety+(i + 1)*psize);
				graphics.DrawLine(&cross_pen, offsetx+(j + 1)*psize, offsety+i*psize, offsetx+j*psize, offsety+(i + 1)*psize);
			}
			if (toshow.owner(i, j) == 2)//we have to draw circle
			{
				graphics.DrawEllipse(&circle_pen, offsetx+j*psize, offsety+i*psize, psize, psize );
			}
		}
	}

	EndPaint(hWnd, &ps);
}

//Function which manages user events as buttons and click
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
	{	
		int a, b;
		a = HIWORD(lParam);
		b = LOWORD(lParam);

		if (a >= offsety && b >= offsetx && a < offsety+my.size()*psize && b < offsetx + my.size()*psize )
		{
			a = (((a - offsety) / psize));
			b = (((b - offsetx) / psize));

			if (my.move(my.p_id, a, b) && !my.is_over())
			{
				pair<int, int>mo = my.aimove(my.c_id), pom = { -1, -1 };

				if (mo != pom)
				{
					a = mo.first;
					b = mo.second;
				}
			}
		}

		draw(hWnd, my);
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
			int value1 = 0, value2 = 0, pom = 0;
			wchar_t a[30] = { 0 }, b[30] = { 0 };

			pom = GetWindowText(TextBox1, a, 20);
			pom = GetWindowText(TextBox2, b, 20);

			value1 = _wtoi(a);
			value2 = _wtoi(b);

			RECT rect;
			GetWindowRect(hWnd, &rect);

			if ( my.is_createable(value1, value2) )
			{
				gsize = value1;
				win = value2;
				psize = min(rect.right - rect.left - 2 * offsetx, rect.bottom - rect.top - 2 * offsety) / gsize;
			}

			if ((HWND)lParam == hwndButton1)
			{
				if( my.newgame(gsize, true, win) )draw(hWnd, my);
				else MessageBox(hWnd, L"Unable to create game", NULL, MB_OK);
			}
			else if ((HWND)lParam == hwndButton2)
			{
				if (my.newgame(gsize, false, win))draw(hWnd, my);
				else MessageBox(hWnd, L"Unable to create game", NULL, MB_OK);
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