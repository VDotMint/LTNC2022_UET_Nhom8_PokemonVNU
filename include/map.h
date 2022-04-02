#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Map {
    private:
        int** map;
        int mapWidth;
        int mapHeight;
    public:
        Map();
        ~Map();
        void freeMap();
        void loadMap();

};