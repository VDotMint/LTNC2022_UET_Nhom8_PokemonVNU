#include "Pokemon.h"
#include <ctime>
#include <SDL2/SDL.h>

Pokemon::Pokemon(int i) {
	data=pokemonData+i;
	c_hp=data->hp;
	for (int i=0;i<4;i++) {
		c_pp[i]=data->move[i]->pp;
	}
}

Pokemon::Pokemon() {
	data=pokemonData;
	c_hp=data->hp;
	for (int i=0;i<4;i++) {
		c_pp[i]=data->move[i]->pp;
	}
};