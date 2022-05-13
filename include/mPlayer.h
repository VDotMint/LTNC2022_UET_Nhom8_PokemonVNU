#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Pokemon.h"

extern Pokemon pokemon[];

class mPlayer {
    private:
        int gender;
        std::string name;
        int currentMap;
        int xCoords, yCoords;
        int faceDirection;
        SDL_Texture* playerTexture;
        SDL_Rect walkFrames[32];
        
    public:
        mPlayer();
        ~mPlayer();
        Pokemon party[3];
        int activePokemonCount;
        bool loadPlayerData();
        bool savePlayerData();
        int getGender();
        std::string getPlayerName();
        int getCurrentMap();
        int getXCoords();
        int getYCoords();
        void changeFacingDirect(int direct);
        int getFacingDirection();
        void initPlayerTexture();
        SDL_Texture* getPlayerTexture();
        SDL_Rect* getCurrentFrame();
        void setPlayerCoords(int x, int y, int mapID);
        void renderStandingPlayer();
        void renderMovingPlayer();
        void renderRunningPlayer();
};