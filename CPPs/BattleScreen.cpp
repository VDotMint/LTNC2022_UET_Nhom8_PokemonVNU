#include <iostream>
#include "BattleScreen.h"
#include "RenderWindow.h"
#include "Variables.h"

// BATTLE RENDERING

static double BattleSen = 0;

static double pCirX = 0;
static double oCirX = 0;

static int currentPlayerFrame = 0;

BattleScreen::BattleScreen() {
}

BattleScreen::~BattleScreen() {
	freeBattleScreen();
}

void BattleScreen::initBattleScreen(mPlayer* player, Trainer* opponent) {
    battPlayer = player;

    bd_Text.createFont("res/font/gamefont.ttf", 42);
    playerPokeName.createFont("res/font/gamefont.ttf", 28);
    oppoPokeName.createFont("res/font/gamefont.ttf", 28);

    // LOAD THE MAIN BATTLE ASSETS
	battleBackground = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlebackground0.png");
	battleCircle = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/battlecircle0.png");
	grayInputBox = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/graybox.png");
	playerCircle = {832, 450, 320, 100};
	opponentCircle = {-320, 200, 320, 100};
	grayBoxRect = {0, 704, 832, 204};

    // HP BAR TEXTURES;
    playerHPBar = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/pHPBar.png");
    oppoHPBar = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/oHPBar.png");
    HPColor = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/HPColor.png");
    playerHPRect = {548, 388, 284, 95};
    oppoHPRect = {0, 66, 273, 78};
    currPlayHP = {708, 443, 106, 7};
    currOppoHP = {112, 119, 107, 7};

    // CHARACTER SPRITE BOXES
    playerSpriteBox = {832, 260, 240, 240};
    opponentSpriteBox = {-320, 20, 240, 240};

    // POKEMON SPRITE BOXES
    playerPokeRect = {-240, 300, 240, 240};
    oppoPokeRect = {832, 20, 240, 240};

    // PLAYER SPRITE HANDLING
    for (int i = 0; i < 4; i++) playerTextureFrames[i] = {i*80, 0, 80, 80};
    if (player->getGender() == 0) playerTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/malefrontsprite.png");
    else if (player->getGender() == 1) playerTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/femalefrontsprite.png");

    // OPPONENT SPRITE HANDLING
    opponentTexture = IMG_LoadTexture(RenderWindow::renderer, opponent->battleSpritePath.c_str());

    // BATTLE DIALOGUE (FIRST 3 SENTENCES ONLY)
    std::string sen1 = "You are challenged by " + opponent->name + "!";
    battleDialogues.push_back(sen1);
    std::string sen2 = opponent->name + " sent out Garchomp!";
    battleDialogues.push_back(sen2);
    std::string sen3 = player->getPlayerName() + " sent out " + player->party[0].data->name + "!";
    battleDialogues.push_back(sen3);

    // STARTING POKEMON TEXTURE
    playerPokeText = IMG_LoadTexture(RenderWindow::renderer, "res/pokemonassets/charizardback.png");
    oppoPokeText = IMG_LoadTexture(RenderWindow::renderer, "res/pokemonassets/garchompfront.png");

    // INIT THE FIGHT SCREEN BUTTONS
    fightButton.initBSB("res/battleassets/fightbutton.png", 230, 527, 371, 150, 556, 224);
    pokemonButton.initBSB("res/battleassets/pokemonbutton.png", 23, 559, 188, 86, 282, 130);
    retireButton.initBSB("res/battleassets/retirebutton.png", 621, 559, 188, 86, 282, 130);

    // INIT THE MOVE SCREEN BUTTONS
    moveButtons[0].initBSB("res/battleassets/moveButton.png", 33, 519, 278, 72, 652, 167);
    moveButtons[1].initBSB("res/battleassets/moveButton.png", 344, 519, 278, 72, 652, 167);
    moveButtons[2].initBSB("res/battleassets/moveButton.png", 33, 611, 278, 72, 652, 167);
    moveButtons[3].initBSB("res/battleassets/moveButton.png", 344, 611, 278, 72, 652, 167);
    backButton.initBSB("res/battleassets/backbutton.png", 659, 620, 154, 60, 362, 139);
}

void BattleScreen::freeBattleScreen() {
    // MAIN BATTLE TEXTURES
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

    playerCircle = {832, 450, 320, 100};
	opponentCircle = {-320, 200, 320, 100};
	grayBoxRect = {0, 704, 832, 204};
    playerSpriteBox = {872, 260, 240, 240};
    opponentSpriteBox = {-320, 20, 240, 240};

    // ON SCREEN POKEMANS
    SDL_DestroyTexture(playerPokeText);
    SDL_DestroyTexture(oppoPokeText);
    playerPokeText = NULL;
    oppoPokeText = NULL;
    playerPokeRect = {-240, 300, 240, 240};
    oppoPokeRect = {832, 20, 240, 240};

    // HP Bars
    playerHPRect = {548, 388, 284, 95};
    oppoHPRect = {0, 66, 273, 78};
    currPlayHP = {708, 443, 106, 7};
    currOppoHP = {112, 119, 107, 7};

    SDL_DestroyTexture(playerHPBar);
    SDL_DestroyTexture(oppoHPBar);
    SDL_DestroyTexture(HPColor);
    playerHPBar = NULL;
    oppoHPBar = NULL;
    HPColor = NULL;

    // Free the buttons
    fightButton.~BattleScreenButton();
    pokemonButton.~BattleScreenButton();
    retireButton.~BattleScreenButton();

    for (int i = 0; i < 4; i++) moveButtons[i].~BattleScreenButton();
    backButton.~BattleScreenButton();

    battleDialogues.clear();

	pCirX = 0;
	oCirX = 0;

    inAnim0 = true;
    showPHPBar = false, showOHPBar = false;
    BattleSen = 0;
    fightScreen = false, moveScreen = false;
    currentPlayerFrame = 0;
}

