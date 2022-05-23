g++ -fdiagnostics-color=always -Wall -Wl,-subsystem,windows CPPs\\*.cpp -o build/game -Iinclude/SDL2 -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
cd build
game
cd ..