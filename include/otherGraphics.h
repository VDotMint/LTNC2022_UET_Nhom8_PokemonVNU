#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TitleScreenButton {
    private:
        SDL_Texture* buttonTexture;
        bool clickedOn;
        SDL_Rect buttonFrames[3];
        SDL_Rect buttonDest;
        int currentButtonFrame;
    public:
        TitleScreenButton();
        ~TitleScreenButton();
        void initTSBTexture(const char* path); // Initialize the buttons
        void setButtonDest(int x, int y);
        SDL_Rect* getButtonFrame();
        void drawButton();
        void buttonHandler(SDL_Event* e);
        bool isClicked();
        void resetClickState();
};

class TitleScreen {
    private:
        SDL_Texture* splashScreenTexture;
        SDL_Texture* titleScreenBackground;
        SDL_Texture* titleScreenLogo;
        TitleScreenButton tsButtons[4];
        bool acceptInput;
    public:
        TitleScreen();
        ~TitleScreen();
        void freeTitleScreen();
        void initTitleScreen();
        void drawTitleScreen();
        bool acceptInputState();
        void doButtonEvents(SDL_Event* e);
        void tsButtonInit(); // Place the buttons on the title screen
};