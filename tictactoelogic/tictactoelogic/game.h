#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<ctime>

using namespace std;

class game
{
private:
	vector<vector<int> >board;
	int board_size, moves, towin;
	bool over;
public:
	int p_id, c_id;

	void newgame(int size, bool f_move, int tw);

	bool move(int id, int a, int b);
	pair<int, int> aimove(int id);

	bool is_over();
	int size();

	int owner(int a, int b);

	bool inrange(int a, int b);
	int longestrow(int a, int b, int k);
	bool iswinning(int a, int b);

	int status();
	void vypis();
};

int game::owner(int a, int b)
{
	if(inrange(a, b))return board[a][b];
	else return -1;
}

int game::size()
{
	return board_size;
}


int game::status()
{
	if (!over)return -1;
	else
	{
		for (int i = 0; i < board_size; ++i)for (int j = 0; j < board_size; ++j)if (iswinning(i, j))
		{
			return board[i][j];
		}
	}
	return 0;
}

bool game::iswinning(int a, int b)
{
	if (longestrow(a, b, 0) + longestrow(a, b, 4) - 1 >= towin)return true;
	if (longestrow(a, b, 1) + longestrow(a, b, 5) - 1 >= towin)return true;
	if (longestrow(a, b, 2) + longestrow(a, b, 6) - 1 >= towin)return true;
	if (longestrow(a, b, 3) + longestrow(a, b, 7) - 1 >= towin)return true;
	return false;
}

int game::longestrow(int a, int b, int k)
{
	int kolko = 0;
	//	[i-1;j-1]	[i-1;j]	[i-1; j+1]
	//	[i;j-1]		[i;j]		[i;j+1]
	//	[i+1;j-1]	[i+1;j]		[i+1j;j+1]
	int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int dy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	if (board[a][b] == 0)return 0;

	for (int i = 0;kolko<towin; ++i)
	{
		if (inrange(a + i*dx[k], b + i*dy[k]) && board[a][b] == board[a + i*dx[k]][b + i*dy[k]])kolko++;
		else break;
	}

	return kolko;
}

bool game::inrange(int a, int b)
{
	if (a >= 0 && b >= 0 && a < board_size && b < board_size)return true;
	return false;
}

void game::vypis()
{
	cout << "Hracia plocha: " << endl;
	for (int i = 0; i < board_size; ++i)
	{
		for (int j = 0; j < board_size; ++j)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	
}

void game::newgame(int size, bool f_move, int tw)
{
	if (size < 3)return;
	board_size = size;
	towin = tw;

	over = false;
	moves = 0;

	board.resize(size, vector<int>(size));
	for (int i = 0; i < size; ++i)fill(board[i].begin(), board[i].end(), 0);
	
	p_id = 1, c_id = 2;
	
	if (!f_move)
	{
		swap(p_id, c_id);
		aimove(c_id);
	}
}

bool game::move(int id, int a, int b)
{
	if (!over && inrange(a, b) && board[a][b] == 0)
	{
		board[a][b] = id;
		moves++;
		if (iswinning(a, b) || moves == board_size*board_size)over = true;
		return true;
	}
	cout << "Invalid Move" << endl;
	return false;
}

pair<int, int> game::aimove(int id)
{
	pair<int, int>mymove;
	int guess, pom=0;
	srand(time(NULL));
	guess = rand() % (board_size*board_size-moves);
	for(int i=0;i<board_size;++i)for (int j = 0; j < board_size; ++j)if(board[i][j] == 0)
		{
			if (pom == guess && move(id, i, j))
			{
				mymove = { i, j };
				return mymove;
			}
			pom++;
		}
	return{ -1, -1 };
}

bool game::is_over()
{
	return over;
}