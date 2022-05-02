#pragma once
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class Move {
	public:
		string name;
		int type;
		int power;
		int pp;
};

class PokemonData {
	public:
		string name;
		int type;
		int hp;
		int atk;
		int def;
		Move* move[2];
};

class Pokemon {
	public:
		PokemonData* data;
		int c_hp;
		int c_pp[2];
		Pokemon();
		Pokemon(int i);
};

class Trainer {
	public:
		string name;
		Pokemon party[2];
};

void battle (Pokemon my[],Pokemon op[]);

bool battle(Pokemon &my,Pokemon &op);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);

extern string Type[];
extern PokemonData pokemonData[];