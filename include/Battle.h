#pragma once
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

extern string Type[];

class Move {
	public:
		string name;
		int power;
		int type;
};

class Pokemon {
	public:
		string name;
		int type;
		int hp;
		int atk;
		int def;
		Move* move[2];
};

class Trainer {
	public:
		string name;
		Pokemon party[2];
};

void battle(Pokemon my,Pokemon op);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);