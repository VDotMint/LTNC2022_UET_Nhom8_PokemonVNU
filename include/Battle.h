#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include "BattleScreen.h"

#include "mPlayer.h"
#include "Pokemon.h"

using namespace std;

void battle (Pokemon my[],Pokemon op[]);

int battle(Pokemon &my,Pokemon &op);

void battle(Pokemon my[]);

void updateTerminal(Pokemon &my, Pokemon &op);

void printParty(Pokemon my[]);

int selectPokemon(Pokemon my[]);

void printMoves(Pokemon &my);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);

extern string Type[];
extern int psize;
extern BattleScreen mainBattle;