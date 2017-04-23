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

	srand(time(nullptr));
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
my_namespace::game my_game;
int game_size = 5;
int needed_to_win = 3;
int field_size = 50;
const int offsetx = 10, offsety = 90;
//default settings for game
//game size represents size of the board, needed_to_win is minimal length of chain which is needed to win
//field_size*field_size is size of one field
/////////////////////////////////////////////////////////

//Function which drawes actual game
void draw_game(HWND &hWnd, my_namespace::game &toshow)
{
	InvalidateRect(hWnd, 0, true);

	//sets graphic properties
	PAINTSTRUCT  ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	// if you hate my colors :P, you can change it here :
	Pen game_pen  (Color(255, 255, 0, 0));
	Pen	circle_pen(Color(255, 0, 0, 0));
	Pen cross_pen (Color(255, 34, 139, 34));
	
	for (int i = 0; i <= toshow.size(); ++i)
	{
		graphics.DrawLine(&game_pen, offsetx + i*field_size, offsety, offsetx + i*field_size, offsety + field_size*toshow.size());//draw vertical lines
		graphics.DrawLine(&game_pen, offsetx, offsety + i*field_size, offsetx + field_size * toshow.size(), offsety + i*field_size);//draws horizontal lines
	}

	for (int i = 0; i < toshow.size(); ++i)
	{
		for (int j = 0; j < toshow.size(); ++j)
		{
			if (toshow.owner(i, j) == 1 )//we have to draw cross
			{
				graphics.DrawLine(&cross_pen, offsetx+j*field_size, offsety+i*field_size, offsetx+(j + 1)*field_size, offsety+(i + 1)*field_size);
				graphics.DrawLine(&cross_pen, offsetx+(j + 1)*field_size, offsety+i*field_size, offsetx+j*field_size, offsety+(i + 1)*field_size);
			}
			if (toshow.owner(i, j) == 2)//we have to draw circle
			{
				graphics.DrawEllipse(&circle_pen, offsetx+j*field_size, offsety+i*field_size, field_size, field_size );
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
		int row, column;
		int y_coordinate = HIWORD(lParam);
		int x_coordinate = LOWORD(lParam);

		if (y_coordinate >= offsety && x_coordinate >= offsetx && y_coordinate < offsety+my_game.size()*field_size && x_coordinate < offsetx + my_game.size()*field_size )
		{
			row = (((y_coordinate - offsety) / field_size));
			column = (((x_coordinate - offsetx) / field_size));

			if (my_game.make_move(my_game.player_id(), row, column) && !my_game.is_over())
			{
				pair<int, int>computer_move = my_game.make_ai_move();

				if (computer_move.first !=  -1)
				{
					row = computer_move.first;
					column = computer_move.second;
				}
			}
		}

		draw_game(hWnd, my_game);
		if (my_game.is_over())
		{
			if (my_game.game_status() == my_game.player_id())MessageBox(hWnd, L"Win", NULL, MB_OK);
			else if (my_game.game_status() == my_game.computer_id())MessageBox(hWnd, L"Loose", NULL, MB_OK);
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

			if ( my_game.is_createable(value1, value2) )
			{
				game_size = value1;
				needed_to_win = value2;
				field_size = min(rect.right - rect.left - 2 * offsetx, rect.bottom - rect.top - 2 * offsety) / game_size;
			}

			if ((HWND)lParam == hwndButton1)
			{
				if( my_game.new_game(game_size, true, needed_to_win) )draw_game(hWnd, my_game);
				else MessageBox(hWnd, L"Unable to create game", NULL, MB_OK);
			}
			else if ((HWND)lParam == hwndButton2)
			{
				if (my_game.new_game(game_size, false, needed_to_win))draw_game(hWnd, my_game);
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