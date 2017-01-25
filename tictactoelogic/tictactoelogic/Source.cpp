#include"game.h"

using namespace std;

int main()
{
	int a, b;
	game my;
	my.newgame(500, true, 3);
	my.move(my.p_id, 0, 3);
	my.move(my.p_id, 2, 0);
	my.move(my.p_id, 1, 1);	
	my.move(my.p_id, 8, 8);
	my.aimove(my.c_id);
		
	return 0;
}