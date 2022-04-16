#include <fstream>

#include "map.h"
#include "RenderWindow.h"
#include "tile.h"
#include "tilesheet.h"

using namespace std;

Map::Map() {
    map = NULL;
    mapWidth = 0;
    mapHeight = 0;
}

Map::~Map() {
    freeMap();
}

void Map::freeMap() {
    for (int i = 0; i < mapHeight; i++) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::loadMap(const char* path) {
    ifstream inputmap(path);
    inputmap >> mapHeight;
    inputmap >> mapWidth;

    map = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new int[mapWidth];
    }

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            inputmap >> map[i][j];
        }
    }
    inputmap.close();    
}

void Map::drawMap(TileSheet* sheet, gameCam* camera) {
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            Tile drawTile;
            drawTile.defineTile(sheet, map[i][j]);
            SDL_Rect dstRect;
            dstRect.x = j*64 - camera->getCamX();
            dstRect.y = i*64 - camera->getCamY();
            dstRect.w = 64;
            dstRect.h = 64;
            SDL_RenderCopy(RenderWindow::renderer, sheet->getTileSheet(), drawTile.getClip(), &dstRect);
        }
    }
}

int Map::getMapWidth() {
    return mapWidth;
}

int Map::getMapHeight() {
    return mapHeight;
}