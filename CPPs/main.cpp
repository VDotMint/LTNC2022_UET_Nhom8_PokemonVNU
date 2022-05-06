#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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
#include "Pokemon.h"
#include "otherGraphics.h"

bool debugMode = false;

RenderWindow renderWindow;

TitleScreen gameTitleScreen;
SDL_Texture* blackTransitionTexture;

SDL_Rect dBoxClip = { 10, 515, 812, 179 };
dialogueBox d_box;
Text d_text;

Mix_Chunk* changeMap;
Mix_Chunk* aButton;

std::string gameMaps[] = {
    "res/map/g2.map",
    "res/map/e3.map",
    "res/map/e3i.map",
    "res/map/g2i.map"
};

std::string gameTileSets[] = {
    "res/tileset/g2o_tiles.png",
    "res/tileset/e3o_tiles.png",
    "res/tileset/e3i_tiles.png",
    "res/tileset/g2i_tiles.png",
};

std::string gameThemes[] = {
    "res/music/g2o_theme.mp3",
    "res/music/e3o_theme.mp3",
    "res/music/e3i_theme.mp3",
    "res/music/e3i_theme.mp3",
};

double themeRepeats[] = {
    8.85,
    30.555,
    0.77,
    0.77,
};

bool mapOverlays[] = {
    false,
    false,
    true,
    true,
};

Map* playerMap;
Music gameMusic;

mPlayer mainPlayer;
gameCam mainCamera;

BattleScreen mainBattle;

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
	{"Charmander",1,99,61,52,70,{moves,moves+1}},
	{"Pikachu",3,105,83,60,125,{moves+1,moves+2}}
};
//              Test
Pokemon pokemon[]={0,1};
Pokemon opParty[]={0,1};
//              

// GAME STATES

bool tsToMapTransition = false;
bool beginMapToMapTransition = false;
bool finishMapToMapTransition = false;
static int transitionTransparency = 0;

bool beginMapToBattleTransition = false;
bool finishMapToBattleTransition = false;
bool beginBattleToMapTransition = false;
bool finishBattleToMapTransition = false;

bool hasSaveFile = true;
bool quit = false;

bool inTitleScreen = false; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG
bool inBattle = false;
bool inDialogue = false;

bool playerIsRunning = false;

// CORE GAME FUNCTIONS

void initSystem();
void gameLoop();
void titleScreenInputProcess(SDL_Event* e);
void battleInputProcess(SDL_Event *e);
void overworldInputProcess(SDL_Event* e, int pCX, int pCY);
void freeMainAssets();

int main(int argc, char *argv[]) {
    initSystem();
    gameLoop();
    freeMainAssets();
    renderWindow.close();
    return 0;
}

void initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
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
    playerMap->loadMap(gameMaps[mainPlayer.getCurrentMap()].c_str(), gameTileSets[mainPlayer.getCurrentMap()].c_str(), gameThemes[mainPlayer.getCurrentMap()].c_str(), themeRepeats[mainPlayer.getCurrentMap()], mapOverlays[mainPlayer.getCurrentMap()]);

    // INIT THE OVERLAY ELEMENTS TEXTURE
    playerMap->initOverlayElements("res/tileset/overlaying_tiles.png");

    // LOAD TRANSITION EFFECT
    blackTransitionTexture = renderWindow.loadTexture("res/otherassets/blacktransition.png");
    SDL_SetTextureBlendMode(blackTransitionTexture, SDL_BLENDMODE_BLEND);

    // INIT THE SOUND EFFECTS
    gameMusic.loadMusic(gameThemes[mainPlayer.getCurrentMap()].c_str(), themeRepeats[mainPlayer.getCurrentMap()]);
    changeMap = Mix_LoadWAV("res/sfx/change_map.wav");
    aButton = Mix_LoadWAV("res/sfx/a_button.wav");

    // INIT THE DIALOGUE BOX TEXTURE
    d_box.initDialogueBox(RenderWindow::renderer, "res/otherassets/dialoguebox.png");
}

void freeMainAssets() {
    playerMap->freeOverlayElements();
    playerMap->freeMap();
    SDL_DestroyTexture(blackTransitionTexture);
    blackTransitionTexture = NULL;
    gameMusic.freeMusic();
    d_box.~dialogueBox();
    Mix_FreeChunk(changeMap);
    changeMap = NULL;
    Mix_FreeChunk(aButton);
    aButton = NULL;
}

