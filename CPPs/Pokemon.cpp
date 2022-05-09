#include "Pokemon.h"
#include <ctime>
#include <SDL2/SDL.h>

Pokemon::Pokemon(int i) {
	data=pokemonData+i;
	c_hp=data->hp;
	for (int i=0;i<2;i++) {
		c_pp[i]=data->move[i]->pp;
	}
}

Pokemon::Pokemon() {
	srand(time(0));
	SDL_Delay(2);
	data=pokemonData+rand()%psize;
	c_hp=data->hp;
	for (int i=0;i<2;i++) {
		c_pp[i]=data->move[i]->pp;
	}
};