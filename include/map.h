#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "camera.h"
#include "tile.h"

class Map {
    private:
        int** map; //Tile map
        int** tilePropMap; //Collision, Events Map
        int mapWidth;
        int mapHeight;
    public:
        Map();
        ~Map();
        void freeMap();
        void loadMap(const char* path);
        void drawMap(TileSheet* sheet, gameCam* camera);
        int getMapWidth();
        int getMapHeight();
        int** getCollisionMap();
};