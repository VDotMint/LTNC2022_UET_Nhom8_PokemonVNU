#include <iostream>
#include <fstream>

#include "NPCs.h"
#include "map.h"
#include "RenderWindow.h"
#include "Tiling.h"

using namespace std;

bool mapDebug = true;

TileSheet overlayElements;

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

    if (mapDebug == true) cerr << "Freeing procedure started\n";

    for (int i = 0; i < mapHeight; i++) {
        delete[] tilePropMap[i];
    }
    delete[] tilePropMap;
    tilePropMap = NULL;

    if (mapDebug == true) cerr << "Collision Map Freed Properly\n";

    for (int i = 0; i < mapHeight; i++) {
        delete[] map[i];
    }
    delete[] map;
    map = NULL;

    if (mapDebug == true) cerr << "Texture Map Freed Properly\n";

    if (tileOverlayMap != NULL) {
        for (int i = 0; i < mapHeight; i++) {
            delete[] tileOverlayMap[i];
        }
        delete[] tileOverlayMap;
        tileOverlayMap = NULL;
    }

    if (mapDebug == true) cerr << "Overlay Map Freed Properly\n";

    NPCsinFront.clear();
    for (unsigned int i = 0; i < mapNPCs.size(); i++) {
        delete mapNPCs[i];
    }
    mapNPCs.clear();

    if (mapDebug == true) cerr << "Map NPCs Freed Properly\n";

    for (unsigned int i = 0; i < mapWarpTiles.size(); i++) {
        delete mapWarpTiles[i];
    }
    mapWarpTiles.clear();

    if (mapDebug == true) cerr << "Map Warp Tiles Freed Properly\n";

    for (unsigned int i = 0; i < mapInterTiles.size(); i++) {
        delete mapInterTiles[i];
    }
    mapInterTiles.clear();

    if (mapDebug == true) cerr << "Map Inter Tiles Freed Properly\n";

    mapWidth = 0;
    mapHeight = 0;
    mapSheet.freeTileSheet();

    if (mapDebug == true) cerr << "Tilesheet Freed Successfully\n";

    mapTheme.freeMusic();

    if (mapDebug == true) cerr << "Music Freed Successfully\n";

    if (mapDebug == true) cerr << "Map Freed Successfully\n\n";
}

void Map::loadMap(const char* path, const char* sheetPath, const char* musicPath, double repeatP, bool hasOverlay) {    

    if (mapDebug == true) cerr << "Loading procedure started\n";

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

    if (mapDebug == true) cerr << "Tile texture data loaded\n";

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

    if (mapDebug == true) cerr << "Tile collision data loaded\n";

    // Flush the tile collision flag
    inputmap >> temp;

    // Reads the tile overlay map if it exists
    if (hasOverlay == true) {
        tileOverlayMap = new int*[mapHeight];
        for (int i = 0; i < mapHeight; i++) {
            tileOverlayMap[i] = new int[mapWidth];
        }
        for (int i = 0; i < mapHeight; i++) {
            for (int j = 0; j < mapWidth; j++) {
                inputmap >> tileOverlayMap[i][j];
            }
        }

        if (mapDebug == true) cerr << "Tile overlay data loaded\n";

        // Flush the tile overlay flag
        inputmap >> temp;
    }

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

    if (mapDebug == true) cerr << "NPC data loaded\n";

    i = 0;
    string nextWarpTile;
    while (i < 1) {
        i++;
        inputmap >> nextWarpTile;
        if (nextWarpTile != "WARP_NEXT_TILE" or nextWarpTile == "WARP_DATA_STOP") {
            break;
        } else {
            int tileX, tileY, destMap, destX, destY;
            inputmap >> tileX >> tileY >> destMap >> destX >> destY;

            WarpTile* newWarpTile = new WarpTile(tileX, tileY, destMap, destX, destY);
            
            mapWarpTiles.push_back(newWarpTile);
            tilePropMap[tileY][tileX] = 2;
            i--;
        }
    }

    if (mapDebug == true) cerr << "Warp tile data loaded\n";

    i = 0;
    string nextInterTile;
    while (i < 1) {
        i++;
        inputmap >> nextInterTile;
        if (nextInterTile != "INTER_NEXT_TILE" or nextInterTile == "INTER_DATA_STOP") {
            break;
        } else {
            int tileX, tileY;
            inputmap >> tileX >> tileY;

            InterTile* newInterTile = new InterTile(tileX, tileY);

            string interSentence;
            while (interSentence != "INTER_DIALOGUE_END") {
                getline(inputmap, interSentence);
                if (interSentence != "INTER_DIALOGUE_END" and interSentence != "") {
                    newInterTile->initTileDialogue(interSentence);
                }
            }

            mapInterTiles.push_back(newInterTile);
            tilePropMap[tileY][tileX] = 3;
            i--;
        }
    }

    if (mapDebug == true) cerr << "Inter tile data loaded\n";

    inputmap.close();

    // Load the map's tilesheet
    mapSheet.loadTileSheet(sheetPath);
    mapTheme.loadMusic(musicPath, repeatP);

    if (mapDebug == true) cerr << "Everything loaded\n----------------------------------------------\n";
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

void Map::initOverlayElements(const char* path) {
    overlayElements.loadTileSheet(path);
}

void Map::freeOverlayElements() {
    overlayElements.freeTileSheet();
}

void Map::drawOverlay(gameCam* camera) {
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (tileOverlayMap[i][j] != -1) {
                Tile overlayTile;
                overlayTile.defineTile(&overlayElements, tileOverlayMap[i][j]);
                SDL_Rect dstRect;
                dstRect.x = j*64 - camera->getCamX();
                dstRect.y = i*64 - camera->getCamY();
                dstRect.w = 64;
                dstRect.h = 64;
                SDL_RenderCopy(RenderWindow::renderer, overlayElements.getTileSheet(), overlayTile.getClip(), &dstRect);
            }
        }
    }
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

