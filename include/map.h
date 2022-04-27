#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "NPCs.h"
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
        std::vector<NPC*> mapNPCs;
        std::vector<NPC*> NPCsinFront; // Only used for rendering NPCs that are in front of the players
    public:
        Map();
        ~Map();
        Music mapTheme; // TEMPORARILY A PUBLIC VARIABLE FOR EASY MUSIC TESTING
        void freeMap();
        void loadMap(const char* path, const char* sheetPath, const char* musicPath, double repeatP = 0.0);
        void loadNPCs(const char* npcData);
        void drawMap(gameCam* camera);
        void drawNPCs(gameCam* camera);
        void drawFrontNPCs(gameCam* camera);
        void playMapTheme();
        int getMapWidth();
        int getMapHeight();
        int** getCollisionMap();
        NPC* getNearbyNPC(int pCX, int pCY, int playerFace);
};