#include "mPlayer.h"
#include "RenderWindow.h"

#include <iostream>
#include <fstream>
using namespace std;

SDL_Rect screenCenter = {(832-64)/2, (704-64)/2-22, 64, 88};
int moveFrame = 0, walkFrame = 0;

mPlayer::mPlayer() {
    gender = -1; // -1 = UNDEFINED, 0 = MALE, 1 = FEMALE
    name = "NO NAME";
    currentMap = -1;
    xCoords = 0, yCoords = 0;
    faceDirection = 0; // 0 = SOUTH, 1 = EAST, 2 = NORTH, 3 = WEST
    playerTexture = NULL;
    for (int i = 0; i < 16; i++) {
        walkFrames[i] = {i*64, 0, 64, 88};
    }
}

mPlayer::~mPlayer() {
    SDL_DestroyTexture(playerTexture);
    playerTexture = NULL;
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
        playerDatOutStream << faceDirection << endl;
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

int mPlayer::getFacingDirection() {
    return faceDirection;
}

void mPlayer::initPlayerTexture() {
    if (gender == -1) {
        cout << "Player gender uninitiated!\n";
    } else {
        SDL_Surface* tempSurface;
        if (gender == 0) {
            tempSurface = IMG_Load("res/playersprite/femalesprite.png");
            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 255, 255));
            playerTexture = SDL_CreateTextureFromSurface(RenderWindow::renderer, tempSurface);
        } else {
            tempSurface = IMG_Load("res/playersprite/femalesprite.png");
            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 255, 255));
            playerTexture = SDL_CreateTextureFromSurface(RenderWindow::renderer, tempSurface);
        }
        SDL_FreeSurface(tempSurface);
    }
}

void mPlayer::setPlayerCoords(int x, int y) {
    xCoords = x;
    yCoords = y;
}

void mPlayer::changeFacingDirect(int direct) {
    faceDirection = direct;
}

void mPlayer::renderStandingPlayer() {
    switch (faceDirection) {
        case 0:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[0], &screenCenter);
            break;
        case 1:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[4], &screenCenter);
            break;
        case 2:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[8], &screenCenter);
            break;
        case 3:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[12], &screenCenter);
            break;
        default:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[0], &screenCenter);
            break;
    }
}

void mPlayer::renderMovingPlayer() {
    moveFrame++;
    if (moveFrame > 60) moveFrame = 1;
    if (moveFrame % 10 == 0) walkFrame++;
    if (walkFrame > 3) walkFrame = 0;
    switch (faceDirection) {
        case 0:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[0+walkFrame], &screenCenter);
            break;
        case 1:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[4+walkFrame], &screenCenter);
            break;
        case 2:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[8+walkFrame], &screenCenter);
            break;
        case 3:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[12+walkFrame], &screenCenter);
            break;
        default:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[0], &screenCenter);
            break;
    }
}