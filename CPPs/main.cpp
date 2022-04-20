#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <iomanip>

#include "RenderWindow.h"
#include "music.h"
#include "Tiling.h"
#include "map.h"
#include "mPlayer.h"
#include "camera.h"
#include "Battle.h"

using namespace std;

RenderWindow renderWindow;
TileSheet g2TileSheet;
Map g2Map;
Music gameTheme;
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

bool quit = false;
bool inTitleScreen = false;
bool inBattle = false;
bool running = false;

void initSystem();
void gameLoop();
void titleScreenInputProcess(SDL_Event e);
void overworldInputProcess(SDL_Event e);
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
    g2TileSheet.loadTileSheet("res/tileset/g2o_tiles.png");
    g2Map.loadMap("res/map/g2.map");
    gameTheme.loadMusic("res/music/pallettown.mp3");
    if (!mainPlayer.loadPlayerData()) cout << "No player save detected!\n";
    else {
        mainPlayer.initPlayerTexture();
        mainCamera.setCameraPos((mainPlayer.getXCoords() - 6) * 64, (mainPlayer.getYCoords() - 5) * 64);
    }
}

void overworldInputProcess(SDL_Event e, int pCX, int pCY) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            mainPlayer.savePlayerData();
            quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            cerr << e.motion.x << " " << e.motion.y << endl;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_b) {
            battle(pokemon[0],pokemon[1]);
        } else if (e.type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e.key.repeat == 0) {
            switch (e.key.keysym.sym) {
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
            mainCamera.beginMovement(&e, pCX, pCY, g2Map.getCollisionMap());
        } else if (e.type == SDL_KEYUP and mainCamera.getMovementState() == true and e.key.repeat == 0) {
            mainCamera.stopMovement(&e);
        }
    }
}

void titleScreenInputProcess(SDL_Event e) {
    while (SDL_PollEvent(&e)) {

    }
}

void gameLoop() {
    SDL_Event e;

    while (quit == false) {
        if (inTitleScreen == true) {

        } else {
            int pCX = mainPlayer.getXCoords(), pCY = mainPlayer.getYCoords();
            overworldInputProcess(e, pCX, pCY);

            const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
            if (currentKeyStates[SDL_SCANCODE_Z]) mainCamera.speedUp();
            else mainCamera.slowDown();

            if (mainCamera.getFinishingState() == true) {
                mainCamera.finishMovement();
            } else {
                mainCamera.moveCamera(pCX, pCY, g2Map.getCollisionMap());
                mainCamera.finishIllegalPos(g2Map.getMapWidth(), g2Map.getMapHeight());
            }

            mainPlayer.setPlayerCoords(mainCamera.getCamX() / 64 + 6, mainCamera.getCamY() / 64 + 5);

            renderWindow.drawColor(0, 0, 0);
            renderWindow.clear();

            g2Map.drawMap(&g2TileSheet, &mainCamera);

            if (mainCamera.getMovementState() == true) mainPlayer.renderMovingPlayer();
            else mainPlayer.renderStandingPlayer();

            renderWindow.display();

            SDL_Delay(1000 / 60);

            if (Mix_PlayingMusic() == 0) gameTheme.play();
        }
    }
}

void battle(Pokemon my,Pokemon op) {
	bool battleState = true;
	int turn = 0;
	int input;
	cout << "Turn: " << turn << '\n';
	cout << setw(12) << setfill(' ') << left << my.name << setw(50) << setfill(' ') << right << op.name << '\n';
	cout << setw(12) << setfill(' ') << left << my.health << setw(50) << setfill(' ') << right << op.health << '\n';
	while (battleState) {
		turn++;
		cout<<"Turn: "<<turn<<'\n';
		cout<<"Choose your move\n";
		for (int i=0;i<2;i++) {
			cout<<i<<": "<<moves[i].name<<'\n';
		}
		cin>>input;
		if (input>1) {
			cout<<"invalid move\n";
			continue;
		}
		else {
			op.health-=moves[input].damage;
			if (op.health<0) op.health=0;
		}
		cout<<my.name<<" used "<<moves[input].name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.name<<setw(50)<<setfill(' ')<<right<<op.name<<'\n';
		cout<<setw(12)<<setfill(' ')<<left<<my.health<<setw(50)<<setfill(' ')<<right<<op.health<<'\n';
		if (!op.health) {
			cout<<"You win\n";
			break;
		}
	}
};