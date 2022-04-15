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

const int MOVE_SPEED = 4;

RenderWindow renderWindow;
TileSheet g2TileSheet;
Map g2Map;
Music gameTheme;
mPlayer mainPlayer;
SDL_Rect camera = {0, 0, 64*13, 64*11};

bool quit = false;
void initSystem();
void gameLoop();
void moveCamera();
void finishMovement();

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
        camera.x = mainPlayer.getXCoords() * 64;
        camera.y = mainPlayer.getYCoords() * 64;
    }
}

bool isMoving = false;
bool finishMove = false;
bool moveUp = false, moveLeft = false, moveDown = false, moveRight = false;

void moveCamera() {
    if (moveRight == true) {
        isMoving = true; 
        camera.x += MOVE_SPEED;
    } else if (moveLeft == true) {
        isMoving = true; 
        camera.x -= MOVE_SPEED;
    } else if (moveUp == true) {
        isMoving = true; 
        camera.y -= MOVE_SPEED;
    } else if (moveDown == true) {
        isMoving = true; 
        camera.y += MOVE_SPEED;
    }
}

void finishMovement() {
    if (moveRight == true) {
        if (camera.x % 64 != 0) {
            camera.x += MOVE_SPEED;
        } else {
            moveRight = false;
            isMoving = false;
            finishMove = false;
        }
    } else if (moveLeft == true) {
        if (camera.x % 64 != 0) {
            camera.x -= MOVE_SPEED;
        } else {
            isMoving = false;
            moveLeft = false;
            finishMove = false;
        }
    } else if (moveUp == true) {
        if (camera.y % 64 != 0) {
            camera.y -= MOVE_SPEED;
        } else {
            isMoving = false;
            moveUp = false;
            finishMove = false;
        }
    } else if (moveDown == true) {
        if (camera.y % 64 != 0) {
            camera.y += MOVE_SPEED;
        } else {
            isMoving = false;
            moveDown = false;
            finishMove = false;
        }
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
            } else if (e.type == SDL_KEYDOWN and isMoving == false and e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        cout << "Key pressed: W" << endl;
                        moveUp = true; break;
                    case SDLK_a:
                        cout << "Key pressed: A" << endl;
                        moveLeft = true; break;
                    case SDLK_s:
                        cout << "Key pressed: S" << endl;
                        moveDown = true; break;
                    case SDLK_d:
                        cout << "Key pressed: D" << endl;
                        moveRight = true; break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYUP and isMoving == true and e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        cout << "Key released: W" << endl;
                        finishMove = true; break;
                    case SDLK_a:
                        cout << "Key released: A" << endl;
                        finishMove = true; break;
                    case SDLK_s:
                        cout << "Key released: S" << endl;
                        finishMove = true; break;
                    case SDLK_d:
                        cout << "Key released: D" << endl;
                        finishMove = true; break;
                    default:
                        break;
                }
            }
        }

        if (finishMove == true) {
            finishMovement();
        } else {
            moveCamera();
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

        mainPlayer.setPlayerCoords(camera.x/64, camera.y/64);

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

