#include <iostream>
#include <cmath>

#include "BattleScreen.h"
#include "RenderWindow.h"
#include "Variables.h"

static SDL_Rect halfPlayerPokeRect = {0, 0, 96, 96};
static SDL_Rect halfOppoPokeRect = {96, 0, 96, 96};

static int currentPlayerFrame = 0;
static unsigned int BattleSen = 0;

static double pCirX = 0, oCirX = 0, playerPokemonMoveAnim = 0, opponentPokemonMoveAnim = 0, PokemonTransparency = 0;

void BattleScreen::updateScreen(Pokemon &my, Pokemon &opponent) {
    SDL_RenderCopy(RenderWindow::renderer, battleBackground, NULL, NULL);
    SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &opponentCircle);
    SDL_RenderCopy(RenderWindow::renderer, opponentTexture, NULL, &opponentSpriteBox);
    SDL_RenderCopy(RenderWindow::renderer, battleCircle, NULL, &playerCircle);
    SDL_RenderCopy(RenderWindow::renderer, playerTexture, &playerTextureFrames[currentPlayerFrame], &playerSpriteBox);
    SDL_RenderCopy(RenderWindow::renderer, oppoPokeText[0], &halfOppoPokeRect, &oppoPokeRect);
    SDL_RenderCopy(RenderWindow::renderer, playerPokeText[0], &halfPlayerPokeRect, &playerPokeRect);
    SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);
    SDL_RenderCopy(RenderWindow::renderer, oppoHPBar, NULL, &oppoHPRect);
    SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currOppoHP);
    oppoPokeName.textInit(RenderWindow::renderer, (opponent.data->name).c_str(), {0, 0, 0});
    oppoPokeName.display(4, 83, RenderWindow::renderer);
    SDL_RenderCopy(RenderWindow::renderer, playerHPBar, NULL, &playerHPRect);
    SDL_RenderCopy(RenderWindow::renderer, HPColor, NULL, &currPlayHP);
    playerPokeName.textInit(RenderWindow::renderer, (my.data->name).c_str(), {0, 0, 0});
    playerPokeName.display(585, 407, RenderWindow::renderer);
    renderWindow.display();
};
void BattleScreen::init(mPlayer* player, Trainer* opponent) {
    initBattleScreen(player,opponent);
    while ((-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462)||(grayBoxRect.y > 500)) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return;
            }
        }
        if (grayBoxRect.y > 500) grayBoxRect.y -= 4;
        if (-0.1276*oCirX*oCirX+20.58*oCirX-320 < 462) {
            opponentSpriteBox.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-300);
		    opponentCircle.x = int(-0.1276*oCirX*oCirX+20.58*oCirX-320);
		    oCirX++;
		    playerCircle.x = int(0.1244*pCirX*pCirX-20.26*pCirX+832);
            playerSpriteBox.x = int(0.1244*pCirX*pCirX-20.26*pCirX+862);
            pCirX++;
        }
        if (-0.1276*oCirX*oCirX+20.58*oCirX-320 > 430) {
            inAnim0 = false;
        }
        updateScreen(player->party[0],opponent->party[0]);
        gameMusic.play();
        SDL_Delay(1000/60);
    }
    printText(opponent->name+" sent out "+opponent->party[0].data->name);
    oSentPkm(player->party[0],opponent->party[0]);
    printText(player->getPlayerName()+" sent out "+player->party[0].data->name);
    pSentPkm(player->party[0],opponent->party[0]);
};

int BattleScreen::menuInput(Pokemon &my) {
    while (true) {
        int move;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return -2;
            }
            fightButton.buttonHandler();
            pokemonButton.buttonHandler();
            retireButton.buttonHandler();
        }
        SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);
        fightButton.drawButton();
        pokemonButton.drawButton();
        retireButton.drawButton();
        if (fightButton.clickedOn == true) {
            fightButton.clickedOn = false;
            move=moveInput(my);
            if (move==-1) continue;
            else return move;
        } else if (pokemonButton.clickedOn == true) {
            pokemonButton.clickedOn = false;
        } else if (retireButton.clickedOn == true) {
            retireButton.clickedOn = false;
            return -1;
        }
        renderWindow.display();
        gameMusic.play();
        SDL_Delay(1000/60);
    }
};

int BattleScreen::moveInput(Pokemon &my) {
    while (true) {
        SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return -2;
            }
            backButton.buttonHandler();
            for (int i = 0; i < 4; i++) {
                moveButtons[i].buttonHandler();
            }
        }      
        for (int i = 0; i < 4; i++) moveButtons[i].drawButton();
        backButton.drawButton();
        if (backButton.clickedOn == true) {
            backButton.clickedOn = false;
            return -1;
        }
        for (int i = 0; i < 4; i++) {
            if (moveButtons[i].clickedOn) {
                moveButtons[i].clickedOn=false;
                return i;
            }
        }
        renderWindow.display();
        gameMusic.play();
        SDL_Delay(1000/60);
    }
};

void BattleScreen::printText(string s) {
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return ;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                return;
            }
        }
        SDL_RenderCopy(RenderWindow::renderer, grayInputBox, NULL, &grayBoxRect);
        bd_Text.textInit(RenderWindow::renderer, (buffer+s).c_str(), {255, 255, 255}, 800);
        bd_Text.display(32, 530, RenderWindow::renderer);
        renderWindow.display();
        gameMusic.play();
        SDL_Delay(1000/60);
    }
}; 

void BattleScreen::oSentPkm(Pokemon &my,Pokemon &op) {
    while ((opponentSpriteBox.x < 860)||oppoPokeRect.x >500) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return ;
            }
        }
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
        updateScreen(my,op);
        gameMusic.play();
        SDL_Delay(1000/60);
    }    
};

void BattleScreen::pSentPkm(Pokemon &my,Pokemon &op) {
    while ((playerSpriteBox.x > -240)||(playerPokeRect.x < 90)) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                return ;
            }
        }
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
        updateScreen(my,op);
        gameMusic.play();
        SDL_Delay(1000/60);
    }
};