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

	void createFont(const char* fontPath, int fontSize);

	void display(int x, int y, SDL_Renderer* renderer);

	void textInit(SDL_Renderer* renderer, const char* message, const SDL_Color& color, int rightEdge = 753);

	SDL_Texture* loadFont(SDL_Renderer* renderer, const char* message, const SDL_Color& color, int rightEdge);

private:
	TTF_Font* textFont;
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
