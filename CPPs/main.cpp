#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "RenderWindow.h"
#include "music.h"
#include "tile.h"
#include "tilesheet.h"

using namespace std;

enum tileDesc {
    GRA,
    GR_E,
    L_SIGN,
    FLO,
    S_SIGN,
    TR1_0_0,
    TR1_0_1,
    TR1_1_0,
    TR1_1_1,
    TR2_0_0,
    TR2_1_0,
    HB1_2_0,
    HB1_2_1,
    HB1_2_2,
    HB1_1_0,
    HB1_1_2,
    HB1_0_0,
    HB1_0_2,
    TILE_TOTAL
};

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    RenderWindow window("Pokémon VNU", 832, 704);
    TileSheet natureSheet, objSheet;
    natureSheet.loadTileSheet("res/tileset/tileset01.png");
    objSheet.loadTileSheet("res/tileset/tileset02.png");
    Tile grass;
    grass.defineTile(&natureSheet, 0, 1);

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

        window.drawColor(255,0,0);
        window.clear();
        
        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 11; j++) {
                SDL_Rect dstRect;
                dstRect.x = i*64;
                dstRect.y = j*64;
                dstRect.w = 64;
                dstRect.h = 64;
                SDL_RenderCopy(RenderWindow::renderer, natureSheet.getTileSheet(), grass.getClip(), &dstRect);
            }
        }

        window.display();

        if (Mix_PlayingMusic() == 0) {
            gameTheme.play();
        }

    }

    window.close();
    return 0;
}