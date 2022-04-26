#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class dialogueBox {
    private:
        TTF_Font* textFont;
        SDL_Texture* dialogueBoxTexture;
    public:
        dialogueBox();
        ~dialogueBox();
        void initDialogueBox(const char* fontPath, const char* dBTexture);
        void renderDialogueBox();
};