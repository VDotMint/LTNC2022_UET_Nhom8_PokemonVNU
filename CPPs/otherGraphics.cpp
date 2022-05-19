#include <RenderWindow.h>
#include <otherGraphics.h>
#include "Variables.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <iostream>
#include <string>

// TEXT

void Text::textInit(SDL_Renderer* renderer, const char* message, const SDL_Color& color, int rightEdge) {
	SDL_DestroyTexture(TextTexture);
	TextTexture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(textFont, message, color, 753);
	TextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_QueryTexture(TextTexture, nullptr, nullptr, &TextRect.w, &TextRect.h);
};

Text::Text() {
	TextTexture = NULL;
}

Text::~Text() {
	freeText();
}

void Text::freeText() {
        TTF_CloseFont(textFont);
	    textFont=NULL;
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

// DIALOGUE BOX

dialogueBox::dialogueBox() {
	dialogueBoxTexture = NULL;
}

dialogueBox::~dialogueBox() {
	freeDialogueBox();
}

void dialogueBox::freeDialogueBox() {
    if (dialogueBoxTexture != NULL) {
        SDL_DestroyTexture(dialogueBoxTexture);
	    dialogueBoxTexture = NULL;
    }
}

void dialogueBox::initDialogueBox(SDL_Renderer* renderer, const char* path) {
	SDL_Surface* dialogueBoxSurface = IMG_Load(path);
	dialogueBoxTexture = SDL_CreateTextureFromSurface(renderer, dialogueBoxSurface);
	if (!dialogueBoxTexture) {
		std::cout << "Error" << endl;
	}
}

void dialogueBox::renderDialogueBox(SDL_Renderer* renderer, SDL_Rect* clip) {
	SDL_RenderCopy(renderer, dialogueBoxTexture, NULL, clip);
}

MenuButton::MenuButton() {
    buttonTexture = NULL;
    clickedOn = false;
}

MenuButton::~MenuButton() {
	SDL_DestroyTexture(buttonTexture);
    buttonTexture = NULL;
    clickedOn = false;
}

void MenuButton::initMB(const char* path, int x, int y) {
    buttonTexture = IMG_LoadTexture(RenderWindow::renderer, path);
    buttonDest.x = x;
	buttonDest.y = y;
	SDL_QueryTexture(buttonTexture, NULL, NULL, &buttonDest.w, &buttonDest.h);
}

void MenuButton::drawButton() {
    SDL_RenderCopy(RenderWindow::renderer, buttonTexture, NULL, &buttonDest);
}

void MenuButton::buttonHandler() {
    // 0: INSIDE, 1: HOVER, 2: CLICK
    if (e.type == SDL_MOUSEMOTION or e.type == SDL_MOUSEBUTTONDOWN or e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;
        if (x < buttonDest.x) inside = false;
        else if (x > buttonDest.x + buttonDest.w) inside = false;
        else if (y < buttonDest.y) inside = false;
        else if (y > buttonDest.y + buttonDest.h) inside = false;

        if (inside == true and e.type == SDL_MOUSEBUTTONUP) clickedOn = true;
    }
}

// GAME MENU

gameMenu::gameMenu() {
}

gameMenu::~gameMenu() {
	freeMenu();
}

void gameMenu::initMenu() {
	menuTexture = IMG_LoadTexture(RenderWindow::renderer, "res/otherassets/gameMenu.png");

	buttons[0].initMB("res/otherassets/pokemonButton.png", 559, 87);
	buttons[1].initMB("res/otherassets/saveButton.png", 559, 182);
	buttons[2].initMB("res/otherassets/highscoreButton.png", 559, 277);
	buttons[3].initMB("res/otherassets/quitButton.png", 559, 372);

	menuSelScreen.initMenuSelectionScreen();

	menuDest.x = 538;
	menuDest.y = 43;
	SDL_QueryTexture(menuTexture, NULL, NULL, &menuDest.w, &menuDest.h);
}

void gameMenu::freeMenu() {
	SDL_DestroyTexture(menuTexture);
	menuTexture = NULL;
	for (int i = 0; i < 4; i++) buttons[i].~MenuButton();
}

void gameMenu::centralMenuInputProcess() {
	if (e.type == SDL_KEYDOWN and e.key.keysym.sym == SDLK_ESCAPE and inPokemonView == false) {
		inMenu = false;
        return;
	}

    if (inPokemonView == false) {

        for (int i = 0; i < 4; i++) buttons[i].buttonHandler();

        buttons[0].buttonHandler();
        if (buttons[0].clickedOn == true) {
            buttons[0].clickedOn = false;
            Mix_PlayChannel(-1, clickedOnSound, 0);
            menuSelScreen.updateMenuSelectionScreen();
            inPokemonView = true;
            return;
        }

        buttons[1].buttonHandler();
        if (buttons[1].clickedOn == true) {
            buttons[1].clickedOn = false;
            Mix_PlayChannel(-1, gameSaved, 0);
            mainPlayer.savePlayerData();
            inMenu = false;
            return;
        }

        buttons[3].buttonHandler();
        if (buttons[3].clickedOn == true) {
            buttons[3].clickedOn = false;
            Mix_PlayChannel(-1, clickedOnSound, 0);
            inMenu = false;
            return;
        }
    }

	if (inPokemonView == true) {
		menuSelScreen.backButton.buttonHandler();
        if (menuSelScreen.backButton.clickedOn == true) {
            menuSelScreen.backButton.clickedOn = false;
            inPokemonView = false;
            return;
        }
	}
}

void gameMenu::drawMenu() {
	SDL_RenderCopy(RenderWindow::renderer, menuTexture, NULL, &menuDest);
	for (int i = 0; i < 4; i++) buttons[i].drawButton();

    if (inPokemonView == true) {
        menuSelScreen.display();
    }
}

// POKEMON SELECTION SCREEN (MENU'S VERSION)

MenuPokemonSelectionScreen::MenuPokemonSelectionScreen() {
}

MenuPokemonSelectionScreen::~MenuPokemonSelectionScreen() {
    freeMenuSelectionScreen();
}

void MenuPokemonSelectionScreen::initMenuSelectionScreen() {
    pokePartyScreenText = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/pokemon_sel_screen.png");
    pokePartyScreenRect = {91, 152, 650, 400};

    canBattleTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/canBattleIcon.png");
    cannotBattleTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/cannotBattleIcon.png");
    pokeBallRect[0] = {124, 244, 36, 36};
    pokeBallRect[1] = {124, 322, 36, 36};
    pokeBallRect[2] = {124, 400, 36, 36};

    HPBarTexture = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/inPartyHP.png");
    hpBarRect[0] = {500, 280, 168, 18};
    hpBarRect[1] = {500, 358, 168, 18};
    hpBarRect[2] = {500, 436, 168, 18};

    hpColor = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/HPColor.png");
    pokemonHP[0] = {541, 284, int(ceil(121.0*(double(mainPlayer.party[0].c_hp)/double(mainPlayer.party[0].data->hp)))), 8};
    pokemonHP[1] = {541, 362, int(ceil(121.0*(double(mainPlayer.party[1].c_hp)/double(mainPlayer.party[1].data->hp)))), 8};
    pokemonHP[2] = {541, 440, int(ceil(121.0*(double(mainPlayer.party[2].c_hp)/double(mainPlayer.party[2].data->hp)))), 8};

    for (int i = 0; i < 3; i++) {
        pokeNames[i].createFont("res/font/gamefont.ttf", 42);
        pokeNames[i].textInit(RenderWindow::renderer, (mainPlayer.party[i].data->name).c_str(), {0, 0, 0}, 710);

        pokeTextHPs[i].createFont("res/font/gamefont.ttf", 28);
        std::string newHPText = to_string(mainPlayer.party[i].c_hp) + "/" + to_string(mainPlayer.party[i].data->hp); 
        pokeTextHPs[i].textInit(RenderWindow::renderer, newHPText.c_str(), {0,0,0}, 710);
    }

    backButton.initMB("res/otherassets/backButton.png", 592, 473);
}

void MenuPokemonSelectionScreen::freeMenuSelectionScreen() {
    SDL_DestroyTexture(pokePartyScreenText);
    SDL_DestroyTexture(canBattleTexture);
    SDL_DestroyTexture(cannotBattleTexture);
    SDL_DestroyTexture(HPBarTexture);
    SDL_DestroyTexture(hpColor);
    pokePartyScreenText = NULL;
    canBattleTexture = NULL;
    cannotBattleTexture = NULL;
    HPBarTexture = NULL;
    hpColor = NULL;

    for (int i = 0; i < 3; i++) pokeNames[i].freeText();
    backButton.~MenuButton();
}

void MenuPokemonSelectionScreen::updateMenuSelectionScreen() {
    hpColor = IMG_LoadTexture(RenderWindow::renderer, "res/battleassets/HPColor.png");
    pokemonHP[0] = {541, 284, int(ceil(121.0*(double(mainPlayer.party[0].c_hp)/double(mainPlayer.party[0].data->hp)))), 8};
    pokemonHP[1] = {541, 362, int(ceil(121.0*(double(mainPlayer.party[1].c_hp)/double(mainPlayer.party[1].data->hp)))), 8};
    pokemonHP[2] = {541, 440, int(ceil(121.0*(double(mainPlayer.party[2].c_hp)/double(mainPlayer.party[2].data->hp)))), 8};

    for (int i = 0; i < 3; i++) {
        pokeNames[i].createFont("res/font/gamefont.ttf", 42);
        pokeNames[i].textInit(RenderWindow::renderer, (mainPlayer.party[i].data->name).c_str(), {0, 0, 0}, 710);

        pokeTextHPs[i].createFont("res/font/gamefont.ttf", 28);
        std::string newHPText = to_string(mainPlayer.party[i].c_hp) + "/" + to_string(mainPlayer.party[i].data->hp); 
        pokeTextHPs[i].textInit(RenderWindow::renderer, newHPText.c_str(), {0,0,0}, 710);
    }
}

void MenuPokemonSelectionScreen::display() {
    SDL_RenderCopy(RenderWindow::renderer, pokePartyScreenText, NULL, &pokePartyScreenRect);

    pokeNames[0].display(173, 244, RenderWindow::renderer);
    pokeNames[1].display(173, 322, RenderWindow::renderer);
    pokeNames[2].display(173, 400, RenderWindow::renderer);

    for (int i = 0; i < 3; i++) {
        if (pokemonHP[i].w == 0) SDL_RenderCopy(RenderWindow::renderer, cannotBattleTexture, NULL, &pokeBallRect[i]);
        else SDL_RenderCopy(RenderWindow::renderer, canBattleTexture, NULL, &pokeBallRect[i]);

        SDL_RenderCopy(RenderWindow::renderer, HPBarTexture, NULL, &hpBarRect[i]);
        SDL_RenderCopy(RenderWindow::renderer, hpColor, NULL, &pokemonHP[i]);
    }

    pokeTextHPs[0].display(500, 244, RenderWindow::renderer);
    pokeTextHPs[1].display(500, 322, RenderWindow::renderer);
    pokeTextHPs[2].display(500, 400, RenderWindow::renderer);

    backButton.drawButton();
}