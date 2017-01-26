#pragma once
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
	
	void bfs(vector<vector<int> >&pg);

	int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

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
	int most(int a, int b);

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

int game::most(int a, int b)
{
	int maxi = -1;
	for (int i = 0; i < 4;++i)maxi=max(maxi, longestrow(a, b, i) + longestrow(a, b, i+4) - 1);
	return maxi;
}

int game::longestrow(int a, int b, int k)
{
	int kolko = 0;
	//	[i-1;j-1]	[i-1;j]	[i-1; j+1]
	//	[i;j-1]		[i;j]		[i;j+1]
	//	[i+1;j-1]	[i+1;j]		[i+1j;j+1]
	//int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	//int dy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

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
bool game::is_over()
{
	return over;
}

/*pair<int, int> game::aimove(int id)
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
}*/


pair<int, int> game::aimove(int id)
{
	vector<int>oko, mozne;
	pair<int, int>mymove; 
	int a, b, guess, maxx = -1;
	
	vector<vector<int> >dist(board_size, vector<int>(board_size, -1));
	if(moves!=0)bfs(dist);

	/*cout << "Hracia plocha: " << endl;
	for (int i = 0; i < board_size; ++i){for (int j = 0; j < board_size; ++j){cout << dist[i][j] << " ";}
	cout << endl;
	}*/

	for (int i = 0; i < board_size; ++i)for (int j = 0; j < board_size; ++j)if(board[i][j] == 0 && dist[i][j] <= 1)
	{
		mozne.push_back(i*board_size + j);
	}

	//int guess = rand() % mozne.size();
	
	for (int i = 0; i < mozne.size(); ++i)
	{
		a = mozne[i] / board_size;
		b = mozne[i] % board_size;
		board[a][b] = id;
		
		if (most(a, b) > maxx)oko.clear(), oko.push_back(mozne[i]), maxx = most(a, b), guess = i;
		else if (most(a, b) == maxx)oko.push_back(mozne[i]);
		board[a][b] = 0;
	}
	//cout << random() << endl;
	guess = rand() % oko.size();
	//cout << oko.size() << endl;
	//cout << oko[guess] / board_size << " " << oko[guess] % board_size << endl;
	move(id, oko[guess] / board_size, oko[guess] % board_size);
	return{ oko[guess]/board_size, oko[guess]%board_size};

	return{ -1, -1 };
}

void game::bfs(vector<vector<int> >&pg)
{
	int a, b, na, nb;
	queue<int>Q;
	for (int i = 0; i < board_size; ++i)
		for (int j = 0; j < board_size; ++j)if(board[i][j] != 0)
	{
			pg[i][j] = 0;
			Q.push(i);
			Q.push(j);
	}
	
	while (!Q.empty())
	{
		a = Q.front(); Q.pop();
		b = Q.front(); Q.pop();
		for (int i = 0; i < 8; ++i)
		{
			na = a + dx[i];
			nb = b + dy[i];
			if (inrange(na, nb) && pg[na][nb]==-1)
			{
				Q.push(na); Q.push(nb);
				pg[na][nb] = pg[a][b] + 1;
			}
		}
	}
}