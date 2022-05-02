#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

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

std::string gameMaps[] = {
    "res/map/g2.map",
    "res/map/e3.map"
};

std::string gameTileSets[] = {
    "res/tileset/g2o_tiles.png",
    "res/tileset/e3o_tiles.png"
};

Map* playerMap;

mPlayer mainPlayer;
gameCam mainCamera;

string Type[]= {
	"Normal",
	"Fire",
	"Water",
	"Electric",
	"Grass",
	"Ice",
	"Fighting",
	"Poison",
	"Ground",
	"Flying",
	"Psychic",
	"Bug",
	"Rock",
	"Ghost",
	"Dragon",
	"Dark",
	"Steel",
	"Fairy"
};

Move moves[]=
{
	{"flamethrower",1,90,15},
	{"tackle",0,40,35},
    {"thunderbolt",3,90,15},
    {"surf",2,90,15},
    {"earthquake",8,100,10}
};
PokemonData pokemonData[]=
{
	{"Charmander",1,99,65,55,{moves,moves+1}},
	{"Pikachu",3,95,55,55,{moves+1,moves+2}}
};
//              Test
Pokemon pokemon[]={0,1};
Pokemon opParty[]={0,1};
//              

// GAME STATES

bool tsToMapTransition;
bool beginMapToMapTransition;
bool finishMapToMapTransition;
static int transitionTransparency = 0;

bool hasSaveFile = true;
bool quit = false;

bool inTitleScreen = false; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG

bool inDialogue = false;
bool inBattle = false;
bool playerIsRunning = false;

// CORE GAME FUNCTIONS

void initSystem();
void gameLoop();
void titleScreenInputProcess(SDL_Event* e);
void overworldInputProcess(SDL_Event* e, int pCX, int pCY);

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

    // LOAD SAVE FILE
    if (!mainPlayer.loadPlayerData()) {
        hasSaveFile = false;
        inTitleScreen = true;
    } else {
        mainPlayer.initPlayerTexture();
        mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
    }

    // INIT THE TITLE SCREEN
    gameTitleScreen.initTitleScreen(hasSaveFile);
    gameTitleScreen.tsButtonInit();

    // INIT THE PLAYER'S CURRENT MAP
    playerMap = new Map;
    playerMap->loadMap(gameMaps[mainPlayer.getCurrentMap()].c_str(), gameTileSets[mainPlayer.getCurrentMap()].c_str(), "res/music/overworldMusic.mp3", 8.85);

    blackTransitionTexture = renderWindow.loadTexture("res/otherassets/blacktransition.png");
    SDL_SetTextureBlendMode(blackTransitionTexture, SDL_BLENDMODE_BLEND);
}

void overworldInputProcess(SDL_Event* e, int pCX, int pCY) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT)
        {
            mainPlayer.savePlayerData();
            delete playerMap;
            quit = true;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            cerr << e->motion.x << " " << e->motion.y << endl;
            // playerMap->mapTheme.manualSkip(70.03); // MUSIC TESTING
            cout << mainPlayer.getXCoords() << " " << mainPlayer.getYCoords() << endl;
        }
        else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b and inDialogue == false) // START A BATTLE
        { 
            battle(pokemon,opParty);
        } 
        else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_x) // INTERACT WITH NPCS AND BLOCKS
        {
            NPC* selNPC = playerMap->getNearbyNPC(pCX, pCY, mainPlayer.getFacingDirection());
            if (selNPC != NULL) {
                if (selNPC->talkNPC(mainPlayer.getFacingDirection()) == true) inDialogue = true;
                else inDialogue = false;
            }
        } 
        else if 
        (e->type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e->key.repeat == 0 and inDialogue == false) // BEGIN MOVEMENT
        { 
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
            WarpTile* selWarpTile = playerMap->getNearbyWarpTile(pCX, pCY, mainPlayer.getFacingDirection());
            if (selWarpTile == NULL) {
                mainCamera.beginMovement(e, pCX, pCY, playerMap->getCollisionMap());
            } else {
                beginMapToMapTransition = true;
            }
        } 
        else if (e->type == SDL_KEYUP and mainCamera.getMovementState() == true and e->key.repeat == 0) // STOP MOVEMENT 
        { 
            mainCamera.stopMovement(e);
        }
    }
}

void titleScreenInputProcess(SDL_Event* e) // ALREADY MOSTLY FINISHED. DO NOT TOUCH.
{ 
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
        if (inTitleScreen == true) // PLAYER IN THE TITLE SCREEN. AGAIN, MOSTLY FINISHED, DO NOT TOUCH
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

            // MOVE THE CAMERA
            if (mainCamera.getFinishingState() == true)
            { 
                mainCamera.finishMovement();
            } 
            else 
            {
                mainCamera.moveCamera(pCX, pCY, playerMap->getCollisionMap());
                mainCamera.finishIllegalPos(playerMap->getMapWidth(), playerMap->getMapHeight());
            }

            // UPDATE THE PLAYER COORDINATES
            mainPlayer.setPlayerCoords(mainCamera.getCamX() / 64 + 6, mainCamera.getCamY() / 64 + 5, playerMap->getMapID()); 

            // PRPARE THE RENDERING SCREEN
            renderWindow.drawColor(0, 0, 0); 
            renderWindow.clear();

            playerMap->drawMap(&mainCamera); // DRAW THE MAP
            playerMap->drawNPCs(&mainCamera); // DRAW THE MAP'S NPC

            // DRAW THE PLAYER
            if (mainCamera.getMovementState() == false) mainPlayer.renderStandingPlayer();  
            else {
                if (playerIsRunning == true) mainPlayer.renderRunningPlayer(); 
                else mainPlayer.renderMovingPlayer();
            }

            playerMap->drawFrontNPCs(&mainCamera); // DRAW NPCs THAT ARE IN FRONT OF THE PLAYER
            
            // ONLY ACTIVATED GOING FROM THE TITLE SCREEN TO THE OVERWORLD. SMOOTH BLACK TRANSITION
            if (tsToMapTransition == true) { 
                if (transitionTransparency > 0) {
                    transitionTransparency -= 17;
                } else if (transitionTransparency <= 0) {
                    tsToMapTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            // MAP TO MAP TRANSITION HANDLING
            if (beginMapToMapTransition == true) {
                Mix_FadeOutMusic(1500);
                if (transitionTransparency < 255) {
                    transitionTransparency += 17;
                } else if (transitionTransparency >= 255) {
                    WarpTile* selWarpTile = playerMap->getNearbyWarpTile(pCX, pCY, mainPlayer.getFacingDirection());
                    int tdM = selWarpTile->getDestMap(), tdX = selWarpTile->getDestX(), tdY = selWarpTile->getDestY();
                    beginMapToMapTransition = false;
                    finishMapToMapTransition = true;
                    playerMap->mapTheme.resetChord();
                    playerMap->freeMap();
                    playerMap->loadMap(gameMaps[tdM].c_str(), gameTileSets[tdM].c_str(), "res/music/overworldMusic.mp3", 8.85);
                    mainPlayer.setPlayerCoords(tdX, tdY, tdM);
                    mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            if (finishMapToMapTransition == true) {
                if (transitionTransparency > 0) {
                    transitionTransparency -= 17;
                } else if (transitionTransparency <= 0) {
                    finishMapToMapTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            if (beginMapToMapTransition == false) playerMap->playMapTheme(); // PLAY THE MAP'S THEME
            
            renderWindow.display(); // DISPLAY THE CONTENT TO THE WINDOW

            SDL_Delay(1000 / 60); // PRIMITIVE CAP FRAME RATE

        }
    }
}