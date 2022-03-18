#include "MainGame.h"

MainGame::MainGame() {
	gameState=false;
};

MainGame::~MainGame(){

};

void MainGame::run(){
	renderWindow.initSystem();
	gameState=true;
	gameLoop();
};



void MainGame::gameLoop(){
    while(gameState) {
		inputProcess();
		updateGame();
		renderGame();
		delay();
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

void MainGame::updateGame() {

};
void MainGame::renderGame(){

};
void MainGame::delay(){
	const int ph_fps=24;
	SDL_Delay(1000/ph_fps);
};


