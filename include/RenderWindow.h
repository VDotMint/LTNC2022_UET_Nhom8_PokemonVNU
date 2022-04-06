#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class RenderWindow {
    public:
        RenderWindow();
        ~RenderWindow();
        SDL_Texture* loadTexture(const char* path);
        void create(const char *title="Title",int h=64*13,int w=64*11);
        void close();
        void clear();
        void drawColor(int red, int green, int blue);
        void render(SDL_Texture* texture);
        void display();

        static SDL_Renderer* renderer;
        
    private:
        SDL_Window* window;
};
