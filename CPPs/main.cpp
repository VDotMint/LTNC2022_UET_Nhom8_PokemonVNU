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

RenderWindow renderWindow;
TileSheet g2TileSheet;
Map g2Map;
Music gameTheme;

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
}

void gameLoop() {
    SDL_Event e;

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
		        case SDL_QUIT:
			        quit = true;
			        break;
		        case SDL_MOUSEBUTTONDOWN:
			        cerr << e.motion.x << " "<< e.motion.y << endl;
			        break;
		        default:
			        break;
		    }
        }

        SDL_Delay(1000/60);

        renderWindow.drawColor(0,0,0);
        renderWindow.clear();
        
        g2Map.drawMap(&g2TileSheet);

        renderWindow.display();

        if (Mix_PlayingMusic() == 0) {
            gameTheme.play();
        }
    }
}

