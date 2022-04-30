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
    for (unsigned int i = 0; i < NPCsinFront.size(); i++) {
        delete mapNPCs[i];
    }
}

void Map::loadMap(const char* path, const char* sheetPath, const char* musicPath, double repeatP) {    
    // Load the level
    ifstream inputmap(path);
    string temp;
    inputmap >> mapID;
    inputmap >> mapHeight;
    inputmap >> mapWidth;

    // Load in the tile texture data
    map = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new int[mapWidth];
    }
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            inputmap >> map[i][j];
        }
    }

    // Flush the end tile texture flag
    inputmap >> temp;

    // Load in the tile collision data
    tilePropMap = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        tilePropMap[i] = new int[mapWidth];
    }
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            inputmap >> tilePropMap[i][j];
        }
    }

    // Flush the tile collision flag
    inputmap >> temp;

    // Reads the NPC data
    int i = 0;
    string nextNPC;
    while (i < 1) {
        i++;
        inputmap >> nextNPC;
        if (nextNPC != "MAP_NEXT_NPC" or nextNPC == "NPC_DATA_STOP") {
            break;
        } else {
            int npcX, npcY, face;
            string npcTextPath;
            inputmap >> npcX >> npcY >> face >> npcTextPath;

            NPC* newNPC = new NPC;
            newNPC->initNPC(npcX, npcY, face, npcTextPath.c_str());
            
            string dialogueSentence;
            while (dialogueSentence != "NPC_DIALOGUE_END") {
                getline(inputmap, dialogueSentence);
                if (dialogueSentence != "NPC_DIALOGUE_END" and dialogueSentence != "") {
                    newNPC->initDialogue(dialogueSentence);
                }
            }

            mapNPCs.push_back(newNPC);

            tilePropMap[npcY][npcX] = 4;
            i--;
        }
    }

    inputmap >> temp;
    cout << temp << endl;

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

void Map::drawNPCs(gameCam* camera) {
    NPCsinFront.clear();
    for (unsigned int i = 0; i < mapNPCs.size(); i++) {
        if (camera->getCamY() + 64*5 < mapNPCs[i]->getY()*64) {
            NPCsinFront.push_back(mapNPCs[i]);
        } else {
            mapNPCs[i]->drawNPC(camera->getCamX(), camera->getCamY());
        }
    }
}

void Map::drawFrontNPCs(gameCam* camera) {
    for (unsigned int i = 0; i < NPCsinFront.size(); i++) {
        NPCsinFront[i]->drawNPC(camera->getCamX(), camera->getCamY());
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

NPC* Map::getNearbyNPC(int pCX, int pCY, int playerFace) {
    for (unsigned int i = 0; i < mapNPCs.size(); i++) {
        switch (playerFace) {
        case 0:
            if (mapNPCs[i]->getY() == pCY + 1 and mapNPCs[i]->getX() == pCX) return mapNPCs[i];
            break;
        case 1:
            if (mapNPCs[i]->getX() == pCX + 1 and mapNPCs[i]->getY() == pCY) return mapNPCs[i];
            break;
        case 2:
            if (mapNPCs[i]->getY() == pCY - 1 and mapNPCs[i]->getX() == pCX) return mapNPCs[i];
            break;
        case 3:
            if (mapNPCs[i]->getX() == pCX - 1 and mapNPCs[i]->getY() == pCY) return mapNPCs[i];
            break;
        default:
            break;
        }
    }
    
    return NULL;
}

WarpTile::WarpTile(int _tileX, int _tileY, int _destMap, int _destX, int _destY) {
    tileX = _tileX;
    tileY = _tileY;
    destMap = _destMap;
    destX = _destX;
    destY = _destY;
}

WarpTile::~WarpTile() {
    tileX = 0;
    tileY = 0;
    destMap = 0;
    destX = 0;
    destY = 0;
}

void WarpTile::activateWarpTile() {

}