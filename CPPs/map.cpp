#include <iostream>
#include <fstream>

#include "map.h"
#include "RenderWindow.h"
#include "Tiling.h"

using namespace std;

Map::Map() {
    map = NULL;
    tilePropMap = NULL; // 0 = walkable, 1 = not walkable, 2 = warp tile, 3 = interactibles
    mapWidth = 0;
    mapHeight = 0;
}

Map::~Map() {
    freeMap();
}

void Map::freeMap() {   
    for (int i = 0; i < mapHeight; i++) {
        delete[] tilePropMap[i];
    }
    delete[] tilePropMap;

    for (int i = 0; i < mapHeight; i++) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::loadMap(const char* path, const char* sheetPath, const char* musicPath, double repeatP) {
    // Load the level
    ifstream inputmap(path);
    string temp;
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
    inputmap >> temp;
    tilePropMap = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        tilePropMap[i] = new int[mapWidth];
    }
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            inputmap >> tilePropMap[i][j];
        }
    }
    inputmap.close();

    // Load the map's tilesheet
    mapSheet.loadTileSheet(sheetPath);
    mapTheme.loadMusic(musicPath, repeatP);
}

void Map::drawMap(gameCam* camera) {
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            Tile drawTile;
            drawTile.defineTile(&mapSheet, map[i][j]);
            SDL_Rect dstRect;
            dstRect.x = j*64 - camera->getCamX();
            dstRect.y = i*64 - camera->getCamY();
            dstRect.w = 64;
            dstRect.h = 64;
            SDL_RenderCopy(RenderWindow::renderer, mapSheet.getTileSheet(), drawTile.getClip(), &dstRect);
        }
    }
}

void Map::playMapTheme() {
    mapTheme.play();
}

int Map::getMapWidth() {
    return mapWidth;
}

int Map::getMapHeight() {
    return mapHeight;
}

int** Map::getCollisionMap() {
    return tilePropMap;
}