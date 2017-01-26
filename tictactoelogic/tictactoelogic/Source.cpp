#include"game.h"

using namespace std;

int main()
{
	srand(time(NULL));
	int a, b;
	game my;
	for (int i = 0; i < 500;++i)cout<<i<<endl, my.newgame(5, false, 3);
	
		
	return 0;
}