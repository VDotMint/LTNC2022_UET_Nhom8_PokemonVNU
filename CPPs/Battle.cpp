#include "RenderWindow.h"
#include "Battle.h"

void battle (Pokemon my[],Pokemon op[]) {
	int i=0,j=0;
	while (i<2&&j<2) {
		if(battle(my[i],op[j])) {
			j++;
		}
		else i++;
	}
	if (i==2) cout<<"You lost!\n";
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
		cout<<i<<". "<<setw(12)<<setfill(' ')<<left<<my.data->move[i]->name<<" power: "<<my.data->move[i]->power<<" Type: "<<setw(10)<<Type[my.data->move[i]->type]<<"PP: "<<my.c_pp[i]<<'\n';
	}
	int input;
	cin>>input;
	if (input>=2||input<0) {
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

// BATTLE RENDERING

static double pCirX = 0;
static double oCirX = 0;

BattleScreen::BattleScreen() {

}

BattleScreen::~BattleScreen() {
	freeBattleScreen();
}

void BattleScreen::initBattleScreen() {
	battleBackground = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlebackground0.png");
	battleCircle = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlecircle0.png");
	playerCircle = {832, 400, 320, 100};
	opponentCircle = {-320, 175, 320, 100};
}

void BattleScreen::freeBattleScreen() {
	SDL_DestroyTexture(battleBackground);
	SDL_DestroyTexture(battleCircle);
	battleBackground = NULL;
	battleCircle = NULL;
	pCirX = 0;
	oCirX = 0;
	playerCircle = {832, 400, 320, 100};
	opponentCircle = {-320, 175, 320, 100};
}

void BattleScreen::drawBattleScreen(bool fMtB, bool fBtM) {
	SDL_RenderCopy(RenderWindow::renderer, battleBackground, NULL, NULL);
	if (fMtB == false) {
		if (-0.1276*pCirX*pCirX+20.58*pCirX-320 < 462) {
			playerCircle.x = int(0.1244*pCirX*pCirX-20.26*pCirX+832);
			pCirX++;
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &playerCircle);
		if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
			opponentCircle.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-320);
			oCirX++;
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &opponentCircle);
	}
}