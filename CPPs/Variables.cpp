#include "Variables.h"

bool debugMode=false;

RenderWindow renderWindow;

SDL_Event e;

TitleScreen gameTitleScreen;
SDL_Texture* blackTransitionTexture;

string gameMaps[] = {
    "res/map/g2.map",
    "res/map/e3.map",
    "res/map/e3i.map",
    "res/map/g2i.map",
    "res/map/e3i_2.map",
};
string gameTileSets[] = {
    "res/tileset/g2o_tiles.png",
    "res/tileset/e3o_tiles.png",
    "res/tileset/e3i_tiles.png",
    "res/tileset/g2i_tiles.png",
    "res/tileset/e3i_2_tiles.png",
};
string gameThemes[] = {
    "res/music/g2o_theme.mp3",
    "res/music/e3o_theme.mp3",
    "res/music/e3i_theme.mp3",
    "res/music/e3i_theme.mp3",
    "res/music/e3i_theme.mp3",
};
double themeRepeats[] = {
    8.85,
    30.555,
    0.77,
    0.77,
    0.77,
};
bool mapOverlays[] = {
    false,
    false,
    true,
    true,
    true,
};
string Type[]= {
    "NULL",
	"Normal",
	"Fire",
	"Water",
	"Electric",
	"Grass",
	"Ice",
	"Fighting",
	"Poison",
	"Ground",
	"Flying",
	"Psychic",
	"Bug",
	"Rock",
	"Ghost",
	"Dragon",
	"Dark",
	"Steel",
	"Fairy"
};

Move moves[]=
{
	{"Body Slam",1,85,15},
	{"Flamethrower",2,90,15},
    {"Surf",3,90,15},
    {"thunderbolt",4,90,15},
    {"Leaf Blade",5,90,15},
    {"Ice Beam",6,90,10},
    {"Sacred Sword",7,90,15},
    {"Sludge Bomb",8,90,10},
    {"Earthquake",9,100,10},
    {"Drill Peck",10,80,20},
    {"Psychic",11,90,10},
    {"Bug Buzz",12,90,10},
    {"Power Gem",13,80,20},
    {"Shadow Ball",14,80,20},
    {"Dragon Pulse",15,85,10},
    {"Darkest Lariat",16,85,10},
    {"Iron Head",17,80,15},
    {"Moon Blast",18,95,15}
};
PokemonData pokemonData[]=
{
	//{"Snorlax",1,0,220,93,93,35,{moves,moves}},
    //{"Charmander",2,0,99,61,52,70,{moves,moves+1}},
	//{"Pikachu",4,0,105,83,60,125,{moves,moves+2}},
    {"Tauros", 1, 0, 135, 105, 75, 115, {moves,moves,moves,moves}}, 
    {"Snorlax", 1, 0, 220, 70, 115, 35, {moves,moves,moves,moves}},
    {"Charizard", 2, 10, 138, 114, 83, 105, {moves,moves+1,moves,moves}}, 
    {"Torkoal", 2, 0, 130, 90, 145, 25, {moves,moves+1,moves,moves}}, 
    {"Gyarados", 3, 10, 155, 130, 84, 86, {moves,moves+2,moves,moves}}, 
    {"Milotic", 3, 0, 155, 65, 130, 86,{moves,moves+2,moves,moves}}, 
    {"Electivire", 4, 0, 135, 128, 72, 100,{moves,moves+3,moves,moves}}, 
    {"Magnezone", 4, 17, 130, 75, 120, 65, {moves,moves+3,moves,moves}},
    {"Exeggutor", 5, 11, 155, 130, 80, 60, {moves,moves+4,moves,moves}},
    {"Virizion", 5, 7, 151, 95, 134, 113, {moves,moves+4,moves,moves}}, 
    {"Mamoswine", 6, 9, 170, 135, 65, 85, {moves,moves+5,moves,moves}},
    {"Lapras", 6, 3, 190, 90, 100, 65, {moves,moves+5,moves,moves}},
    {"Lucario", 7, 17, 130, 120, 75, 95, {moves,moves+6,moves,moves}},
    {"Infernape", 7, 3, 136, 109, 76, 113, {moves,moves+6,moves,moves}},
    {"Gengar", 8, 14, 120, 135, 65, 115, {moves,moves+7,moves,moves}},  
    {"Venusaur", 8, 5, 140, 87, 105, 85, {moves,moves+7,moves,moves}},
    {"Swampert", 9, 3, 160, 115, 95, 65, {moves,moves+8,moves,moves}},
    {"Golem", 9, 13, 140, 60, 135, 50, {moves,moves+8,moves,moves}},
    {"Archeops", 10, 13, 135, 145, 70, 115, {moves,moves+9,moves,moves}},
    {"Togekiss", 10, 18, 145, 125, 100, 85, {moves,moves+9,moves,moves}},
    {"Alakazam", 11, 0, 105, 140, 50, 125, {moves,moves+10,moves,moves}},
    {"Mr. Mime", 11, 18, 100, 50, 125, 95, {moves,moves+10,moves,moves}},
    {"Heracross", 12, 7, 140, 130, 80, 90, {moves,moves+11,moves,moves}},
    {"Shuckle", 12, 13, 80, 15, 235, 10, {moves,moves+11,moves,moves}},
    {"Crustle", 13, 12, 130, 70, 130, 50, {moves,moves+12,moves,moves}},
    {"Sudowoodo", 13, 0, 130, 35, 120, 35, {moves,moves+12,moves,moves}},
    {"Chandelure", 14, 3, 120, 150, 60, 85, {moves,moves+13,moves,moves}},
    {"Dusknoir", 14, 0, 105, 105, 140, 50, {moves,moves+13,moves,moves}},
    {"Garchomp", 15, 9, 168, 135, 90, 107, {moves,moves+14,moves,moves}},
    {"Dragonite", 15, 10, 151, 139, 100, 85, {moves,moves+14,moves,moves}},
    {"Absol", 16, 0, 125, 125, 70, 80, {moves,moves+15,moves,moves}},
    {"Umbreon", 16, 0, 155, 65, 135, 70, {moves,moves+15,moves,moves}},
    {"Excadrill", 17, 9, 170, 140, 65, 93, {moves,moves+16,moves,moves}},
    {"Steelix", 17, 9, 135, 60, 205, 35, {moves,moves+16,moves,moves}},
    {"Gardevoir", 18, 11, 128, 130, 70, 85, {moves,moves+17,moves,moves}},
    {"Clefable", 18, 0, 155, 100, 78, 65, {moves,moves+17,moves,moves}}
};

