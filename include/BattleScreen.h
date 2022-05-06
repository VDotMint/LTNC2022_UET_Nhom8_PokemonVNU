#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "RenderWindow.h"
#include "mPlayer.h"

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