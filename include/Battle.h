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

extern Pokemon pokemon[];
extern Move moves[];


void battle(Pokemon my,Pokemon op);
