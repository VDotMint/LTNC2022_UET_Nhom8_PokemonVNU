#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "tilesheet.h"

class Tile {
    private:
        SDL_Rect tileClip;
    public:
        Tile();
        ~Tile();
        void defineTile(TileSheet* sheet, int index);
        SDL_Rect* getClip();
};