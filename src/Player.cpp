#include "Player.h"

Player::Player() {
    rect={0,0,32,32};
    texturePath="res/titlescreen/logo.png"; //temp
};

Player::Player(int x, int y, int h, int w,const char* _texturePath) {
    rect={x,y,w,h};
    texturePath=_texturePath;
};

Player::Player(SDL_Rect _rect, const char* _texturePath) {
    rect=_rect;
    texturePath=_texturePath;
};
