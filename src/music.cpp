#include "music.h"

Music::Music() {
    gameTheme = NULL;
}

Music::~Music() {

}

void Music::loadMusic(const char* path) {
    gameTheme = Mix_LoadMUS(path);
}

void Music::play() {
    Mix_PlayMusic(gameTheme, -1);
}