#include "otherGraphics.h"

dialogueBox::dialogueBox(){
    textFont = NULL;
    dialogueBoxTexture = NULL;
}

dialogueBox::~dialogueBox() {
    TTF_CloseFont(textFont);
    textFont = NULL;
    SDL_DestroyTexture(dialogueBoxTexture);
    dialogueBoxTexture = NULL;
}

void dialogueBox::initDialogueBox(con)