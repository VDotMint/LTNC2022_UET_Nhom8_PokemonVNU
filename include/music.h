#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Music {
    public:
        Music();
        ~Music();
        bool isPastChord();
        void freeMusic();
        void loadMusic(const char* path, double repeatP = 0.0);
        void play();
        void manualSkip(double test);
    private:
        bool pastChord;
        double repeatPoint;
        Mix_Music* gameTheme;
};