#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class gameButton {
    private:
        SDL_Rect boundingBox;
        SDL_Texture* buttonTexture;
        std::string buttonText;
    public:
        gameButton(int x, int y, int w, int h, std::string content = "");
        ~gameButton();
        SDL_Rect* getButtonFrame();
        void drawButton();
};