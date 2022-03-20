#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

class Player {
    public:
        SDL_Rect rect;
        const char* texturePath;
        Player();
        Player(int x, int y, int w, int h,const char* _texturePath);
        Player(SDL_Rect _rect, const char* _texturePath);
};