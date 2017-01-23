#pragma once
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class game
{
private:
	vector<vector<int> >board;
	int board_size;
	bool is_over;
public:
	game();

	void newgame(int size, bool f_move);
};

void game::newgame(int size, bool f_move)
{
	board.resize(size, vector<int>(size));
	for (int i = 0; i < size; ++i)fill(board[i].begin(), board[i].end(), 0);
	board_size = size;
	is_over = false;
}

game::game()
{
	board = vector<vector<int> >(0);
}