#include "Battle.h"

void battle(Pokemon my,Pokemon op) {
    using namespace std;

	bool battleState = true;
	int turn = 0;
	int input;
	cout << "Turn: " << turn << '\n';
	cout << setw(12) << setfill(' ') << left << my.name << setw(50) << setfill(' ') << right << op.name << '\n';
	cout << setw(12) << setfill(' ') << left << my.health << setw(50) << setfill(' ') << right << op.health << '\n';
	while (battleState) {
		turn++;
		cout<<"Turn: "<<turn<<'\n';
		cout<<"Choose your move\n";
		for (int i=0;i<2;i++) {
			cout<<i<<": "<<moves[i].name<<'\n';
		}
		cin>>input;
		if (input>1) {
			cout<<"invalid move\n";
			continue;
		}
		else {
			op.health-=moves[input].damage;
			if (op.health<0) op.health=0;
		}
		cout<<my.name<<" used "<<moves[input].name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.name<<setw(50)<<setfill(' ')<<right<<op.name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.health<<setw(50)<<setfill(' ')<<right<<op.health<<'\n';
		if (!op.health) {
			cout<<"You win\n";
			break;
		}
	}
};