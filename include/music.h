#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Music {
    public:
        Music();
        ~Music();
        void loadMusic(const char* path);
        void play();
    private:
        Mix_Music* gameTheme;
};