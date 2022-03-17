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
private:
    RenderWindow renderWindow;
    bool gameState;

};