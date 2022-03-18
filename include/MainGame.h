#pragma once
#include "RenderWindow.h"
#include <iostream>

class MainGame {
public:
    MainGame();
    ~MainGame();
    void run();
    void initSystem();
    void gameLoop();
    void inputProcess();
    void updateGame();
    void renderGame();
    void delay();
private:
    RenderWindow renderWindow;
    bool gameState;

};