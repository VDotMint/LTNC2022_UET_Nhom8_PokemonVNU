#include "Battle.h"

Pokemon::Pokemon(int i) {
	data=pokemonData+i;
	c_hp=data->hp;
	for (int i=0;i<2;i++) {
		c_pp[i]=data->move[i]->pp;
	}
}

Pokemon::Pokemon() {
	data=NULL;
};

BattleScreen::BattleScreen() {

}

BattleScreen::~BattleScreen() {
	
}

void BattleScreen::initBattleScreen() {

}

void BattleScreen::freeBattleScreen() {
	SDL_DestroyTexture(battleBackground);
}

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
		cout<<setw(63)<<setfill('-')<<'\n';
		input=selectMove(my);
		if (input==-1) continue;
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
};

void updateTerminal(Pokemon &my, Pokemon &op) {
	cout<<setw(12)<<setfill(' ')<<left<<my.data->name<<setw(50)<<setfill(' ')<<right<<op.data->name<<'\n';
	cout<<my.c_hp<<'/'<<setw(9)<<setfill(' ')<<left<<my.data->hp<<setw(47)<<setfill(' ')<<right<<op.c_hp<<'/'<<op.data->hp<<'\n';
};

int selectMove(Pokemon &my) {
	cout<<"Choose your move\n";
	for (int i=0;i<2;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my.data->move[i]->name<<" power: "<<my.data->move[i]->power<<" Type: "<<Type[my.data->move[i]->type]<<'\n';
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
	cout<<my.data->name<<" used "<<my.data->move[input]->name<<'\n';
	op.c_hp-=my.data->move[input]->power*my.data->atk/op.data->def/2;
	if (op.c_hp<0) op.c_hp=0;
	if (!op.c_hp) return true;
	return false;
}
