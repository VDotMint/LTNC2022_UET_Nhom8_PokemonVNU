 g++ -fdiagnostics-color=always -g -Wall CPPs/*.cpp -o build/game -Iinclude `sdl2-config --cflags --libs` -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
 cd build
 ./game