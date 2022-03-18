#include "RenderWindow.h"

RenderWindow::RenderWindow() {
    window=nullptr;
	renderer=nullptr;
};

void RenderWindow::initSystem(){
 	if (SDL_Init(SDL_INIT_EVERYTHING)<0) {
 		std::cerr<<"SDL_Init Error"<<SDL_GetError()<<std::endl;
 	}
 	if (!(IMG_Init(IMG_INIT_PNG))) {
 		std::cerr<<"IMG_Init Error"<<SDL_GetError()<<std::endl;
	}
    window=SDL_CreateWindow("VNUmon",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);
	if (window==nullptr) std::cerr<<"CreateWindow Error";
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	if (renderer==nullptr) std::cerr<<"CreateRenderer Error";
	
	render("res/hello_world.bmp");
};

void RenderWindow::render(const char* path) {
	SDL_Rect dst={0,0,64*4,64*4};
	SDL_Texture* texture=loadTexture(path);
	SDL_RenderCopy(renderer,texture,NULL,&dst);
	SDL_RenderPresent(renderer);
};


SDL_Texture* RenderWindow::loadTexture(const char* path){
	SDL_Texture* texture=nullptr;
	texture=IMG_LoadTexture(renderer,path);
	if (texture==nullptr) std::cerr<<"Failed to load texture from "<<path;
	return texture;
};
