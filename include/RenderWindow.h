#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class RenderWindow {
public:
    RenderWindow();
    
    SDL_Window* window;
    SDL_Renderer* renderer;

    void initSystem();
    void render(const char* path);
    
    SDL_Texture* loadTexture(const char* path);

};