void overworldInputProcess(SDL_Event* e, int pCX, int pCY) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT)
        {
            mainPlayer.savePlayerData();
            quit = true;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            cerr << e->motion.x << " " << e->motion.y << endl;
            // playerMap->mapTheme.manualSkip(57.03); // MUSIC TESTING
            cout << mainPlayer.getXCoords() << " " << mainPlayer.getYCoords() << " " << mainPlayer.getCurrentMap() << endl;
            cout << boolalpha << beginMapToBattleTransition << " " << finishMapToBattleTransition << " " << beginBattleToMapTransition << " " << finishBattleToMapTransition << endl;
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
                Mix_PlayChannel(-1, aButton, 0);
            } else {
                InterTile* selInterTile = playerMap->getNearbyInterTile(pCX, pCY, mainPlayer.getFacingDirection());
                if (selInterTile != NULL) {
                    if (selInterTile->talkTile() == true) inDialogue = true;
                    else inDialogue = false;
                    Mix_PlayChannel(-1, aButton, 0);
                }
            }
        } 
        else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_n && beginMapToBattleTransition != true && finishBattleToMapTransition != true && inDialogue == false)
        {
            beginMapToBattleTransition = true;
        }
        else if (e->type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e->key.repeat == 0 and inDialogue == false and beginMapToMapTransition == false and finishMapToMapTransition == false) // BEGIN MOVEMENT
        {
            // cout << e->key.keysym.sym << " clicked down at game tick: " << SDL_GetTicks() << endl;
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
            if (selWarpTile != NULL and (e->key.keysym.sym == SDLK_w or e->key.keysym.sym == SDLK_a or e->key.keysym.sym == SDLK_s or e->key.keysym.sym == SDLK_d)) {
                beginMapToMapTransition = true;
            } else {
                mainCamera.beginMovement(e, pCX, pCY, playerMap->getCollisionMap());
            }
        } 
        else if (e->type == SDL_KEYUP and mainCamera.getMovementState() == true and e->key.repeat == 0) // STOP MOVEMENT 
        { 
            mainCamera.stopMovement(e);
        }
    }
}

void battleInputProcess(SDL_Event* e) // THE BATTLE INPUT PROCESS
{
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            quit = true;
        }
        else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_m && finishMapToBattleTransition != true && beginBattleToMapTransition != true)
        {
            beginBattleToMapTransition = true;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN)
        {
            cerr << e->motion.x << " " << e->motion.y << endl;
        }
    }
}

