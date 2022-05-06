#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "RenderWindow.h"


static double pCirX = 0;
static double oCirX = 0;

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