WarpTile* Map::getNearbyWarpTile(int pCX, int pCY, int playerFace) {
    for (unsigned int i = 0; i < mapWarpTiles.size(); i++) {
        switch (playerFace) {
        case 0:
            if (mapWarpTiles[i]->getY() == pCY + 1 and mapWarpTiles[i]->getX() == pCX) return mapWarpTiles[i];
            break;
        case 1:
            if (mapWarpTiles[i]->getX() == pCX + 1 and mapWarpTiles[i]->getY() == pCY) return mapWarpTiles[i];
            break;
        case 2:
            if (mapWarpTiles[i]->getY() == pCY - 1 and mapWarpTiles[i]->getX() == pCX) return mapWarpTiles[i];
            break;
        case 3:
            if (mapWarpTiles[i]->getX() == pCX - 1 and mapWarpTiles[i]->getY() == pCY) return mapWarpTiles[i];
            break;
        default:
            break;
        }
    }
    
    return NULL;
}

InterTile* Map::getNearbyInterTile(int pCX, int pCY, int playerFace) {
    for (unsigned int i = 0; i < mapInterTiles.size(); i++) {
        switch (playerFace) {
        case 0:
            if (mapInterTiles[i]->getY() == pCY + 1 and mapInterTiles[i]->getX() == pCX) return mapInterTiles[i];
            break;
        case 1:
            if (mapInterTiles[i]->getX() == pCX + 1 and mapInterTiles[i]->getY() == pCY) return mapInterTiles[i];
            break;
        case 2:
            if (mapInterTiles[i]->getY() == pCY - 1 and mapInterTiles[i]->getX() == pCX) return mapInterTiles[i];
            break;
        case 3:
            if (mapInterTiles[i]->getX() == pCX - 1 and mapInterTiles[i]->getY() == pCY) return mapInterTiles[i];
            break;
        default:
            break;
        }
    }
    
    return NULL;
}

// WARP TILE CONFIGS

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

// INTER TILE CONFIGS

static unsigned int INTERsen = 0;

InterTile::InterTile(int _tileX, int _tileY) {
    tileX = _tileX;
    tileY = _tileY;
}

InterTile::~InterTile() {
    tileX = 0;
    tileY = 0;
    dialogueTexts.clear();
}

void InterTile::initTileDialogue(std::string nextSentence) {
    dialogueTexts.push_back(nextSentence);
}

bool InterTile::talkTile() {
    if (INTERsen < dialogueTexts.size()) {
        std::cout << dialogueTexts[INTERsen] << std::endl;
        if (INTERsen + 1 == dialogueTexts.size()) {
            INTERsen = 0;
            return false;
        } else {
            INTERsen++;
            return true;
        }
    } else {
        return false;
    }
}
