#include <iostream>
#include <cmath>

#include "BattleScreen.h"
#include "RenderWindow.h"
#include "Variables.h"

// BATTLE RENDERING

static SDL_Rect halfPlayerPokeRect = {0, 0, 96, 96};
static SDL_Rect halfOppoPokeRect = {96, 0, 96, 96};

static int currentPlayerFrame = 0;
static unsigned int BattleSen = 0;

static double pCirX = 0, oCirX = 0, playerPokemonMoveAnim = 0, opponentPokemonMoveAnim = 0, PokemonTransparency = 0;

BattleScreen::BattleScreen() {
}

BattleScreen::~BattleScreen() {
	freeBattleScreen();
}

void BattleScreen::initBattleScreen(mPlayer* player, Trainer* opponent) {
    battlePlayer = player;
    battleOpponent = opponent;

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
    battleDialogues.clear();
    std::string sen1 = "You are challenged by " + opponent->name + "!";
    battleDialogues.push_back(sen1);
    std::string sen2 = opponent->name + " sent out " + opponent->party[0].data->name + "!";
    battleDialogues.push_back(sen2);
    std::string sen3 = player->getPlayerName() + " sent out " + player->party[0].data->name + "!";
    battleDialogues.push_back(sen3);

    // STARTING POKEMON TEXTURE
    for (int i = 0; i < 3; i++) {
        std::string imgPlayerPath = "res/pokemonassets/" + battlePlayer->party[i].data->name + ".png";
        playerPokeText[i] = IMG_LoadTexture(RenderWindow::renderer, imgPlayerPath.c_str());
        SDL_SetTextureBlendMode(playerPokeText[i], SDL_BLENDMODE_BLEND);

        std::string imgOppoPath = "res/pokemonassets/" + battleOpponent->party[i].data->name + ".png";
        oppoPokeText[i] = IMG_LoadTexture(RenderWindow::renderer, imgOppoPath.c_str());
        SDL_SetTextureBlendMode(oppoPokeText[i], SDL_BLENDMODE_BLEND);
    }

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
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(playerPokeText[i]);
        SDL_DestroyTexture(oppoPokeText[i]);
        playerPokeText[i] = NULL;
        oppoPokeText[i] = NULL;
    }
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
    turnActionQueue.clear();

	pCirX = 0;
	oCirX = 0;
    playerPokemonMoveAnim = 0;
    opponentPokemonMoveAnim = 0;

    inAnim0 = true;
    showPHPBar = false, showOHPBar = false;
    fightScreen = false, moveScreen = false;
    startingBattle = true;

    BattleSen = 0;
    currentPlayerFrame = 0;
}

