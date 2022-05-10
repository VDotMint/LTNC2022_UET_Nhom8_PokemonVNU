#include "RenderWindow.h"
#include "Battle.h"

void battle(Pokemon my[]) {
	srand(time(0));
	Pokemon op[3]={rand()%psize,rand()%psize,rand()%psize};
	battle(my,op);
}

void battle (Pokemon my[],Pokemon op[]) {
	int i=0,j=0;
	int pkm_c=3;
	while (j<3) {
		if(battle(my[i],op[j])) {
			j++;
		}
		else {
			pkm_c--;
			if (pkm_c==0) {
				cout<<"You lost!\n";
				return;
			}
			i=selectPokemon(my);
		}
	}
	if (pkm_c==0) cout<<"You lost!\n";
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
		cout<<'-'<<setw(61)<<setfill('-')<<'-'<<endl;
		input=selectMove(my);
		if (input==-1) continue;
		if (my.data->speed>=op.data->speed) {
			isKO=useMove(input,my,op);
			updateTerminal(my,op);
			if (isKO) {
				cout<<"Opposing "<<op.data->name<<" fainted!\n";
				return true;
			}
			cout<<"Opposing ";
			isKO=useMove(rand()%2,op,my);
			updateTerminal(my,op);
			if (isKO) {
				cout<<my.data->name<<" fainted!\n";
				return false;
			}
		}
		else {
			cout<<"Opposing ";
			isKO=useMove(rand()%2,op,my);
			updateTerminal(my,op);
			if (isKO) {
				cout<<my.data->name<<" fainted!\n";
				return false;
			}
			isKO=useMove(input,my,op);
			updateTerminal(my,op);
			if (isKO) {
				cout<<"Opposing "<<op.data->name<<" fainted!\n";
				return true;
			}
		}
	}
}

void updateTerminal(Pokemon &my, Pokemon &op) {
	cout<<setw(12)<<setfill(' ')<<left<<my.data->name<<setw(50)<<setfill(' ')<<right<<op.data->name<<'\n';
	cout<<my.c_hp<<'/'<<setw(9)<<setfill(' ')<<left<<my.data->hp<<setw(47)<<setfill(' ')<<right<<op.c_hp<<'/'<<op.data->hp<<'\n';
};

int selectMove(Pokemon &my) {
	cout<<"Choose your move\n";
	for (int i=0;i<4;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my.data->move[i]->name<<" power: "<<my.data->move[i]->power<<" Type: "<<setw(10)<<Type[my.data->move[i]->type]<<"PP: "<<my.c_pp[i]<<'\n';
	}
	int input;
	cin>>input;
	if (input>=4||input<0) {
			cout<<"invalid move\n";
			return -1;
	}
	if (!my.c_pp[input]) {
		cout<<"Out of PP\n";
		return -1;
	}
	return input;
};

bool useMove(int input, Pokemon &my, Pokemon &op) {
	cout<<my.data->name<<" used "<<my.data->move[input]->name<<'\n';
	op.c_hp-=my.data->move[input]->power*my.data->atk/op.data->def/2;
	my.c_pp[input]--;
	if (op.c_hp<0) op.c_hp=0;
	
	if (!op.c_hp) return true;
	return false;
}

void printParty(Pokemon my[]) {
	for (int i=0;i<4;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my[i].data->name<<my[i].c_hp<<'/'<<setw(9)<<setfill(' ')<<my[i].data->hp<<'\n';
	}
}

int selectPokemon(Pokemon my[]) {
	cout<<"Choose your pokemon\n";
	printParty(my);
	int input;
	while (true) {
		cin>>input;
		if (my[input].c_hp==0) {
			cout<<"invalid\n";
		}
		else return input;
	}
};
