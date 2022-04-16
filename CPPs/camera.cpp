#include <camera.h>

const int MOVE_SPEED = 4;

gameCam::gameCam() {
    camera = {0, 0, 64*13, 64*11};
    isMoving = 0, finishMove = 0, moveUp = 0, moveLeft = 0, moveDown = 0, moveRight = 0;
}

gameCam::~gameCam() {
    camera = {0, 0, 0, 0};
}

void gameCam::setCameraPos(int _x, int _y) {
    camera.x = _x;
    camera.y = _y;
}

void gameCam::beginMovement(SDL_Event* e) {
    switch (e->key.keysym.sym) {
        case SDLK_w:                    
            moveUp = true; break;
        case SDLK_a:                     
            moveLeft = true; break;
        case SDLK_s:                    
            moveDown = true; break;
        case SDLK_d:                    
            moveRight = true; break;
        default:
            break;
    }
}

void gameCam::stopMovement(SDL_Event *e) {
    switch (e->key.keysym.sym) {
        case SDLK_w:                        
            finishMove = true; break;
        case SDLK_a:                        
            finishMove = true; break;
        case SDLK_s:                        
            finishMove = true; break;
        case SDLK_d:                        
            finishMove = true; break;
        default:
            break;
    }
}

void gameCam::moveCamera() {
    if (moveRight == true) {
        isMoving = true; 
        camera.x += MOVE_SPEED;
    } else if (moveLeft == true) {
        isMoving = true; 
        camera.x -= MOVE_SPEED;
    } else if (moveUp == true) {
        isMoving = true; 
        camera.y -= MOVE_SPEED;
    } else if (moveDown == true) {
        isMoving = true; 
        camera.y += MOVE_SPEED;
    }
}

void gameCam::finishMovement() {
    if (moveRight == true) {
        if (camera.x % 64 != 0) {
            camera.x += MOVE_SPEED;
        } else {
            moveRight = false;
            isMoving = false;
            finishMove = false;
        }
    } else if (moveLeft == true) {
        if (camera.x % 64 != 0) {
            camera.x -= MOVE_SPEED;
        } else {
            isMoving = false;
            moveLeft = false;
            finishMove = false;
        }
    } else if (moveUp == true) {
        if (camera.y % 64 != 0) {
            camera.y -= MOVE_SPEED;
        } else {
            isMoving = false;
            moveUp = false;
            finishMove = false;
        }
    } else if (moveDown == true) {
        if (camera.y % 64 != 0) {
            camera.y += MOVE_SPEED;
        } else {
            isMoving = false;
            moveDown = false;
            finishMove = false;
        }
    }    
}

void gameCam::finishIllegalPos(int mapWidth, int mapHeight) {
    if (camera.x > mapWidth*64 - 832) {
        camera.x -= 64;
    } else if (camera.y > mapHeight*64 - 704) {
        camera.y -= 64;
    } else if (camera.x < 0) {
        camera.x += 64;
    } else if (camera.y < 0) {
        camera.y += 64;
    }
}

bool gameCam::getMovementState() {
    return isMoving;
}

bool gameCam::getFinishingState() {
    return finishMove;
}

int gameCam::getCamX() {
    return camera.x;
}

int gameCam::getCamY() {
    return camera.y;
}