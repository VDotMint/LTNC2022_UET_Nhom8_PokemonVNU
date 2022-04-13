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

using namespace std;

RenderWindow renderWindow;
TileSheet g2TileSheet;
Map g2Map;
Music gameTheme;
mPlayer mainPlayer;

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
        cout << mainPlayer.getPlayerName() << endl << mainPlayer.getXCoords() << " " << mainPlayer.getYCoords() << endl;
    }
}

void gameLoop() {
    SDL_Event e;

    SDL_Rect camera = {0, 0, 64*13, 64*11};

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                cerr << e.motion.x << " " << e.motion.y << endl;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        camera.y -= 64;
                        break;
                    case SDLK_a:
                        camera.x -= 64;
                        break;
                    case SDLK_s:
                        camera.y += 64;
                        break;
                    case SDLK_d:
                        camera.x += 64;
                        break;
                    default:
                        break;
                }
            }
        }

        if (camera.x > g2Map.getMapWidth()*64 - 832) {
            camera.x -= 64;
        } else if (camera.y > g2Map.getMapHeight()*64 - 704) {
            camera.y -= 64;
        } else if (camera.x < 0) {
            camera.x += 64;
        } else if (camera.y < 0) {
            camera.y += 64;
        }

        SDL_Delay(1000/60);

        renderWindow.drawColor(0,0,0);
        renderWindow.clear();
        
        g2Map.drawMap(&g2TileSheet, &camera);
        
        renderWindow.display();

        if (Mix_PlayingMusic() == 0) {
            gameTheme.play();
        }
    }
}