void BattleScreen::drawBattleScreen(bool fMtB, bool fBtM) {
	SDL_RenderCopy(RenderWindow::renderer, battleBackground, NULL, NULL);
	if (fMtB == false) {
        if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
            opponentSpriteBox.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-300);
			opponentCircle.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-320);
			oCirX++;
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &opponentCircle);
        SDL_RenderCopy(RenderWindow::renderer, opponentTexture, NULL, &opponentSpriteBox);            
		if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
			playerCircle.x = int(0.1244*pCirX*pCirX-20.26*pCirX+832);
            playerSpriteBox.x = int(0.1244*pCirX*pCirX-20.26*pCirX+862);
            pCirX++;
            if (-0.1276*oCirX*oCirX+20.58*oCirX-320 > 430) {
                inAnim0 = false;
            }
		}
		SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &playerCircle);
        SDL_RenderCopy(RenderWindow::renderer, playerTexture, &playerTextureFrames[currentPlayerFrame], &playerSpriteBox);  
	}

    SDL_RenderCopy(RenderWindow::renderer, oppoPokeText, NULL, &oppoPokeRect);
    SDL_RenderCopy(RenderWindow::renderer, playerPokeText, NULL, &playerPokeRect);

	if (grayBoxRect.y > 500) grayBoxRect.y -= 4;
	SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);

    // the first battle sentence
    if (fMtB == false and BattleSen == 0) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[BattleSen].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
    }

    // opponent sending out the pokemons
    if (BattleSen == 1) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[BattleSen].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
        if (opponentSpriteBox.x < 860) {
            opponentSpriteBox.x += 8;
        }

        if (opponentSpriteBox.x > 850) {
            if (oppoPokeRect.x > 500) {
                oppoPokeRect.x -= 8;
                if (oppoPokeRect.x < 510) {
                    inAnim0 = false;
                    showOHPBar = true;
                }
            }
        }
    }

    // player sending out the pokemons
    if (BattleSen == 2) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[BattleSen].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
        if (playerSpriteBox.x > -240) {
            playerSpriteBox.x -= 8;
            
            if (playerSpriteBox.x == 70) currentPlayerFrame++;
            if (playerSpriteBox.x == 22) currentPlayerFrame++;
            if (playerSpriteBox.x == -26) currentPlayerFrame++;
        }

        if (playerSpriteBox.x < -230) {
            if (playerPokeRect.x < 90) {
                playerPokeRect.x += 8;
                if (playerPokeRect.x > 80) {
                    inAnim0 = false;
                    showPHPBar = true;
                }
            }
        }
    }

    if (showOHPBar == true) {
        SDL_RenderCopy(RenderWindow::renderer, oppoHPBar, NULL, &oppoHPRect);
        SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currOppoHP);
        oppoPokeName.textInit(RenderWindow::renderer, "Garchomp", {0, 0, 0});
        oppoPokeName.display(4, 83, RenderWindow::renderer);
    }

    if (showPHPBar == true) {
        SDL_RenderCopy(RenderWindow::renderer, playerHPBar, NULL, &playerHPRect);
        SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currPlayHP);
        playerPokeName.textInit(RenderWindow::renderer, (battPlayer->party[0].data->name).c_str(), {0, 0, 0});
        playerPokeName.display(585, 407, RenderWindow::renderer);
    }

    if (fightScreen == true) {
        fightButton.drawButton();
        pokemonButton.drawButton();
        retireButton.drawButton();
    }

    if (moveScreen == true) {
        for (int i = 0; i < 4; i++) moveButtons[i].drawButton();
        backButton.drawButton();
    }
}

void BattleScreen::centralBattleProcess(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_x && inAnim0 == false) {
        if (BattleSen < battleDialogues.size()) {
            inAnim0 = true;
            BattleSen++;
            if (BattleSen == 3) {
                inAnim0 = false;
                fightScreen = true;
            }
        } else {
            fightScreen = true;
        }
    }

    if (fightScreen == true) {
        fightButton.buttonHandler(e);
        pokemonButton.buttonHandler(e);
        retireButton.buttonHandler(e);
        if (fightButton.clickedOn == true) {
            moveScreen = true;
            fightScreen = false;
            fightButton.clickedOn = false;
        } else if (pokemonButton.clickedOn == true) {
            pokemonButton.clickedOn = false;
        } else if (retireButton.clickedOn == true) {
            retireButton.clickedOn = false;
            beginBattleToMapTransition = true;
        }
    }

    if (moveScreen == true) {
        backButton.buttonHandler(e);
        if (backButton.clickedOn == true) {
            backButton.clickedOn = false;
            fightScreen = true;
            moveScreen = false;
        }
    }
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

void BattleScreenButton::initBSB(const char* path, int x, int y, int BW, int BH, int imgWidth, int imgHeight) {
    buttonTexture = IMG_LoadTexture(RenderWindow::renderer, path);
    for (int i = 0; i < 3; i++) {
        buttonFrames[i] = {0, imgHeight*i, imgWidth, imgHeight};
    }
    buttonDest = {x, y, BW, BH};
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
        else if (x > buttonDest.x + buttonDest.w) inside = false;
        else if (y < buttonDest.y) inside = false;
        else if (y > buttonDest.y + buttonDest.h) inside = false;

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