#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "tilesheet.h"

class Tile {
    private:
        int tID;
        bool isWalkable;
        SDL_Rect tileClip;
    public:
        Tile();
        ~Tile();
        void defineTile(TileSheet* sheet, int index, bool walk);
        SDL_Rect* getClip();
};