#include <RenderWindow.h>
#include <otherGraphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>



using namespace std;
void Text::textInit(SDL_Renderer* renderer, const char* fontPath, int fontSize, const char* message, const SDL_Color& color) {
	TextTexture = loadFont(renderer, fontPath, fontSize, message, color);
	SDL_QueryTexture(TextTexture, nullptr, nullptr, &TextRect.w, &TextRect.h);
};

Text::Text() {
	TextTexture = NULL;
}

Text::~Text() {
	SDL_DestroyTexture(TextTexture);
	TextTexture = NULL;
}

void Text::display(int x, int y, SDL_Renderer* renderer) {
	TextRect.x = x;
	TextRect.y = y;
	SDL_RenderCopy(renderer, TextTexture, nullptr, &TextRect);
}

SDL_Texture* Text::loadFont(SDL_Renderer* renderer, const char* fontPath, int fontSize, const char* message, const SDL_Color& color) {
	TTF_Font* textFont = TTF_OpenFont(fontPath, fontSize);
	if (!textFont) {
		cout << "Failed to load font" << endl;
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(textFont, message, color, 832);
	//SDL_FreeSurface(textSurface);
	if (!textSurface) {
		cout << "textSurface failed" << endl;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		cout << "textTexture1 failed" << endl;
	}
	SDL_FreeSurface(textSurface);
	return textTexture;
}

dialogueBox::dialogueBox() {
	dialogueBoxTexture = NULL;
}

dialogueBox::~dialogueBox() {
	SDL_DestroyTexture(dialogueBoxTexture);
	dialogueBoxTexture = NULL;
}

void dialogueBox::initDialogueBox(SDL_Renderer* renderer, const char* path) {
	SDL_Surface* dialogueBoxSurface = IMG_Load(path);
	dialogueBoxTexture = SDL_CreateTextureFromSurface(renderer, dialogueBoxSurface);
	if (!dialogueBoxTexture) {
		cout << "Error" << endl;
	}
}

void dialogueBox::renderDialogueBox(SDL_Renderer* renderer, SDL_Rect* clip) {
	SDL_RenderCopy(renderer, dialogueBoxTexture, NULL, clip);
}
