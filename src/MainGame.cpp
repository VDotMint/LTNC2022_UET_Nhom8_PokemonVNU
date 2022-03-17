#include "MainGame.h"

MainGame::MainGame() {
    gameState=true;
};

MainGame::~MainGame(){

};

void MainGame::run(){
	renderWindow.initSystem();
	gameLoop();
};



void MainGame::gameLoop(){
	int fps=24;
    while(gameState) {
		inputProcess();
		SDL_Delay(1000/fps);
	}
	
};

void MainGame::inputProcess() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			gameState=false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			std::cout<<event.motion.x<<" "<<event.motion.y<<std::endl;
			break;
		default:
			break;
		}
	};
};

