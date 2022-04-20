#include "otherGraphics.h"
#include "RenderWindow.h"

gameButton::gameButton(int x, int y, int w, int h, std::string content) {
    boundingBox = {x, y, w, h};
    buttonTexture = NULL;
    buttonText = content;
}

gameButton::~gameButton() {
    boundingBox = {0, 0, 0, 0};
    SDL_DestroyTexture(buttonTexture);
    buttonTexture = NULL;
    buttonText = "";
}

SDL_Rect* gameButton::getButtonFrame() {
    return &boundingBox;
}

void gameButton::drawButton() {
    
}