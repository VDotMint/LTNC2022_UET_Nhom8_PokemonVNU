#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "music.h"
#include "camera.h"
#include "Tiling.h"

class Map {
    private:
        int** map; //Tile map
        int** tilePropMap; //Collision, Events Map
        int mapWidth;
        int mapHeight;
        TileSheet mapSheet;
        
    public:
        Map();
        ~Map();
        Music mapTheme; // TEMPORARILY A PUBLIC VARIABLE FOR EASY MUSIC TESTING
        void freeMap();
        void loadMap(const char* path, const char* sheetPath, const char* musicPath, double repeatP = 0.0);
        void drawMap(gameCam* camera);
        void playMapTheme();
        int getMapWidth();
        int getMapHeight();
        int** getCollisionMap();
};