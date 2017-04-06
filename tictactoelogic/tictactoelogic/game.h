#ifndef GAME_H_
#define GAME_H_

#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include<ctime>

using namespace std;

namespace myclass
{

	class game
	{
		/*  This class represents game. There is no function for drawing this game in this class.
			This is only logical part of game. */

	private:

		vector<vector<int> >board;

		int board_size, to_win, moves;
		int p_id, c_id;
		//board_size denotes vertical/horizontal size of board
		//to_win denotes number of consecutive positions which you have to have to win
		//moves is number of moves which have been made

		bool over;
		//is set to true if game is over and no more moves can be done

		const static int dx[8];
		const static int dy[8];

		bool is_winning(int row, int column);
		bool is_inrange(int row, int column);
		//function is_winning returns true if position [row][column] is in winning chain
		//function is_inrange returns true if position [row][column] is on our board

		int most(int row, int column);
		int longest_row(int row, int column, int direction);

		void bfs(vector<vector<int> >&pg);

	public:

		int player_id();
		int computer_id();

		bool is_createable(int b_size, int to_win);
		//returns true if game with this parameters is possible to create
		bool new_game(int size, bool first_move, int tw);
		//creates game with this parameters and returns true, false otherwise

		int size();
		//returns size of current board
		bool is_over();
		//returns true if game is over, false if it's not

		bool make_move(int id, int row, int column);
		pair<int, int> make_ai_move(int id);

		int owner(int row, int column);
		//returns the owner of field on position[row][column]

		int game_status();
		//returns -1 if game is not ower, returns 0 if game is ower but nobody wins or id of winner otherwise
		void print();
	};

}
#endif