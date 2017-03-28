#include "game.h"

using namespace std;

int game::size()//returns size of current board
{
	return board_size;
}

bool game::is_over()//returns over, boolean variable which denotes if game is over
{
	return over;
}

bool game::is_creatable(int b_size, int t_win)//return true if these settings(parameters) lead to a valid game
{
	return (b_size > 0 && t_win > 0 && b_size >= t_win);
}

bool game::inrange(int row, int column)//returns true if indexes represents valid position, false otherwise
{
	if (row >= 0 && column >= 0 && row < board_size && column < board_size)return true;
	return false;
}

int game::owner(int row, int column)//returns id of owner of field on position [row][column], -1 in case of invalid index
{
	if (inrange(row, column))return board[row][column];
	return -1;
}

int game::status()//return -1 in case game is not ended, returns id of player that wins or 0 in case game is over but there is no winner(draw)
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

int game::longestrow(int row, int column, int direction)//returns size of longest sequence which contains field [row][column] in direction "direction"
{
	//	[i-1;j-1]	[i-1;j]	[i-1; j+1]
	//	[i;j-1]		[i;j]		[i;j+1]
	//	[i+1;j-1]	[i+1;j]		[i+1j;j+1]
	//int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	//int dy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	if (!inrange(row, column) || board[row][column] == 0)return 0;

	int result = 0;

	for (int i = 0; result<towin; ++i)
	{
		if (inrange(row + i*dx[direction], column + i*dy[direction]) && board[row][column] == board[row + i*dx[direction]][column + i*dy[direction]])result++;
		else break;
	}

	return result;
}

int game::most(int row, int column)//returns longest sequenci which contains [row][column]
{
	int maxi = -1;
	if (!inrange(row, column))return maxi;
	for (int i = 0; i < 4; ++i)maxi = max(maxi, longestrow(row, column, i) + longestrow(row, column, i + 4) - 1);
	return maxi;
}

bool game::iswinning(int row, int column)//return true if field [row][column] is in winning succession in any direction
{
	if (most(row, column) >= towin)return true;
	return false;
}

void game::vypis()//method used for visualisation of board in early version and for testing purpose
{
	cout << "Board: " << endl;
	for (int i = 0; i < board_size; ++i)
	{
		for (int j = 0; j < board_size; ++j)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

bool game::newgame(int size, bool f_move, int tw)
{
	if (size < 1 || size > 1000 || tw > size || tw < 1)return false;
	board_size = size;
	towin = tw;

	over = false;
	moves = 0;

	board.clear();
	board.resize(size, vector<int>(size));
	for (int i = 0; i < size; ++i)fill(board[i].begin(), board[i].end(), 0);

	p_id = 1, c_id = 2;

	if (!f_move)
	{
		swap(p_id, c_id);
		aimove(c_id);
	}
	return true;
}

bool game::move(int id, int row, int column)
{
	if (!over && inrange(row, column) && board[row][column] == 0)
	{
		board[row][column] = id;
		moves++;
		if (iswinning(row, column) || moves == board_size*board_size)over = true;
		return true;
	}
	cout << "Invalid Move" << endl;
	return false;
}

pair<int, int> game::aimove(int id)
{
	vector<int>oko, mozne;
	pair<int, int>mymove;
	int a, b, guess, maxx = -1;

	vector<vector<int> >dist(board_size, vector<int>(board_size, -1));
	if (moves != 0)bfs(dist);

	for (int i = 0; i < board_size; ++i)for (int j = 0; j < board_size; ++j)if (board[i][j] == 0 && dist[i][j] <= 1)
	{
		mozne.push_back(i*board_size + j);
	}

	for (int i = 0; i < mozne.size(); ++i)
	{
		a = mozne[i] / board_size;
		b = mozne[i] % board_size;
		board[a][b] = id;

		if (most(a, b) > maxx)oko.clear(), oko.push_back(mozne[i]), maxx = most(a, b), guess = i;
		else if (most(a, b) == maxx)oko.push_back(mozne[i]);
		board[a][b] = 0;
	}

	guess = rand() % oko.size();
	move(id, oko[guess] / board_size, oko[guess] % board_size);
	return{ oko[guess] / board_size, oko[guess] % board_size };
}

void game::bfs(vector<vector<int> >&pg)
{
	int a, b, na, nb;
	queue<int>Q;
	for (int i = 0; i < board_size; ++i) for (int j = 0; j < board_size; ++j) if (board[i][j] != 0)
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
			if (inrange(na, nb) && pg[na][nb] == -1)
			{
				Q.push(na); 
				Q.push(nb);
				pg[na][nb] = pg[a][b] + 1;
			}
		}
	}
}