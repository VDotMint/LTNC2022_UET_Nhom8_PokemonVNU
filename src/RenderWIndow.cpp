#include "RenderWindow.h"

RenderWindow::RenderWindow() {
    window=nullptr;
	renderer=nullptr;
};

void RenderWindow::initSystem(){
 	if (SDL_Init(SDL_INIT_VIDEO)<0) {
 		std::cerr<<"SDL_Init Error"<<SDL_GetError()<<std::endl;
 	}
 	if (!(IMG_Init(IMG_INIT_PNG))) {
 		std::cerr<<"IMG_Init Error"<<SDL_GetError()<<std::endl;
	}
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window=SDL_CreateWindow("Title",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	render("res/hello_world.bmp");
};

void RenderWindow::render(const char* path) {
	SDL_Rect dst={0,0,64,64};
	SDL_Texture* texture=loadTexture(path);
	SDL_RenderCopy(renderer,texture,NULL,&dst);
	SDL_RenderPresent(renderer);
};


SDL_Texture* RenderWindow::loadTexture(const char* path){
	SDL_Texture* texture;
	texture=IMG_LoadTexture(renderer,path);
	return texture;
};
