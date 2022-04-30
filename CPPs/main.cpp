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
#include "NPCs.h"

RenderWindow renderWindow;

TitleScreen gameTitleScreen;
SDL_Texture* blackTransitionTexture;

Map g2Map;
mPlayer mainPlayer;
gameCam mainCamera;

Move moves[]=
{
	{"flamethrower",90},
	{"tackle",40},
    {"thunderbolt",90},
    {"surf",90},
    {"earthquake",100}
};
Pokemon pokemon[]=
{
	{"Charmander",99,65,55,{moves,moves+1}},
	{"Pikachu",95,55,55,{moves+1,moves+2}}
};


bool tsToMapTransition;
static int transitionTransparency = 0;

bool hasSaveFile = true;
bool quit = false;

bool inTitleScreen = false; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG

bool inDialogue = false;
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

    if (!mainPlayer.loadPlayerData()) {
        hasSaveFile = false;
        inTitleScreen = true;
    } else {
        mainPlayer.initPlayerTexture();
        mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
    }

    gameTitleScreen.initTitleScreen(hasSaveFile);
    gameTitleScreen.tsButtonInit();

    g2Map.loadMap("res/map/g2.map", "res/tileset/g2o_tiles.png", "res/music/overworldMusic.mp3", 8.85);
    g2Map.loadNPCs("res/map/g2.npc");

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
            cout << mainPlayer.getXCoords() << " " << mainPlayer.getYCoords() << endl;
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b and inDialogue == false) { // START A BATTLE
            battle(pokemon[0], pokemon[1]);
        } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_x) {
            NPC* selNPC = g2Map.getNearbyNPC(pCX, pCY, mainPlayer.getFacingDirection());
            if (selNPC != NULL) {
                if (selNPC->talkNPC(mainPlayer.getFacingDirection()) == true) inDialogue = true;
                else inDialogue = false;
            }
        } else if (e->type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e->key.repeat == 0 and inDialogue == false) { // BEGIN MOVEMENT
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
        } else if (e->type == SDL_KEYUP and mainCamera.getMovementState() == true and e->key.repeat == 0) { // STOP MOVEMENT 
            mainCamera.stopMovement(e);
        }
    }
}

void titleScreenInputProcess(SDL_Event* e) { // ALREADY MOSTLY FINISHED. DO NOT TOUCH.
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
        if (inTitleScreen == true) // PLAYER IN THE TITLE SCREEN. AGAIN, MOSTLY FINSIHED, DO NOT TOUCH
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
            int pCX = mainPlayer.getXCoords(), pCY = mainPlayer.getYCoords(); // GET PLAYER COORDS
            overworldInputProcess(&e, pCX, pCY); // INPUT PROCESS

            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if (currentKeyStates[SDL_SCANCODE_Z]) {mainCamera.speedUp(); playerIsRunning = true;} // TOGGLE RUNNING STATE
            else {mainCamera.slowDown(); playerIsRunning = false;}

            if (mainCamera.getFinishingState() == true) { // MOVE THE CAMERA
                mainCamera.finishMovement();
            } else {
                mainCamera.moveCamera(pCX, pCY, g2Map.getCollisionMap());
                mainCamera.finishIllegalPos(g2Map.getMapWidth(), g2Map.getMapHeight());
            }

            mainPlayer.setPlayerCoords(mainCamera.getCamX() / 64 + 6, mainCamera.getCamY() / 64 + 5); // UPDATE THE PLAYER COORDINATES

            renderWindow.drawColor(0, 0, 0); // PRPARE THE RENDERING SCREEN
            renderWindow.clear();

            g2Map.drawMap(&mainCamera); // DRAW THE MAP
            g2Map.drawNPCs(&mainCamera); // DRAW THE MAP'S NPC

            if (mainCamera.getMovementState() == false) mainPlayer.renderStandingPlayer(); // DRAW THE PLAYER 
            else {
                if (playerIsRunning == true) mainPlayer.renderRunningPlayer(); 
                else mainPlayer.renderMovingPlayer();
            }

            g2Map.drawFrontNPCs(&mainCamera); // DRAW NPCs THAT ARE IN FRONT OF THE PLAYER
            
            if (tsToMapTransition == true) { // ONLY ACTIVATED GOING FROM THE TITLE SCREEN TO THE OVERWORLD. SMOOTH BLACK TRANSITION
                if (transitionTransparency > 0) {
                    transitionTransparency -= 17;
                } else if (transitionTransparency <= 0) {
                    tsToMapTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            g2Map.playMapTheme();

            renderWindow.display(); // DISPLAY THE CONTENT TO THE WINDOW

            SDL_Delay(1000 / 60); // PRIMITIVE CAP FRAME RATE

        }
    }
}