#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "RenderWindow.h"
#include "music.h"
#include "tile.h"
#include "tilesheet.h"
#include "map.h"
#include "mPlayer.h"
#include "camera.h"

using namespace std;

const int MOVE_SPEED = 4;

RenderWindow renderWindow;
TileSheet g2TileSheet;
Map g2Map;
Music gameTheme;
mPlayer mainPlayer;
gameCam mainCamera;

bool quit = false;
void initSystem();
void gameLoop();

int main(int argc, char* argv[]) {
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
    g2TileSheet.loadTileSheet("res/tileset/tileset03.png");
    g2Map.loadMap("res/map/g2.map");
    gameTheme.loadMusic("res/music/fridaynight.mp3");
    if (!mainPlayer.loadPlayerData()) {
        cout << "No player save detected!\n";
    } else {
        mainCamera.setCameraPos(mainPlayer.getXCoords() * 64, mainPlayer.getYCoords() * 64);
    }
}

void gameLoop() {
    SDL_Event e;

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                mainPlayer.savePlayerData();
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                cerr << e.motion.x << " " << e.motion.y << endl;
            } else if (e.type == SDL_KEYDOWN and mainCamera.getMovementState() == false and e.key.repeat == 0) {
                mainCamera.beginMovement(&e);
            } else if (e.type == SDL_KEYUP and mainCamera.getMovementState() == true and e.key.repeat == 0) {
                mainCamera.stopMovement(&e);
            }
        }

        if (mainCamera.getFinishingState() == true) {
            mainCamera.finishMovement();
        } else {
            mainCamera.moveCamera();
            mainCamera.finishIllegalPos(g2Map.getMapWidth(), g2Map.getMapHeight());
        }

        mainPlayer.setPlayerCoords(mainCamera.getCamX()/64, mainCamera.getCamY()/64);

        SDL_Delay(1000/60);

        renderWindow.drawColor(0,0,0);
        renderWindow.clear();
        
        g2Map.drawMap(&g2TileSheet, &mainCamera);

        renderWindow.display();

        if (Mix_PlayingMusic() == 0) {
            gameTheme.play();
        }
    }
}