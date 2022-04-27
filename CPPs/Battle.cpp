#include "Battle.h"

void battle(Pokemon my,Pokemon op) {
    using namespace std;

	bool battleState = true;
	int turn = 0;
	int input;
	cout << "Turn: " << turn << '\n';
	cout << setw(12) << setfill(' ') << left << my.name << setw(50) << setfill(' ') << right << op.name << '\n';
	cout << setw(12) << setfill(' ') << left << my.hp << setw(50) << setfill(' ') << right << op.hp << '\n';
	while (battleState) {
		turn++;
		cout<<"Turn: "<<turn<<'\n';
		cout<<"Choose your move\n";
		for (int i=0;i<2;i++) {
			cout<<i<<". "<<my.move[i]->name<<" power: "<<my.move[i]->power<<'\n';
		}
		cin>>input;
		if (input>=2||input<0) {
			cout<<"invalid move\n";
			continue;
		}
		else {
			op.hp-=my.move[input]->power*my.atk/op.def/2;
			if (op.hp<0) op.hp=0;
		}
		cout<<my.name<<" used "<<moves[input].name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.name<<setw(50)<<setfill(' ')<<right<<op.name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.hp<<setw(50)<<setfill(' ')<<right<<op.hp<<'\n';
		if (!op.hp) {
			cout<<"You win\n";
			break;
		}
	}
};