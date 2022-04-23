#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TileSheet {
    private:
        SDL_Texture* tilesheet;
        int sheetColumns, sheetRows;
    public:
        TileSheet();
        ~TileSheet();
        void loadTileSheet(const char* path);
        void freeTileSheet();
        SDL_Texture* getTileSheet();
        int getSheetWidth();
        int getSheetHeight();
};

class Tile {
    private:
        SDL_Rect tileClip;
    public:
        Tile();
        ~Tile();
        void defineTile(TileSheet* sheet, int index);
        SDL_Rect* getClip();
};