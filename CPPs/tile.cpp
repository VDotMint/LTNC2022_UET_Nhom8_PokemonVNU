#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "tile.h"

Tile::Tile() {
    tileClip = {0, 0, 0, 0};
    isWalkable = 0; 
}

Tile::~Tile() {
    tileClip = {0, 0, 0, 0};
    isWalkable = 0; 
}

void Tile::defineTile(TileSheet* sheet, int index, bool walk) {
    if (index > (sheet->getWidth()/sheet->getHeight())) {
        std::cout << "Sheet Index Out Of Bound!\n"; 
    } else {
        tileClip.x = index * sheet->getHeight();
        tileClip.y = 0;
        tileClip.w = sheet -> getHeight();
        tileClip.h = sheet -> getHeight();
        isWalkable = walk;
    }
}

SDL_Rect* Tile::getClip() {
    return &tileClip;
}