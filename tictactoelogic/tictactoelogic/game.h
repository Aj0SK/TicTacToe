#ifndef GAME_H_
#define GAME_H_

#include<iostream>
#include<vector>
#include<queue>
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

	int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	
	bool is_winning(int row, int column);
	bool is_inrange(int row, int column);
	
	int most(int row, int column);
	int longestrow(int row, int column, int direction);

	void bfs(vector<vector<int> >&pg);

public:

	int p_id, c_id;

	bool is_createable(int b_size, int to_win);
	bool newgame(int size, bool first_move, int tw);
	
	int size();
	bool is_over();

	bool move(int id, int row, int column);
	pair<int, int> aimove(int id);
	
	int owner(int row, int column);
	
	int status();
	void vypis();
};

#endif