void titleScreenInputProcess(SDL_Event* e) // ALREADY MOSTLY FINISHED. DO NOT TOUCH.
{ 
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            quit = true;
        }
        if (gameTitleScreen.acceptInputState() == true)
        {
            gameTitleScreen.doButtonEvents(e);
            if (hasSaveFile == true and gameTitleScreen.tsButtons[0].isClicked() == true) {
                Mix_PlayChannel(-1, aButton, 0);
                gameTitleScreen.tsButtons[0].resetClickState();
                gameTitleScreen.stopInputState();
                tsToMapTransition = true;
            } else if (gameTitleScreen.tsButtons[1].isClicked() == true) {
                Mix_PlayChannel(-1, aButton, 0);
                gameTitleScreen.tsButtons[1].resetClickState();
                mPlayer newTempPlayer;
                mainPlayer = newTempPlayer;
                mainPlayer.initPlayerTexture();
                mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
                gameTitleScreen.stopInputState();
                tsToMapTransition = true;
            } else if (gameTitleScreen.tsButtons[3].isClicked() == true) {
                Mix_PlayChannel(-1, aButton, 0);
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
        
        else if (inBattle == true) // PLAYER IN BATTLE
        {
            battleInputProcess(&e);

            renderWindow.drawColor(255, 0, 255);
            renderWindow.clear();

            mainBattle.drawBattleScreen(finishMapToBattleTransition, beginBattleToMapTransition);

            if (finishMapToBattleTransition == true) {
                if (transitionTransparency > 0) {
                    transitionTransparency -= 5;
                } else if (transitionTransparency <= 0) {
                    transitionTransparency = 0;
                    finishMapToBattleTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            if (beginBattleToMapTransition == true) {
                if (transitionTransparency < 255) {
                    transitionTransparency += 5;
                } else if (transitionTransparency >= 255) {
                    transitionTransparency = 255;
                    inBattle = false;
                    beginBattleToMapTransition = false;
                    finishBattleToMapTransition = true;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            gameMusic.play();

            renderWindow.display();

            SDL_Delay(1000 / 60);
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

            // DRAW THE OVERLAYING ELEMENTS IF THE MAP HAS THEM
            if (mapOverlays[mainPlayer.getCurrentMap()] == true) {
                playerMap->drawOverlay(&mainCamera);
            }
            
            // NPCDIALOGUE and INTERTILEDIALOGUE
            if (inDialogue == true) {
                d_box.renderDialogueBox(RenderWindow::renderer, &dBoxClip);
                NPC* selNPC = playerMap->getNearbyNPC(pCX, pCY, mainPlayer.getFacingDirection());
                if (selNPC != NULL) {
                    d_text.textInit(RenderWindow::renderer, "res/font/gamefont.ttf", 38, selNPC->getCurrentSentence().c_str(), { 0, 0, 0 });
                    d_text.display(48, 547, RenderWindow::renderer);
                }
                else {
                    InterTile* selInterTile = playerMap->getNearbyInterTile(pCX, pCY, mainPlayer.getFacingDirection());
                    if (selInterTile != NULL) {
                        d_text.textInit(RenderWindow::renderer, "res/font/gamefont.ttf", 38, selInterTile->getInterCurrentSentence().c_str(), { 0, 0, 0 });
                        d_text.display(48, 547, RenderWindow::renderer);
                    }
                }
            }

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
                // Mix_FadeOutMusic(500);
                if (transitionTransparency < 255) {
                    transitionTransparency += 17;
                    if (transitionTransparency == 51) Mix_PlayChannel(-1, changeMap, 0);
                } else if (transitionTransparency >= 255) {
                    WarpTile* selWarpTile = playerMap->getNearbyWarpTile(pCX, pCY, mainPlayer.getFacingDirection());
                    int tdM = selWarpTile->getDestMap(), tdX = selWarpTile->getDestX(), tdY = selWarpTile->getDestY();
                    beginMapToMapTransition = false;
                    finishMapToMapTransition = true;
                    if (gameThemes[tdM] != gameThemes[mainPlayer.getCurrentMap()]) {
                        Mix_FadeOutMusic(500);
                        gameMusic.freeMusic();
                        gameMusic.loadMusic(gameThemes[tdM].c_str(), themeRepeats[tdM]);
                        gameMusic.resetChord();
                    }
                    playerMap->freeMap();
                    playerMap->loadMap(gameMaps[tdM].c_str(), gameTileSets[tdM].c_str(), gameThemes[tdM].c_str(), themeRepeats[tdM], mapOverlays[tdM]);
                    mainPlayer.setPlayerCoords(tdX, tdY, tdM);
                    mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            // FINISH THE MAP TRANSITION EFFECT
            if (finishMapToMapTransition == true) {
                if (transitionTransparency > 0) {
                    transitionTransparency -= 17;
                } else if (transitionTransparency <= 0) {
                    finishMapToMapTransition = false;
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            // MAP TO BATTLE TRANSITION HANDLING
            if (beginMapToBattleTransition == true) {
                gameMusic.freeMusic();
                gameMusic.resetChord();
                gameMusic.loadMusic("res/music/battleMusic.mp3", 19.9);
                if (transitionTransparency < 255) {
                    transitionTransparency += 5;
                } else if (transitionTransparency >= 255) {
                    Trainer tempoppo;
                    tempoppo.name = "Champion Cynthia";
                    transitionTransparency = 255;
                    inBattle = true;
                    beginMapToBattleTransition = false;
                    finishMapToBattleTransition = true;
                    mainBattle.initBattleScreen(&mainPlayer, &tempoppo);
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            // FINISHING BATTLE TRANSITION HANDLING
            if (finishBattleToMapTransition == true) {
                gameMusic.freeMusic();
                gameMusic.resetChord();
                gameMusic.loadMusic(gameThemes[mainPlayer.getCurrentMap()].c_str(), themeRepeats[mainPlayer.getCurrentMap()]);

                if (transitionTransparency > 0) {
                    transitionTransparency -= 5;
                } else if (transitionTransparency <= 0) {
                    transitionTransparency = 0;
                    finishBattleToMapTransition = false;
                    mainBattle.freeBattleScreen();
                }
                SDL_SetTextureAlphaMod(blackTransitionTexture, transitionTransparency);
                SDL_RenderCopy(RenderWindow::renderer, blackTransitionTexture, NULL, NULL);
            }

            if (beginMapToMapTransition == false) gameMusic.play(); // PLAY THE MAP'S THEME
            
            renderWindow.display(); // DISPLAY THE CONTENT TO THE WINDOW

            SDL_Delay(1000 / 60); // PRIMITIVE CAP FRAME RATE
        }
    }
}