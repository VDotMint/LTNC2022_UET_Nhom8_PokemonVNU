#include "mPlayer.h"

#include <fstream>
using namespace std;

mPlayer::mPlayer() {
    gender = -1;
    name = "NO NAME";
    currentMap = -1;
    xCoords = 0, yCoords = 0;
}

mPlayer::~mPlayer() {
    
}

bool mPlayer::loadPlayerData() {
    bool success = true;
    ifstream playerDatInStream("data/player.sav");
    if (!playerDatInStream) {
        success = false;
    } else {
        getline(playerDatInStream, name);
        playerDatInStream >> gender >> currentMap >> xCoords >> yCoords;
        playerDatInStream.close();
    }
    return success;
}

bool mPlayer::savePlayerData() {
    bool success = true;
    ofstream playerDatOutStream("data/player.sav");
    if (playerDatOutStream) {
        playerDatOutStream << name << endl;
        playerDatOutStream << gender << endl << currentMap << endl << xCoords << " " << yCoords << endl;
    } else {
        success = false;
    }
    return success;
}

int mPlayer::getGender() {
    return gender;
}

std::string mPlayer::getPlayerName() {
    return name;
}

int mPlayer::getCurrentMap() {
    return currentMap;
}

int mPlayer::getXCoords() {
    return xCoords;
}

int mPlayer::getYCoords() {
    return yCoords;
}