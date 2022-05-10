#pragma once
#include "Pokemon.h"
#include "RenderWindow.h"
#include "titleScreen.h"
#include "music.h"
#include "Tiling.h"
#include "map.h"
#include "mPlayer.h"
#include "camera.h"
#include "Battle.h"
#include "BattleScreen.h"
#include "NPCs.h"
#include "otherGraphics.h"

using namespace std;

extern bool debugMode;

extern RenderWindow renderWindow;

extern TitleScreen gameTitleScreen;
extern SDL_Texture* blackTransitionTexture;
extern string gameMaps[];
extern string gameTileSets[];
extern string gameThemes[];

extern double themeRepeats[];
extern bool mapOverlays[];

extern string Type[];
extern Move moves[];
extern PokemonData pokemonData[];
extern int psize;

extern SDL_Rect dBoxClip;
extern dialogueBox d_box;
extern Text d_text;

extern Mix_Chunk* changeMap;
extern Mix_Chunk* aButton;
            

extern Map* playerMap;
extern Music gameMusic;

extern mPlayer mainPlayer;
extern gameCam mainCamera;

extern BattleScreen mainBattle;

// GAME STATES

extern bool tsToMapTransition;
extern bool beginMapToMapTransition;
extern bool finishMapToMapTransition;
extern int transitionTransparency;

extern bool beginMapToBattleTransition;
extern bool finishMapToBattleTransition;
extern bool beginBattleToMapTransition;
extern bool finishBattleToMapTransition;

extern bool hasSaveFile;
extern bool quit;

extern bool inTitleScreen; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG
extern bool inBattle;
extern bool inDialogue;

extern bool playerIsRunning;

extern Trainer defaultOppo;