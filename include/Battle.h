#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "mPlayer.h"
#include "Pokemon.h"

using namespace std;

class BattleScreenButtons {
	private:

	public:

};

class BattleScreen {
	private:
		SDL_Texture* battleBackground;
		SDL_Texture* battleCircle;
		SDL_Texture* grayInputBox;
		SDL_Rect playerCircle, opponentCircle, grayBoxRect;
	public:
		bool scrollingPlayer;
		BattleScreen();
		~BattleScreen();
		void initBattleScreen(mPlayer* player, Trainer* opponent);
		void freeBattleScreen();
		void drawBattleScreen(bool fMtB, bool fBtM);
};

void battle (Pokemon my[],Pokemon op[]);

bool battle(Pokemon &my,Pokemon &op);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);

extern string Type[];