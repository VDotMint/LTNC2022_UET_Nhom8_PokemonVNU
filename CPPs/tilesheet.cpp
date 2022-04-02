#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.h"
#include "tilesheet.h"

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