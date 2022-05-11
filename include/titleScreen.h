#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TitleScreenButton {
    private:
        SDL_Texture* buttonTexture;
        bool clickedOn = false;
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
        SDL_Texture* getButtonTexture();
};

class TitleScreen {
    private:
        SDL_Texture* splashScreenTexture;
        SDL_Texture* titleScreenBackground;
        SDL_Texture* titleScreenLogo;
        bool presentSaveFile;
        bool acceptInput;
    public:
        TitleScreenButton tsButtons[4];
        TitleScreen();
        ~TitleScreen();
        void freeTitleScreen();
        void initTitleScreen(bool hasSaveFile);
        void drawTitleScreen();
        void stopInputState();
        bool acceptInputState();
        void doButtonEvents(SDL_Event* e);
        void tsButtonInit(); // Place the buttons on the title screen
};