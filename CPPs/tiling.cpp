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
}

TileSheet::~TileSheet() {
    freeTileSheet();
}

void TileSheet::loadTileSheet(const char* path) {
    freeTileSheet();
    SDL_Surface* tempSurface = IMG_Load(path);
    tilesheet = SDL_CreateTextureFromSurface(RenderWindow::renderer, tempSurface);
    sheetRows = tempSurface->h / 16;
    sheetColumns = tempSurface->w / 16;
    SDL_FreeSurface(tempSurface);
}

void TileSheet::freeTileSheet() {
    if (tilesheet != NULL) {
        SDL_DestroyTexture(tilesheet);
        tilesheet = NULL;
    }
}

SDL_Texture* TileSheet::getTileSheet() {
    return tilesheet;
}

int TileSheet::getSheetWidth() {
    return sheetColumns;
}

int TileSheet::getSheetHeight() {
    return sheetRows;
}

// TILE

Tile::Tile() {
    tileClip = {0, 0, 0, 0};
}

Tile::~Tile() {
    tileClip = {0, 0, 0, 0};
}

void Tile::defineTile(TileSheet* sheet, int index) {
    int coordsX = 16 * (index % sheet->getSheetWidth());
    int coordsY = 16 * (index / sheet->getSheetWidth());
    
    tileClip.x = coordsX;
    tileClip.y = coordsY;
    tileClip.w = 16;
    tileClip.h = 16;
}