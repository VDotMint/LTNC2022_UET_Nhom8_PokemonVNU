#include "BattleScreen.h"

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