int psize=sizeof(pokemonData)/sizeof(pokemonData[0]);

double typeEffectiveness[19][19]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,0.5,0,1,1,0.5,1},
    {1,1,0.5,0.5,1,2,2,1,1,1,1,1,2,0.5,1,0.5,1,2,1},
    {1,1,2,0.5,1,0.5,1,1,1,2,1,1,1,2,1,0.5,1,1,1},
    {1,1,1,2,0.5,0.5,1,1,1,0,2,1,1,1,1,0.5,1,1,1},
    {1,1,0.5,2,1,0.5,1,1,0.5,2,0.5,1,0.5,2,1,0.5,1,0.5,1},
    {1,1,0.5,0.5,1,2,0.5,1,1,2,2,1,1,1,1,2,1,0.5,1},
    {1,2,1,1,1,1,2,1,0.5,1,0.5,0.5,0.5,2,0,1,2,2,0.5},
    {1,1,1,1,1,2,1,1,0.5,0.5,1,1,1,0.5,0.5,1,1,0,2},
    {1,1,2,1,2,0.5,1,1,2,1,0,1,0.5,2,1,1,1,2,1},
    {1,1,1,1,0.5,2,1,2,1,1,1,1,2,0.5,1,1,1,0.5,1},
    {1,1,1,1,1,1,1,2,2,1,1,0.5,1,1,1,1,0,0.5,1},
    {1,1,0.5,1,1,2,1,0.5,0.5,1,0.5,2,1,1,0.5,1,2,0.5,0.5},
    {1,1,2,1,1,1,2,0.5,1,0.5,2,1,2,1,1,1,1,0.5,1},
    {1,0,1,1,1,1,1,1,1,1,1,2,1,1,2,1,0.5,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,0.5,0},
    {1,1,1,1,1,1,1,0.5,1,1,1,2,1,1,2,1,0.5,1,0.5},
    {1,1,0.5,0.5,0.5,1,2,1,1,1,1,1,1,2,1,1,1,0.5,2},
    {1,1,0.5,1,1,1,1,2,0.5,1,1,1,1,1,1,2,2,0.5,1}
};

SDL_Rect dBoxClip = { 10, 515, 812, 179 };
dialogueBox d_box;
Text d_text;

Mix_Chunk* changeMap;
Mix_Chunk* aButton;

Map* playerMap;
Music gameMusic;

mPlayer mainPlayer;
gameCam mainCamera;

BattleScreen mainBattle;

// GAME STATES

bool tsToMapTransition = false;
bool beginMapToMapTransition = false;
bool finishMapToMapTransition = false;
int transitionTransparency = 0;

bool beginMapToBattleTransition = false;
bool finishMapToBattleTransition = false;
bool beginBattleToMapTransition = false;
bool finishBattleToMapTransition = false;

bool hasSaveFile = true;
bool quit = false;

bool inTitleScreen = false; // SET TO FALSE TO SKIP TITLE SCREEN FOR FASTER DEBUG
bool inBattle = false;
bool inDialogue = false;

bool playerIsRunning = false;

Trainer defaultOppo;