#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class NPC {
private:
    int npcID;
    int npcXCoords, npcYCoords;
    SDL_Texture* npcTexture;
    SDL_Rect walkFrames[16];
public:
    NPC();
    ~NPC();
    void initNPC();
};