void BattleScreen::drawBattleScreen(bool fMtB, bool fBtM) {
    // RENDER THE BATTLE BACKGROUND
	SDL_RenderCopy(RenderWindow::renderer, battleBackground, NULL, NULL);

    // ANIMATING THE PLAYER AND OPPONENT SPRITES
	if (fMtB == false) {

        // I WAS BRAINDEAD WHILE CODING THIS PARTICULAR ANIMATION :(
        // HINH DUNG CAI PHUONG TRINH BAC 2 O DUOI LA 1 CAI PHEP ANH XA, OCIRX LA X
        // X CHAY TU 0 DEN 100. CAI PHUONG TRINH BAC 2 DAY NHU LA PHUONG TRINH CHUYEN DONG CUA MAY CAI SPRITE TREN MAN HINH THOI
        
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

    // RENDER THE 2 SIDE'S POKEMONS
    SDL_RenderCopy(RenderWindow::renderer, oppoPokeText[0], &halfOppoPokeRect, &oppoPokeRect);
    SDL_RenderCopy(RenderWindow::renderer, playerPokeText[0], &halfPlayerPokeRect, &playerPokeRect);

    // RENDER THE GRAY BOX CONTAINING THE DIALOGUE AND THE TEXT
	if (grayBoxRect.y > 500) grayBoxRect.y -= 4;
	SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);

    // the first battle sentence
    if (fMtB == false and BattleSen == 0 and startingBattle == true) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[BattleSen].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
    }

    // opponent sending out the pokemons
    if (BattleSen == 1 and startingBattle == true) {
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
    if (BattleSen == 2 and startingBattle == true) {
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

    // SHOW THE OPPONENT'S HP BAR
    if (showOHPBar == true) {
        SDL_RenderCopy(RenderWindow::renderer, oppoHPBar, NULL, &oppoHPRect);
        SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currOppoHP);
        oppoPokeName.textInit(RenderWindow::renderer, (battleOpponent->party[0].data->name).c_str(), {0, 0, 0});
        oppoPokeName.display(4, 83, RenderWindow::renderer);
    }

    // SHOW THE PLAYER'S HP BAR
    if (showPHPBar == true) {
        SDL_RenderCopy(RenderWindow::renderer, playerHPBar, NULL, &playerHPRect);
        SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currPlayHP);
        playerPokeName.textInit(RenderWindow::renderer, (battlePlayer->party[0].data->name).c_str(), {0, 0, 0});
        playerPokeName.display(585, 407, RenderWindow::renderer);
    }

    // DISPLAY THE FIGHT SCREEN
    if (fightScreen == true) {
        fightButton.drawButton();
        pokemonButton.drawButton();
        retireButton.drawButton();
    }

    // DISPLAY THE MOVE SCREEN
    if (moveScreen == true) {
        for (int i = 0; i < 4; i++) moveButtons[i].drawButton();
        backButton.drawButton();
    }

    // HANDLING MOVE ANIMATION AND FLAGS
    if (startingBattle == false and fightScreen == false and moveScreen == false) {
        bd_Text.textInit(RenderWindow::renderer, battleDialogues[BattleSen].c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);

        if (turnActionQueue[BattleSen] == "PLAYER_USE_MOVE") // ANIMATE THE PLAYER USING A MOVE
        {
            if (playerPokemonMoveAnim < 21 and inAnim0 == true) {
                playerPokeRect.x = -(playerPokemonMoveAnim*playerPokemonMoveAnim) + 20*playerPokemonMoveAnim + 90;
                playerPokemonMoveAnim++;
            } else if (playerPokemonMoveAnim == 21 and inAnim0 == true) {
                if (PokemonTransparency < 1170 and inAnim0 == true) {
                    SDL_SetTextureAlphaMod(oppoPokeText[0], int(255*(abs(sin(PokemonTransparency*3.141592/180.0)))));
                    PokemonTransparency += 130;
                } else if (PokemonTransparency >= 1170 and inAnim0 == true) {
                    SDL_SetTextureAlphaMod(oppoPokeText[0], int(255*(abs(sin(PokemonTransparency*3.141592/180.0)))));
                    playerPokemonMoveAnim = 0;
                    PokemonTransparency = 0;
                    inAnim0 = false;
                }
            }
        }
        else if (turnActionQueue[BattleSen] == "OPPONENT_USE_MOVE") // ANIMATE THE OPPONENT USING A MOVE
        {
            if (opponentPokemonMoveAnim < 21 and inAnim0 == true) {
                oppoPokeRect.x = (opponentPokemonMoveAnim*opponentPokemonMoveAnim) - 20*opponentPokemonMoveAnim + 500;
                opponentPokemonMoveAnim++;
            } else if (opponentPokemonMoveAnim == 21 and inAnim0 == true) {
                if (PokemonTransparency < 1170 and inAnim0 == true) {
                    SDL_SetTextureAlphaMod(playerPokeText[0], int(255*(abs(sin(PokemonTransparency*3.141592/180.0)))));
                    PokemonTransparency += 130;
                } else if (PokemonTransparency >= 1170 and inAnim0 == true) {
                    SDL_SetTextureAlphaMod(playerPokeText[0], int(255*(abs(sin(PokemonTransparency*3.141592/180.0)))));
                    opponentPokemonMoveAnim = 0;
                    PokemonTransparency = 0;
                    inAnim0 = false;
                }
            }
        }
        else
        {
            inAnim0 = false;
        }
    }
}

