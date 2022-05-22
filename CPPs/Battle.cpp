#include "Variables.h"
#include "RenderWindow.h"
#include "Battle.h"

/* DEPRECATED TERMINAL BATTLE FUNCTION

void battle(mPlayer &my,Trainer &op) {
	if (my.activePokemonCount==0) return;
	int i=0,j=0;
	while (!my.party[i].c_hp) i++;
	int KO;
	mainBattle.init(&my,&op);
	while (true) {
		KO=battle(my.party[i],op.party[j]);
		if (KO==-2) return;
		if (KO==-1) return;
		if (KO) {
			j++;
			if (j==3) {
				mainBattle.printText("You won!\n");
				return;
			}
			mainBattle.printText(op.name+" sent out "+op.party[j].data->name);
		}
		else {
			my.activePokemonCount--;
			if (!my.activePokemonCount) {
				// cout<<"You lost!\n";
				mainBattle.printText("You lost!\n");
				return;
			}
			i=selectPokemon(my.party);
			mainBattle.printText(my.getPlayerName()+" sent out "+my.party[i].data->name);
		}
	}
};

int battle(Pokemon &my,Pokemon &op) {
	bool isKO = false;
	// int turn = 0;
	int input;
	// cout << "Turn: " << turn << '\n';
	updateTerminal(my,op);
	mainBattle.updateScreen(my,op);
	while (true) {
		// turn++;
		// cout<<"Turn: "<<turn<<'\n';
		if (quit) return -2;
		cout<<'-'<<setw(61)<<setfill('-')<<'-'<<endl;
		printMoves(my);
		input=mainBattle.menuInput(my);
		if (input==-1) return -1;
		if (input==-2) return -2;
		if (my.data->speed>=op.data->speed) {
			isKO=useMove(input,my,op);
			updateTerminal(my,op);
			if (isKO) {
				// cout<<"Opposing "<<op.data->name<<" fainted!\n";
				mainBattle.printText("Opposing "+op.data->name+" fainted!\n");
				return true;
			}
			// cout<<"Opposing ";
			mainBattle.buffer="Opposing ";
			isKO=useMove(rand()%2,op,my);
			mainBattle.buffer="";
			updateTerminal(my,op);
			if (isKO) {
				// cout<<my.data->name<<" fainted!\n";
				mainBattle.printText(my.data->name+" fainted!\n");
				return false;
			}
		}
		else {
			mainBattle.buffer="Opposing ";
			isKO=useMove(rand()%2,op,my);
			mainBattle.buffer="";
			updateTerminal(my,op);
			if (isKO) {
				// cout<<my.data->name<<" fainted!\n";
				mainBattle.printText(my.data->name+" fainted!\n");
				return false;
			}
			isKO=useMove(input,my,op);
			updateTerminal(my,op);
			if (isKO) {
				// cout<<"Opposing "<<op.data->name<<" fainted!\n";
				mainBattle.printText("Opposing "+op.data->name+" fainted!\n");
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
	printMoves(my);
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
	//cout<<my.data->name<<" used "<<my.data->move[input]->name<<'\n';
	mainBattle.printText(my.data->name+" used "+my.data->move[input]->name+'\n');
	float STAB=1;
	if (my.data->move[input]->type==my.data->type||my.data->move[input]->type==my.data->stype) STAB=1.5;
	float TE=typeEffectiveness[my.data->move[input]->type][op.data->type]*typeEffectiveness[my.data->move[input]->type][op.data->stype];
	op.c_hp-=my.data->move[input]->power*my.data->atk/op.data->def/2*STAB*TE;
	my.c_pp[input]--;
	if (op.c_hp<0) op.c_hp=0;
	if (TE==0) mainBattle.printText("It has no effect");
	else if (TE<1) mainBattle.printText("It is not very effcetive");
	else if (TE>1) mainBattle.printText("It is very effective");
	if (!op.c_hp) return true;
	return false;
}

*/

bool useMove(int input, Pokemon &my, Pokemon &op, bool isOpponent) {
	std::string newBattleSentence;
	if (isOpponent == true) newBattleSentence += "Opposing ";
	newBattleSentence += my.data->name + " used " + (my.data->move[input])->name + "!";
	mainBattle.battleDialogues.push_back(newBattleSentence);

	if (isOpponent) mainBattle.turnActionQueue.push_back("OPPONENT_USE_MOVE");
	else mainBattle.turnActionQueue.push_back("PLAYER_USE_MOVE");

	float STAB=1;
	if (my.data->move[input]->type==my.data->type||my.data->move[input]->type==my.data->stype) STAB=1.5;
	float TE=typeEffectiveness[my.data->move[input]->type][op.data->type]*typeEffectiveness[my.data->move[input]->type][op.data->stype];

	srand(time(NULL));

	op.c_hp-=(my.data->move[input]->power*my.data->atk/op.data->def/2*STAB*TE)*(rand() % 15 + 86)/100;
	my.c_pp[input]--;
	if (op.c_hp<0) op.c_hp=0;
	
	if (TE==0) {
		if (!isOpponent) {
			mainBattle.battleDialogues.push_back("It didn't affect the opposing " + op.data->name + "...");
			mainBattle.turnActionQueue.push_back("MOVE_NOEFFECT");
		} else {
			mainBattle.battleDialogues.push_back("It didn't affect " + op.data->name + "...");
			mainBattle.turnActionQueue.push_back("MOVE_NOEFFECT");
		} 
	}

	else if (TE<1) {
		mainBattle.battleDialogues.push_back("It's not very effective...");
		mainBattle.turnActionQueue.push_back("MOVE_NOT_EFFECTIVE");	
	} 

	else if (TE>1) {
		mainBattle.battleDialogues.push_back("It's super effective!");
		mainBattle.turnActionQueue.push_back("MOVE_SUPER_EFFECTIVE");
	}

	if (!op.c_hp) return true;
	return false;
}

/* DEPRECATED TERMINAL BATTLE FUNCTION

void printParty(Pokemon my[]) {
	for (int i=0;i<3;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my[i].data->name<<my[i].c_hp<<'/'<<setw(9)<<setfill(' ')<<my[i].data->hp<<'\n';
	}
}

void printMoves(Pokemon &my) {
	cout<<"Choose your move\n";
	for (int i=0;i<4;i++) {
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my.data->move[i]->name<<" power: "<<my.data->move[i]->power<<" Type: "<<setw(10)<<Type[my.data->move[i]->type]<<"PP: "<<my.c_pp[i]<<'\n';
	}
};

int selectPokemon(Pokemon my[]) {
	cout<<"Choose your pokemon\n";
	printParty(my);
	int input;
	while (true) {
		cin>>input;
		if (my[input].c_hp==0||input>2) {
			cout<<"invalid\n";
		}
		else return input;
	}
};

*/