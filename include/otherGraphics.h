#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;

class Text {

public:
	Text();
	~Text();

	void display(int x, int y, SDL_Renderer* renderer);

	void textInit(SDL_Renderer* renderer, const char* fontPath, int fontSize, const char* message, const SDL_Color& color);

	SDL_Texture* loadFont(SDL_Renderer* renderer, const char* fontPath, int fontSize, const char* message, const SDL_Color& color);
private:
	SDL_Texture* TextTexture;
	SDL_Rect TextRect;
};

class dialogueBox {
public:
	dialogueBox();
	~dialogueBox();

	void initDialogueBox(SDL_Renderer* renderer, const char* path);
	void renderDialogueBox(SDL_Renderer* renderer, SDL_Rect* clip);
private:
	SDL_Texture* dialogueBoxTexture;

};
