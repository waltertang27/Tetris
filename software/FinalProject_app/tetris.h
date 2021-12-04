#include <stdbool.h>

int gameBoard[20][10] = {0}; //global 2d array for master game board
BYTE keypress = 0; //global variable for keyboard input

//I block: -1
//J Block: -2
//L block: -3
//O block: -4
//S-block: -5
//T block: -6
//Z block: -7

//functions to spawn blocks at top of screen
void dropIBlock();
void dropJBlock();
void dropLBlock();
void dropOBlock();
void dropSBlock();
void dropTBlock();
void dropZBlock();

void tetris(); //top-level function to run the tetris game
int spawnBlock(int prevBlock); //spawn random block with repeat check, takes previous block id and returns new block id
int shiftLeft(int blockX); //move block to left and update x position
int shiftRight(int blockX); // move block to right and update x position
bool drop(); //drops moving block down 1 move, returns true if block locks into place
bool lineClear(); //checks for and clears complete lines, called when a block locks into place, returns true if at least one line clears
bool gameOver(); //checks if block is at top row, called when a block locks into place
int rotateLeft(int blockState, int blockX, int blockY); //rotate piece left if possible and return new state
int rotateRight(int blockState, int blockX, int blockY); //rotate piece right if possible and return new state
