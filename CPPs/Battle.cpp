#include "Battle.h"

void battle (Pokemon my[],Pokemon op[]) {
	int i=0,j=0;
	while (i<2&&j<2) {
		if(battle(my[i],op[j])) {
			j++;
		}
		else i++;
	}
	if (i==3) cout<<"You lost!\n";
	else cout<<"You won!\n";
}

bool battle(Pokemon &my,Pokemon &op) {
	bool isKO = false;
	// int turn = 0;
	int input;
	// cout << "Turn: " << turn << '\n';
	updateTerminal(my,op);
	while (true) {
		// turn++;
		// cout<<"Turn: "<<turn<<'\n';
		cout<<setw(62)<<setfill('-')<<'\n';
		input=selectMove(my);
		if (input==-1) continue;
		isKO=useMove(input,my,op);
		updateTerminal(my,op);
		if (isKO) {
			cout<<"Opposing "<<op.name<<" fainted!\n";
			return true;
		}
		cout<<"Opposing ";
		isKO=useMove(rand()%2,op,my);
		updateTerminal(my,op);
		if (isKO) {
			cout<<my.name<<" fainted!\n";
			return false;
		}
	}
};

void updateTerminal(Pokemon &my, Pokemon &op) {
	cout<<setw(12)<<setfill(' ')<<left<<my.name<<setw(50)<<setfill(' ')<<right<<op.name<<'\n';
	cout<<setw(12)<<setfill(' ')<<left<<my.hp<<setw(50)<<setfill(' ')<<right<<op.hp<<'\n';
};

int selectMove(Pokemon &my) {
	cout<<"Choose your move\n";
	for (int i=0;i<2;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my.move[i]->name<<" power: "<<my.move[i]->power<<" Type: "<<Type[my.move[i]->type]<<'\n';
	}
	int input;
	cin>>input;
	if (input>=2||input<0) {
			cout<<"invalid move\n";
			return -1;
	}
	return input;
};

bool useMove(int input, Pokemon &my, Pokemon &op) {
	cout<<my.name<<" used "<<my.move[input]->name<<'\n';
	op.hp-=my.move[input]->power*my.atk/op.def/2;
	if (op.hp<0) op.hp=0;
	if (!op.hp) return true;
	return false;
}
