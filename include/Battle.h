#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Pokemon.h"

using namespace std;
class BattleScreen {
	private:
		SDL_Texture* battleBackground;
		SDL_Texture* battleCircle;
		SDL_Rect playerCircle;
		SDL_Rect opponentCircle;
	public:
		BattleScreen();
		~BattleScreen();
		void initBattleScreen();
		void freeBattleScreen();
		void drawBattleScreen(bool fMtB, bool fBtM);
};

void battle (Pokemon my[],Pokemon op[]);

bool battle(Pokemon &my,Pokemon &op);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);

extern string Type[];