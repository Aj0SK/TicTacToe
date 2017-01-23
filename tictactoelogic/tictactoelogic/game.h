#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<time.h>

using namespace std;

class game
{
private:
	vector<vector<int> >board;
	int board_size, moves;
	bool over;
public:
	int p_id, c_id;
	game();

	void newgame(int size, bool f_move);

	bool move(int id, int a, int b);
	void aimove(int id);
	bool is_over();

	bool inrange(int a, int b);
	void vypis();
};

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

void game::newgame(int size, bool f_move)
{
	board_size = size;
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
	if (inrange(a, b) && board[a][b] == 0)
	{
		board[a][b] = id;
		moves++;
		return true;
	}
	cout << "Invalid Move" << endl;
	return false;
}

void game::aimove(int id)
{
	int guess, pom=0;
	srand(time(NULL));
	guess = rand() % (board_size*board_size-moves);
	for(int i=0;i<board_size;++i)for (int j = 0; j < board_size; ++j)if(board[i][j] == 0)
		{
			if (pom == guess)
			{
				move(id, i, j);
				i += 1000;
				break;
			}
			pom++;
		}
}

bool game::is_over()
{
	return over;
}

game::game()
{
	board = vector<vector<int> >(0);
}