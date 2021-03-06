#pragma once
#include <stdbool.h>
#include "usb_kb/GenericTypeDefs.h"
#include "alt_types.h"

int gameBoard[20][10]; //2d array containing current game board data
int keycode; //keyboard input
int highscore; //high score variable
alt_u8 scorer[3]; //high scorer letters


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

void gameTest();
int tetris(int startLevel); //top-level function to run the tetris game, returns score
void spawnBlock(int Block); //spawn random block with repeat check, takes previous block id and returns new block id
bool shiftLeft(int blockState, int X, int Y); //move block to left, return true if piece is shifted successfully
bool shiftRight(int blockState, int X, int Y); // move block to right, return true if piece is shifted successfully
bool drop(int blockState, int X, int Y); //drops moving block down 1 move, returns true if block locks into placem false if block moves
int lineClear(); //checks for and clears complete lines, called when a block locks into place, returns number of lines cleared
bool gameOver(); //checks if block is at top row, called when a block locks into place
int rotateLeft(int blockState, int X, int Y); //rotate piece left if possible and return new state
int rotateRight(int blockState, int X, int Y); //rotate piece right if possible and return new state

void draw1(int X, int Y, int val);
void draw2(int X, int Y, int val);
void draw3(int X, int Y, int val);
void draw4(int X, int Y, int val);
void draw5(int X, int Y, int val);
void draw6(int X, int Y, int val);
void draw7(int X, int Y, int val);
void draw8(int X, int Y, int val);
void draw9(int X, int Y, int val);
void draw10(int X, int Y, int val);
void draw11(int X, int Y, int val);
void draw12(int X, int Y, int val);
void draw13(int X, int Y, int val);
void draw14(int X, int Y, int val);
void draw15(int X, int Y, int val);
void draw16(int X, int Y, int val);
void draw17(int X, int Y, int val);
void draw18(int X, int Y, int val);
void draw19(int X, int Y, int val);
