#include"game.h"

using namespace std;

int main()
{
	int a, b;
	game my;
	my.newgame(5, true, 3);
	my.aimove(my.c_id);
	while (cin >> a >> b)
	{
		my.move(my.p_id, a, b);
		cout << "Tah1" << endl;
		if (my.is_over())cout << "Konieeec" << endl;
		my.aimove(my.c_id);
		cout << "Tah2" << endl;
		if (my.is_over())cout << "Konieeec" << endl;
		my.vypis();
		if (my.is_over())break;
	}
	cout << my.status() << endl;
	return 0;
}