void BattleScreen::centralBattleProcess(SDL_Event* e) {
    // HANDLING WHEN THE X BUTTON IS PRESSED (USED MOSTLY FOR BATTLE DIALOGUES)
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_x && inAnim0 == false && fightScreen == false && moveScreen == false) {
        if (startingBattle == true) {
            if (BattleSen < 3) {
                inAnim0 = true;
                BattleSen++;
                if (BattleSen == 3) {
                    fightScreen = true;
                    startingBattle = false;
                    BattleSen = 0;
                    battleDialogues.clear();
                    turnActionQueue.clear();
                }
            }
        } else {
            inAnim0 = true;
            BattleSen++;
            if (BattleSen == battleDialogues.size()) {
                fightScreen = true;
                BattleSen = 0;
                battleDialogues.clear();
                turnActionQueue.clear();
            }
        }
    }

    // HANDLING THE SELECT MOVE SCREEN
    if (moveScreen == true) {
        backButton.buttonHandler();
        if (backButton.clickedOn == true) {
            backButton.clickedOn = false;
            fightScreen = true;
            moveScreen = false;
        }

        moveButtons[0].buttonHandler();
        if (moveButtons[0].clickedOn == true) {
            moveButtons[0].clickedOn = false;
            localTurnHandler(0);
            moveScreen = false;
        }

        moveButtons[1].buttonHandler();
        if (moveButtons[1].clickedOn == true) {
            moveButtons[1].clickedOn = false;
            localTurnHandler(1);
            moveScreen = false;
        }

        moveButtons[2].buttonHandler();
        if (moveButtons[2].clickedOn == true) {
            moveButtons[2].clickedOn = false;
            localTurnHandler(2);
            moveScreen = false;
        }

        moveButtons[3].buttonHandler();
        if (moveButtons[3].clickedOn == true) {
            moveButtons[3].clickedOn = false;
            localTurnHandler(3);
            moveScreen = false;
        }
    }

    // HANDLING THE FIGHT SCREEN
    if (fightScreen == true) {
        fightButton.buttonHandler();
        pokemonButton.buttonHandler();
        retireButton.buttonHandler();
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
}

void BattleScreen::localTurnHandler(int move) {
    bool isKO;
    if (battlePlayer->party[0].data->speed >= battleOpponent->party[0].data->speed) {
        isKO = useMove(move, battlePlayer->party[0], battleOpponent->party[0],false);
        if (isKO) {
            std::string newSentence = "The opposing " + battleOpponent->party[0].data->name + " fainted!";
            battleDialogues.push_back(newSentence);
            turnActionQueue.push_back("OPPONENT_FAINT");
        }
        if (!isKO) {
            isKO = useMove(0, battleOpponent->party[0], battlePlayer->party[0],true);
            if (isKO) {
                std::string newSentence = battlePlayer->party[0].data->name + " fainted!";
                battleDialogues.push_back(newSentence);
                turnActionQueue.push_back("PLAYER_FAINT");
            }
        }
    } else {
        isKO = useMove(0, battleOpponent->party[0], battlePlayer->party[0],true);
        if (isKO) {
            std::string newSentence = battlePlayer->party[0].data->name + " fainted!";
            battleDialogues.push_back(newSentence);
            turnActionQueue.push_back("PLAYER_FAINT");
        }
        if (!isKO) {
            isKO = useMove(move, battlePlayer->party[0], battleOpponent->party[0],false);
                battleDialogues.push_back(newSentence);
            }
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

void BattleScreenButton::buttonHandler() {
    // 0: INSIDE, 1: HOVER, 2: CLICK
    if (e.type == SDL_MOUSEMOTION or e.type == SDL_MOUSEBUTTONDOWN or e.type == SDL_MOUSEBUTTONUP) {
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
            switch (e.type) {
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
void BattleScreenButton::contextButtonHandler(SDL_Event* e) {
    
}
