#include <iostream>
#include <fstream>

#include "NPCs.h"
#include "map.h"
#include "RenderWindow.h"
#include "Tiling.h"

using namespace std;

Map::Map() {
    map = NULL;
    tilePropMap = NULL; // 0 = walkable, 1 = not walkable, 2 = warp tile, 3 = interactable, 4 = hasNPC
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
    for (unsigned int i = 0; i < mapNPCs.size(); i++) {
        delete mapNPCs[i];
    }
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

void Map::loadNPCs(const char* npcData) {
    ifstream npcmap(npcData);
    int i = 0;
    string nextNPC;
    while (i < 1) {
        i++;
        npcmap >> nextNPC;
        if (nextNPC != "MAP_NEXT_NPC") {
            break;
        } else {
            int npcX, npcY, face;
            string npcTextPath;
            npcmap >> npcX >> npcY >> face >> npcTextPath;
            NPC* newNPC = new NPC;
            newNPC->initNPC(npcX, npcY, face, npcTextPath.c_str());
            mapNPCs.push_back(newNPC);
            tilePropMap[npcY][npcX] = 4;
            i--;
        }
    }
    npcmap.close();
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
    for (unsigned int i = 0; i < mapNPCs.size(); i++) {
        mapNPCs[i]->drawNPC(camera->getCamX(), camera->getCamY());
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