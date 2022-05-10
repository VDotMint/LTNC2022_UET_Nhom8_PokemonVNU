#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include "otherGraphics.h"
#include "RenderWindow.h"
#include "mPlayer.h"

class BattleScreenButton {
	private:
		SDL_Texture* buttonTexture;
		SDL_Rect buttonFrames[3];
		SDL_Rect buttonDest;
		int currentButtonFrame;
		Text moveNames;
	public:
		bool clickedOn;
		BattleScreenButton();
		~BattleScreenButton();
		void initBSB(const char* path, int x, int y, int BH, int BW, int imgWidth, int imgHeight);
		void drawButton();
        void buttonHandler(SDL_Event* e);
		void moveButtonHandler(SDL_Event* e);
};

class BattleScreen {
	private:
		mPlayer* battlePlayer;
		Trainer* battleOpponent;

		SDL_Texture* battleBackground;
		SDL_Texture* battleCircle;
		SDL_Texture* grayInputBox;

		SDL_Texture* playerTexture;
		SDL_Texture* opponentTexture;

		SDL_Texture* playerHPBar;
		SDL_Texture* oppoHPBar;
		SDL_Rect playerHPRect, oppoHPRect;
		Text playerPokeName, oppoPokeName;

		SDL_Texture* HPColor;
		SDL_Rect currPlayHP, currOppoHP;

		SDL_Texture* playerPokeText;
		SDL_Texture* oppoPokeText;
		SDL_Rect playerPokeRect, oppoPokeRect;

		SDL_Rect playerTextureFrames[4];
		SDL_Rect playerCircle, opponentCircle, grayBoxRect, playerSpriteBox, opponentSpriteBox;

		BattleScreenButton fightButton, pokemonButton, retireButton, backButton;
		BattleScreenButton moveButtons[4];
		
		Text bd_Text;
	public:
		vector<std::string> battleDialogues;
		bool inAnim0 = true, fightScreen = false, moveScreen = false, showPHPBar = false, showOHPBar = false;
		int pressedMoveButton = -1;
		BattleScreen();
		~BattleScreen();
		void initBattleScreen(mPlayer* player, Trainer* opponent);
		void freeBattleScreen();
		void drawBattleScreen(bool fMtB, bool fBtM);
		void centralBattleProcess(SDL_Event* e);
};