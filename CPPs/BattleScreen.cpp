#include "BattleScreen.h"
#include "RenderWindow.h"

// BATTLE RENDERING

static double pCirX = 0;
static double oCirX = 0;

BattleScreen::BattleScreen() {
}

BattleScreen::~BattleScreen() {
	freeBattleScreen();
}

void BattleScreen::initBattleScreen(mPlayer* player, Trainer* opponent) {
    bd_Text.createFont("res/font/gamefont.ttf", 42);

	battleBackground = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlebackground0.png");
	battleCircle = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlecircle0.png");
	grayInputBox = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/graybox.png");
	playerCircle = {832, 450, 320, 100};
	opponentCircle = {-320, 200, 320, 100};
	grayBoxRect = {0, 704, 832, 204};
    playerSpriteBox = {832, 260, 240, 240};
    opponentSpriteBox = {-320, 60, 240, 240};

    for (int i = 0; i < 4; i++) playerTextureFrames[i] = {i*80, 0, 80, 80};
    
    if (player->getGender() == 0) {
        playerTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/malefrontsprite.png");
    } else if (player->getGender() == 1) {
        playerTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/femalefrontsprite.png");
    }
    opponentTexture = IMG_LoadTexture(RenderWindow::renderer, opponent->battleSpritePath.c_str());

    std::string sen1 = "You are challenged by " + opponent->name;
    battleDialogues.push_back(sen1);

    std::string sen2 = opponent->name + " sent out Garchomp";
    battleDialogues.push_back(sen2);
}

void BattleScreen::freeBattleScreen() {
    bd_Text.~Text();

	SDL_DestroyTexture(battleBackground);
	SDL_DestroyTexture(battleCircle);
	SDL_DestroyTexture(grayInputBox);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(opponentTexture);

	battleBackground = NULL;
	battleCircle = NULL;
	grayInputBox = NULL;
    playerTexture = NULL;
    opponentTexture = NULL;

    battleDialogues.clear();

	pCirX = 0;
	oCirX = 0;

	playerCircle = {832, 450, 320, 100};
	opponentCircle = {-320, 200, 320, 100};
	grayBoxRect = {0, 704, 832, 204};
    playerSpriteBox = {872, 260, 240, 240};
    opponentSpriteBox = {-320, 60, 240, 240};

    pScroll1 = true;
    pScroll2 = false;
}

void BattleScreen::drawBattleScreen(bool fMtB, bool fBtM) {
	SDL_RenderCopy(RenderWindow::renderer, battleBackground, NULL, NULL);
	if (fMtB == false) {
        if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
            opponentSpriteBox.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-340);
			opponentCircle.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-320);
			oCirX++;
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &opponentCircle);      
		if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
			playerCircle.x = int(0.1244*pCirX*pCirX-20.26*pCirX+832);
            playerSpriteBox.x = int(0.1244*pCirX*pCirX-20.26*pCirX+862);
            pCirX++;
            if (-0.1276*oCirX*oCirX+20.58*oCirX-320 > 430) {
                pScroll1 = false;
            }
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &playerCircle);
        SDL_RenderCopy(RenderWindow::renderer, playerTexture, &playerTextureFrames[0], &playerSpriteBox);  
	}

	if (grayBoxRect.y > 500) grayBoxRect.y -= 4;
	SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);

    if (pScroll1 == false) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[0].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
    }
}

void BattleScreen::centralBattleProcess(SDL_Event* e) {
    
}

// BATTLE SCREEN BUTTONS

BattleScreenButton::BattleScreenButton() {
    buttonTexture = NULL;
    clickedOn = false;
    currentButtonFrame = 0;
}

BattleScreenButton::~BattleScreenButton() {
    SDL_DestroyTexture(buttonTexture);
    buttonTexture = NULL;
    clickedOn = false;
}

void BattleScreenButton::initBSB(const char* path, int BW, int BH) {
    buttonTexture = IMG_LoadTexture(RenderWindow::renderer, path);
    for (int i = 0; i < 3; i++) {
        buttonFrames[i] = {0, BH*i, BW, BH};
    }
    buttonWidth = BW;
    buttonHeight = BH;
}

void BattleScreenButton::setButtonDest(int x, int y) {
    buttonDest = {x, y, buttonWidth, buttonHeight};
}

void BattleScreenButton::drawButton() {
    SDL_RenderCopy(RenderWindow::renderer, buttonTexture, &buttonFrames[currentButtonFrame], &buttonDest);
}

void BattleScreenButton::buttonHandler(SDL_Event* e) {
    // 0: INSIDE, 1: HOVER, 2: CLICK
    if (e->type == SDL_MOUSEMOTION or e->type == SDL_MOUSEBUTTONDOWN or e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;
        if (x < buttonDest.x) inside = false;
        else if (x > buttonDest.x + buttonWidth) inside = false;
        else if (y < buttonDest.y) inside = false;
        else if (y > buttonDest.y + buttonHeight) inside = false;

        if (inside == false) {
            currentButtonFrame = 0;
        } else {
            switch (e->type) {
                case SDL_MOUSEMOTION:
                currentButtonFrame = 1;
                break;

                case SDL_MOUSEBUTTONDOWN:
                currentButtonFrame = 2;
                break;

                case SDL_MOUSEBUTTONUP:
                currentButtonFrame = 1;
                clickedOn = true;
                break;

                default:
                break;
            }
        }
    }
}