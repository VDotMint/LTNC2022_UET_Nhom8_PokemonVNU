#pragma once
#include <iostream>
#include <iomanip>


using namespace std;
class Move {
	public:
		string name;
		int power;
};

class Pokemon {
	public:
		string name;
		int hp;
		int atk;
		int def;
		Move* move[2];
};

void battle(Pokemon my,Pokemon op);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);