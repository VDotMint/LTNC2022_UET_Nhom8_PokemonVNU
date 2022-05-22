#include "mPlayer.h"
#include "RenderWindow.h"
#include "Battle.h"

#include <iostream>
#include <fstream>

SDL_Rect screenCenter = {(832-64)/2, (704-64)/2-24, 64, 88};
static int moveFrame = 0, walkFrame = 0;

mPlayer::mPlayer() {
    gender = 0; // -1 = UNDEFINED, 0 = MALE, 1 = FEMALE
    name = "NO NAME";
    currentMap = 1; // 0 = G2 EXTERIOR, 1 = E3 EXTERIOR, 2 = E3 INTERIOR, 3 = G2 INTERIOR, 4 = STUDENT BUTTON ROOM , 5 -> 10 = CHALLENGE ROOMS
    xCoords = 20, yCoords = 10;
    faceDirection = 0; // 0 = SOUTH, 1 = EAST, 2 = NORTH, 3 = WEST
    playerTexture = NULL;
    currentHighScore = 0;

    for (int i = 0; i < 32; i++) {
        walkFrames[i] = {i*64, 0, 64, 88};
    }

    for (int i = 0; i < 3; i++) party[i] = 0;
}

mPlayer::~mPlayer() {
    SDL_DestroyTexture(playerTexture);
    playerTexture = NULL;
}

bool mPlayer::loadPlayerData() {
    bool success = true;
    std::ifstream playerDatInStream("data/player.sav");
    if (!playerDatInStream) {
        std::cout << "No save file detected! Default player config loaded instead!\n";
        success = false;
    } else {
        getline(playerDatInStream, name);
        playerDatInStream >> gender >> currentMap >> xCoords >> yCoords;
        int party0, party1, party2;
        playerDatInStream >> party0 >> party1 >> party2;
        party[0] = party0, party[1] = party1, party[2] = party2;
        playerDatInStream >> currentHighScore;
        playerDatInStream.close();
    }
    return success;
}

bool mPlayer::savePlayerData() {
    bool success = true;
    std::ofstream playerDatOutStream("data/player.sav");
    if (playerDatOutStream) {
        playerDatOutStream << name << endl;
        playerDatOutStream << gender << endl << currentMap << endl << xCoords << " " << yCoords << endl;
        playerDatOutStream << party[0].data - pokemonData << " " << party[1].data - pokemonData << " " << party[2].data - pokemonData << endl;
        playerDatOutStream << currentHighScore << endl;
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
        std::cout << "Player gender uninitiated!\n";
    } else {
        SDL_Surface* tempSurface;
        if (gender == 0) {
            tempSurface = IMG_Load("res/playersprite/malesprite.png");
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

void mPlayer::setPlayerCoords(int x, int y, int mapID) {
    xCoords = x;
    yCoords = y;
    currentMap = mapID;
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

void mPlayer::renderRunningPlayer() {
    moveFrame++;
    if (moveFrame > 60) moveFrame = 1;
    if (moveFrame % 5 == 0) walkFrame++;
    if (walkFrame > 3) walkFrame = 0;
    switch (faceDirection) {
        case 0:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[16+walkFrame], &screenCenter);
            break;
        case 1:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[20+walkFrame], &screenCenter);
            break;
        case 2:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[24+walkFrame], &screenCenter);
            break;
        case 3:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[28+walkFrame], &screenCenter);
            break;
        default:
            SDL_RenderCopy(RenderWindow::renderer, playerTexture, &walkFrames[0], &screenCenter);
            break;
    }
}