g++ -fdiagnostics-color=always -g -Wall src\\*.cpp -o build/game.exe -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
cd build
game
cd ..