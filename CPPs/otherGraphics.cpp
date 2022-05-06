#include <RenderWindow.h>
#include <otherGraphics.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

void Text::textInit(SDL_Renderer* renderer, const char* message, const SDL_Color& color, int rightEdge) {
	SDL_DestroyTexture(TextTexture);
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(textFont, message, color, 753);
	TextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_QueryTexture(TextTexture, nullptr, nullptr, &TextRect.w, &TextRect.h);
	SDL_RenderCopy(renderer, TextTexture, nullptr, &TextRect);
};

Text::Text() {
	TextTexture = NULL;
}

Text::~Text() {
	TTF_CloseFont(textFont);
	SDL_DestroyTexture(TextTexture);
	TextTexture = NULL;
}

void Text::createFont(const char* fontPath, int fontSize) {
	textFont = TTF_OpenFont(fontPath, fontSize);
}

void Text::display(int x, int y, SDL_Renderer* renderer) {
	TextRect.x = x;
	TextRect.y = y;
	SDL_RenderCopy(renderer, TextTexture, nullptr, &TextRect);
}

// SDL_Texture* Text::loadFont(SDL_Renderer* renderer, const char* message, const SDL_Color& color, int rightEdge) {
	
// 	return textTexture;
// }

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
