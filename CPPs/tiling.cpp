#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "Tiling.h"

SDL_Rect* Tile::getClip() {
    return &tileClip;
}

TileSheet::TileSheet() {
    tilesheet = NULL;
    sheetWidth = 0;
    sheetHeight = 0;
}

TileSheet::~TileSheet() {
    freeTileSheet();
}

void TileSheet::loadTileSheet(const char* path) {
    freeTileSheet();
    SDL_Surface* tempSurface = IMG_Load(path);
    tilesheet = SDL_CreateTextureFromSurface(RenderWindow::renderer, tempSurface);
    sheetWidth = tempSurface->w;
    sheetHeight = tempSurface->h;
    SDL_FreeSurface(tempSurface);
}

void TileSheet::freeTileSheet() {
    if (tilesheet != NULL) {
        SDL_DestroyTexture(tilesheet);
        tilesheet = NULL;
        sheetWidth = 0;
        sheetHeight = 0;
    }
}

SDL_Texture* TileSheet::getTileSheet() {
    return tilesheet;
}

int TileSheet::getWidth() {
    return sheetWidth;
}

int TileSheet::getHeight() {
    return sheetHeight;
}

Tile::Tile() {
    tileClip = {0, 0, 0, 0};
}

Tile::~Tile() {
    tileClip = {0, 0, 0, 0};
}

void Tile::defineTile(TileSheet* sheet, int index) {
    if (index > (sheet->getWidth()/sheet->getHeight())) {
        std::cout << "Sheet Index Out Of Bound!\n"; 
    } else {
        tileClip.x = index * sheet->getHeight();
        tileClip.y = 0;
        tileClip.w = sheet -> getHeight();
        tileClip.h = sheet -> getHeight();
    }
}