#ifndef GAME_H_
#define GAME_H_

#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include<ctime>

using namespace std;

namespace my_namespace
{

	class game
	{
		/*  This class represents game. There is no function for drawing this game in this class.
			This is only logical part of game. */

	private:

		vector<vector<int> >board;

		int board_size, needed_to_win, moves;
		int p_id, c_id;
		//board_size denotes vertical/horizontal size of board
		//to_win denotes number of consecutive positions which you have to have to win
		//moves is number of moves which have been made

		//p_id and c_id is player and computer id, used on board do denote who owns which position

		bool over;
		//is set to true if game is over and no more moves can be done

		const static int dx[8];
		const static int dy[8];

		bool is_winning(int row, int column);
		//is_winning returns true if position [row][column] is in winning chain//return true if field [row][column] is winning in any direction
		bool is_inrange(int row, int column);
		//is_inrange returns true if indexes represents valid position on our board, false otherwise

		int longest_chain_in_direction(int row, int column, int direction);
		//returns size of the longest chain which passes throught field [row][column] in direction "direction"
		int longest_chain(int row, int column);
		//returns size of the longest chain which passes throught field [row][column] in any direction

		void distances_from_active(vector<vector<int> >&pg);
		//for every field, finds his distance to the nearest active field(to the field where is cross or circle)

	public:

		int player_id();
		int computer_id();

		bool is_createable(int b_size, int to_win);
		//returns true if game with this parameters is possible to create and valid
		
		bool new_game(int size, bool first_move, int tw);
		//creates game with this parameters and returns true, false otherwise
		//this function creates new game with board_size "size" and to_win equal to "tw"	

		int size();
		//returns size of current board
		bool is_over();
		//returns true if game is over, false if it's not

		bool make_move(int id, int row, int column);
		pair<int, int> make_ai_move();

		int owner(int row, int column);
		//returns the owner of field on position[row][column]

		int game_status();
		//returns -1 if game is not over, returns 0 if game is over but nobody wins or id of winner otherwise
		void print();
		//method used for visualisation of board in early version and for testing purpose in console
		
	};

}
#endif