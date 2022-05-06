#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>

#include "mPlayer.h"
#include "Pokemon.h"

using namespace std;

void battle (Pokemon my[],Pokemon op[]);

bool battle(Pokemon &my,Pokemon &op);

void battle(Pokemon my[]);

void updateTerminal(Pokemon &my, Pokemon &op);

int selectMove(Pokemon &my);

bool useMove(int input, Pokemon &my, Pokemon &op);

extern string Type[];