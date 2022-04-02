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

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    RenderWindow window("Pokémon VNU", 832, 704);

    Map testMap;
    testMap.loadMap();

    Music gameTheme;

    gameTheme.loadMusic("res/music/fridaynight.mp3");

    bool quit = false;

    SDL_Event e;

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        window.drawColor(0,0,0);
        window.clear();
        
        testMap.drawMap();

        window.display();

        if (Mix_PlayingMusic() == 0) {
            gameTheme.play();
        }

    }

    window.close();
    return 0;
}