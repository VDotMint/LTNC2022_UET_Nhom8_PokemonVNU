#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <iomanip>

#include "titleScreen.h"
#include "RenderWindow.h"
#include "music.h"
#include "Tiling.h"
#include "map.h"
#include "mPlayer.h"
#include "camera.h"
#include "Battle.h"

RenderWindow renderWindow;

TitleScreen gameTitleScreen;
SDL_Texture* blackTransitionTexture;

Map g2Map;
mPlayer mainPlayer;
gameCam mainCamera;

Pokemon pokemon[]=
{
	{"Charmander",10},
	{"Pikachu",8}
};

Move moves[]=
{
	{"ember",2},
	{"tackle",1}
};

bool tsToMapTransition;
static int transitionTransparency = 0;

bool hasSaveFile = true;
bool quit = false;

bool inTitleScreen = true; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG

bool inBattle = false;

bool playerIsRunning = false;

void initSystem();
void gameLoop();
void titleScreenInputProcess(SDL_Event* e);
void overworldInputProcess(SDL_Event* e, int pCX, int pCY);
void battle(Pokemon my, Pokemon op);

int main(int argc, char *argv[]) {
    initSystem();
    gameLoop();
    renderWindow.close();
    return 0;
}

void initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    renderWindow.create("Pokémon VNU");

    if (!mainPlayer.loadPlayerData()) hasSaveFile = false;
    else {
        mainPlayer.initPlayerTexture();
        mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
    }

    gameTitleScreen.initTitleScreen(hasSaveFile);
    gameTitleScreen.tsButtonInit();

    g2Map.loadMap("res/map/g2.map", "res/tileset/g2o_tiles.png", "res/music/overworldMusic.mp3", 8.85);

    blackTransitionTexture = renderWindow.loadTexture("res/otherassets/blacktransition.png");
    SDL_SetTextureBlendMode(blackTransitionTexture, SDL_BLENDMODE_BLEND);
    
}

void overworldInputProcess(SDL_Event* e, int pCX, int pCY) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            mainPlayer.savePlayerData();
            quit = true;
        } else if (e->type == SDL_MOUSEBUTTONDOWN) {
            cerr << e->motion.x << " " << e->motion.y << endl;
            g2Map.mapTheme.manualSkip(70.03); // MUSIC TESTING
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
            battle(pokemon[0],pokemon[1]);
        } else if (e->type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e->key.repeat == 0) {
            switch (e->key.keysym.sym) {
                case SDLK_s:
                    mainPlayer.changeFacingDirect(0);
                    break;
                case SDLK_d:
                    mainPlayer.changeFacingDirect(1);
                    break;
                case SDLK_w:
                    mainPlayer.changeFacingDirect(2);
                    break;
                case SDLK_a:
                    mainPlayer.changeFacingDirect(3);
                    break;
                default:
                    break;
            }
            mainCamera.beginMovement(e, pCX, pCY, g2Map.getCollisionMap());
        } else if (e->type == SDL_KEYUP and mainCamera.getMovementState() == true and e->key.repeat == 0) {
            mainCamera.stopMovement(e);
        }
    }
}

void titleScreenInputProcess(SDL_Event* e) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            quit = true;
        }
        if (gameTitleScreen.acceptInputState() == true) {
            gameTitleScreen.doButtonEvents(e);
            if (hasSaveFile == true and gameTitleScreen.tsButtons[0].isClicked() == true) {
                gameTitleScreen.tsButtons[0].resetClickState();
                gameTitleScreen.stopInputState();
                tsToMapTransition = true;
            } else if (gameTitleScreen.tsButtons[1].isClicked() == true) {
                gameTitleScreen.tsButtons[1].resetClickState();
                mPlayer newTempPlayer;
                mainPlayer = newTempPlayer;
                mainPlayer.initPlayerTexture();
                mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
                gameTitleScreen.stopInputState();
                tsToMapTransition = true;                
            } else if (gameTitleScreen.tsButtons[3].isClicked() == true) {
                quit = true;
                gameTitleScreen.freeTitleScreen();
            } 
        }
    }
}

void gameLoop() {
    SDL_Event e;

    while (quit == false) {
        if (inTitleScreen == true) // PLAYER IN THE TITLE SCREEN
        {
            titleScreenInputProcess(&e);
            
            renderWindow.drawColor(0, 0, 0);
            renderWindow.clear();            

            gameTitleScreen.drawTitleScreen();

            if (tsToMapTransition == true) {
                if (transitionTransparency < 255) {
                    transitionTransparency += 17;
                } else if (transitionTransparency >= 255) {
                    inTitleScreen = false;
                    gameTitleScreen.freeTitleScreen();
                    SDL_Delay(2000);
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            renderWindow.display();

            SDL_Delay(1000/60);
        }
        
        else // PLAYER IN THE MAP
        
        {
            int pCX = mainPlayer.getXCoords(), pCY = mainPlayer.getYCoords();
            overworldInputProcess(&e, pCX, pCY);

            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if (currentKeyStates[SDL_SCANCODE_Z]) {mainCamera.speedUp(); playerIsRunning = true;}
            else {mainCamera.slowDown(); playerIsRunning = false;}

            if (mainCamera.getFinishingState() == true) {
                mainCamera.finishMovement();
            } else {
                mainCamera.moveCamera(pCX, pCY, g2Map.getCollisionMap());
                mainCamera.finishIllegalPos(g2Map.getMapWidth(), g2Map.getMapHeight());
            }

            mainPlayer.setPlayerCoords(mainCamera.getCamX() / 64 + 6, mainCamera.getCamY() / 64 + 5);

            renderWindow.drawColor(0, 0, 0);
            renderWindow.clear();

            g2Map.drawMap(&mainCamera);

            if (mainCamera.getMovementState() == false) mainPlayer.renderStandingPlayer();
            else {
                if (playerIsRunning == true) mainPlayer.renderRunningPlayer();
                else mainPlayer.renderMovingPlayer();
            }
            
            if (tsToMapTransition == true) {
                if (transitionTransparency > 0) {
                    transitionTransparency -= 17;
                } else if (transitionTransparency <= 0) {
                    tsToMapTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            renderWindow.display();

            SDL_Delay(1000 / 60);

            g2Map.playMapTheme();
        }
    }
}