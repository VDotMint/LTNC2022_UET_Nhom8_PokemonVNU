#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "RenderWindow.h"
#include "music.h"
#include "tile.h"
#include "tilesheet.h"
#include "map.h"

using namespace std;

RenderWindow window;
Map testMap;
Music gameTheme;
bool quit = false;

void initSystem();
void gameLoop();

int main(int argc, char* argv[]) {

    initSystem();
    gameLoop();
    window.close();
    return 0;
}



void initSystem() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    RenderWindow window("Pokémon VNU", 832, 704);
    testMap.loadMap();
    gameTheme.loadMusic("res/music/fridaynight.mp3");
}

void gameLoop() {
    SDL_Event e;

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            switch (e.type) {
		        case SDL_QUIT:
			        quit=true;
			        break;
		        case SDL_MOUSEBUTTONDOWN:
			        cerr<<e.motion.x<<" "<<e.motion.y<<endl;
			        break;
		        default:
			        break;
		    }
        }
    }
}

