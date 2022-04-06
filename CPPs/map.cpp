#include "map.h"
#include "RenderWindow.h"
#include "tile.h"
#include "tilesheet.h"

TileSheet natureTileSheet;
TileSheet objectTileSheet;

int hardMap[11][13] = {
    {5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5},
    {7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7},
    {5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5},
    {7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7},
    {5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5},
    {7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Map::Map() {
    map = NULL;
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

void Map::loadMap() {
    natureTileSheet.loadTileSheet("res/tileset/tileset01.png");
    objectTileSheet.loadTileSheet("res/tileset/tileset02.png");
    map = new int*[11];
    for (int i = 0; i < 11; i++) {
        map[i] = new int[13];
    }
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 13; j++) {
            map[i][j] = hardMap[i][j];
        }
    }
}

void Map::drawMap() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 13; j++) {
            Tile drawTile;
            drawTile.defineTile(&natureTileSheet, map[i][j], 0);
            SDL_Rect dstRect;
            dstRect.x = j*64;
            dstRect.y = i*64;
            dstRect.w = 64;
            dstRect.h = 64;
            SDL_RenderCopy(RenderWindow::renderer, natureTileSheet.getTileSheet(), drawTile.getClip(), &dstRect);
        }
    }
}