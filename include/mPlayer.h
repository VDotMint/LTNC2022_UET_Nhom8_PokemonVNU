#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class mPlayer {
    private:
        int gender;
        std::string name;
        int currentMap;
        int xCoords, yCoords;
    public:
        mPlayer();
        ~mPlayer();
        bool loadPlayerData();
        bool savePlayerData();
        int getGender();
        std::string getPlayerName();
        int getCurrentMap();
        int getXCoords();
        int getYCoords();
        void setPlayerCoords(int x, int y);
};