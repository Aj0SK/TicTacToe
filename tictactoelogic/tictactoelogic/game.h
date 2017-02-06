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

	void bfs(vector<vector<int> >&pg);
	
	bool inrange(int a, int b);
	int longestrow(int a, int b, int k);
	bool iswinning(int a, int b);
	int most(int a, int b);

public:

	int p_id, c_id;

	bool newgame(int size, bool f_move, int tw);
	
	int size();
	bool is_over();

	bool move(int id, int a, int b);
	pair<int, int> aimove(int id);

	int owner(int a, int b);

	int status();
	void vypis();
};

#endif