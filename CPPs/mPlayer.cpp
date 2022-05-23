#include "mPlayer.h"
#include "RenderWindow.h"
#include "Battle.h"

#include <iostream>
#include <fstream>

SDL_Rect screenCenter = {(832-64)/2, (704-64)/2-24, 64, 88};
static int moveFrame = 0, walkFrame = 0;

mPlayer::mPlayer() {
    gender = 0; // -1 = UNDEFINED, 0 = MALE, 1 = FEMALE
    name = "Player";
    currentMap = 1; // 0 = G2 EXTERIOR, 1 = E3 EXTERIOR, 2 = E3 INTERIOR, 3 = G2 INTERIOR, 4 = STUDENT BUTTON ROOM , 5 -> 10 = CHALLENGE ROOMS
    xCoords = 20, yCoords = 10;
    faceDirection = 0; // 0 = SOUTH, 1 = EAST, 2 = NORTH, 3 = WEST
    playerTexture = NULL;
    currentHighScore = 0;

    for (int i = 0; i < 32; i++) walkFrames[i] = {i*64, 0, 64, 88};
    for (int i = 0; i < 3; i++) party[i] = 0;
}

mPlayer::~mPlayer() {
    SDL_DestroyTexture(playerTexture);
    playerTexture = NULL;
    playerScoreList.freeHighScoreList();
}

bool mPlayer::loadPlayerData() {
    bool success = true;
    std::ifstream playerDatInStream("data/player.sav");

    playerScoreList.initHighScoreList();

    if (!playerDatInStream) {
        std::cout << "No save file detected! Default player config loaded instead!\n";
        success = false;
    } else {
        getline(playerDatInStream, name); // NAME
        playerDatInStream >> gender >> currentMap >> xCoords >> yCoords; // PLAYER LOCATION

        // PLAYER POKEMON DATA
        int party0, party1, party2;
        playerDatInStream >> party0 >> party1 >> party2;
        party[0] = party0, party[1] = party1, party[2] = party2;

        // PLAYER HIGH SCORE DATA
        int highScores[5];
        playerDatInStream >> currentHighScore;
        for (int i = 0; i < 5; i++) playerDatInStream >> highScores[i];
        playerScoreList.loadHighScoreList(highScores);

        playerDatInStream.close();
    }

    return success;
}

bool mPlayer::savePlayerData() {
    using namespace std;
    bool success = true;
    ofstream playerDatOutStream("data/player.sav");
    if (playerDatOutStream) {
        playerDatOutStream << name << endl;
        playerDatOutStream << gender << endl << currentMap << endl << xCoords << " " << yCoords << endl;
        playerDatOutStream << party[0].data - pokemonData << " " << party[1].data - pokemonData << " " << party[2].data - pokemonData << endl;
        playerDatOutStream << currentHighScore << endl;
        playerScoreList.saveHighScoreList(playerDatOutStream);
    } else {
        success = false;
    }
    return success;
}

void mPlayer::resetPlayerData() {
    gender = 0; // -1 = UNDEFINED, 0 = MALE, 1 = FEMALE
    name = "Player";
    currentMap = 1; // 0 = G2 EXTERIOR, 1 = E3 EXTERIOR, 2 = E3 INTERIOR, 3 = G2 INTERIOR, 4 = STUDENT BUTTON ROOM , 5 -> 10 = CHALLENGE ROOMS
    xCoords = 20, yCoords = 10;
    faceDirection = 0;
    currentHighScore = 0;
    playerScoreList.resetHighScoreList();
    SDL_DestroyTexture(playerTexture);
    initPlayerTexture();
    party[0] = 0;
    party[1] = 0;
    party[2] = 0;
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

// HIGH SCORE SYSTEM

void swap(int &a, int &b) {
    int c;
    c = a;
    a = b;
    b = c;
}

HighScoreList::HighScoreList() {}

HighScoreList::~HighScoreList() {}

void HighScoreList::initHighScoreList() {
    head = new HighScoreListNode(0);
    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 4; i++) {
        HighScoreListNode* nextNode = new HighScoreListNode(0);
        iterNode->nextHighScore = nextNode;
        iterNode = nextNode;
    }

    highScoreScreenText = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/pokemon_sel_screen.png");
    highScoreScreenRect = {91, 152, 650, 400};

    for (int i = 0; i < 5; i++) highScoreTexts[i].createFont("res/font/gamefont.ttf", 28);

    backButton.initMB("res/otherassets/backButton.png", 592, 473);
}

void HighScoreList::freeHighScoreList() {
    HighScoreListNode* p = head;
    while (p != NULL) {
        HighScoreListNode* p1 = p;
        p = p->nextHighScore;
        delete p1;
    }

    SDL_DestroyTexture(highScoreScreenText);
    for (int i = 0; i < 5; i++) highScoreTexts[i].freeText();
    backButton.~MenuButton();
}

void HighScoreList::loadHighScoreList(int highScoreList[]) {
    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 5; i++) {
        iterNode->score = highScoreList[i];
        iterNode = iterNode->nextHighScore;
    }
}

void HighScoreList::printHighScoreList() {
    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 5; i++) {
        std::cout << iterNode->score << std::endl;
        iterNode = iterNode->nextHighScore;
    }
}

void HighScoreList::saveHighScoreList(std::ofstream& saveStream) {
    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 5; i++) {
        saveStream << iterNode->score << " ";
        iterNode = iterNode->nextHighScore;
    }
    saveStream << std::endl;
}

void HighScoreList::updateHighScoreList(int newHighScore) {
    HighScoreListNode* iterNode = head;
    HighScoreListNode* lastElement = NULL;
    bool insert = false;

    for (int i = 0; i < 5; i++) {
        if (newHighScore > iterNode->score && insert == false) {
            insert = true;
        }
        lastElement = iterNode;
        iterNode = iterNode->nextHighScore;
    }

    if (insert == true) {
        lastElement->score = newHighScore;

        HighScoreListNode* p1 = head;
        HighScoreListNode* p2 = NULL;
        while (p1 != NULL) {
            p2 = p1->nextHighScore;
            while (p2 != NULL) {
                if (p2->score > p1->score) {
                    swap(p1->score, p2->score);
                }
                p2 = p2->nextHighScore;
            }
            p1 = p1->nextHighScore;
        }
    } 
}

void HighScoreList::resetHighScoreList() {
    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 5; i++) {
        iterNode->score = 0;
        iterNode = iterNode->nextHighScore;
    }
}

void HighScoreList::drawHighScoreScreen() {
    SDL_RenderCopy(RenderWindow::renderer, highScoreScreenText, NULL, &highScoreScreenRect);

    HighScoreListNode* iterNode = head;
    for (int i = 0; i < 5; i++) {
        highScoreTexts[i].textInit(RenderWindow::renderer, (to_string(i+1) + ". " + to_string(iterNode->score)).c_str(), {0,0,0});
        iterNode = iterNode->nextHighScore;
    }

    highScoreTexts[0].display(155, 217, RenderWindow::renderer);
    highScoreTexts[1].display(155, 270, RenderWindow::renderer);
    highScoreTexts[2].display(155, 323, RenderWindow::renderer);
    highScoreTexts[3].display(155, 376, RenderWindow::renderer);
    highScoreTexts[4].display(155, 429, RenderWindow::renderer);

    backButton.drawButton();
}