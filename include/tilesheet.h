#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TileSheet {
    private:
        SDL_Texture* tilesheet;
        int sheetWidth;
        int sheetHeight;
    public:
        TileSheet();
        ~TileSheet();
        void loadTileSheet(const char* path);
        void freeTileSheet();
        SDL_Texture* getTileSheet();
        int getWidth();
        int getHeight();
};