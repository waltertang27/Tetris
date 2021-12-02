
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
void shiftLeft(); //move block to left
void shiftRight(); // move block to right

