#include"game.h"

using namespace std;

int main()
{
	int a, b;
	game my;
	my.newgame(5, true);
	while (cin >> a >> b)
	{
		my.move(my.p_id, a, b);
		cout << "Tah1" << endl;
		my.aimove(my.c_id);
		cout << "Tah2" << endl;
		my.vypis();
	}
	return 0;
}