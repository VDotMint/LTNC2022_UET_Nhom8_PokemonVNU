#include "music.h"

Music::Music() {
    gameTheme = NULL;
}

Music::~Music() {
    freeMusic();
}

void Music::freeMusic() {
    Mix_FreeMusic(gameTheme);
}

void Music::loadMusic(const char* path) {
    gameTheme = Mix_LoadMUS(path);
}

void Music::play() {
    Mix_PlayMusic(gameTheme, -1);
}