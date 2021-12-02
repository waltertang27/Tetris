
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//I block: -1
//J Block: -2
//L block: -3
//O block: -4
//S-block: -5
//T block: -6
//Z block: -7

//the next block to spawn on screen
void dropIBlock() {
	gameBoard[0][4] = -1;
    gameBoard[0][5] = -1;
    gameBoard[0][6] = -1;
    gameBoard[0][7] = -1;
}

void dropJBlock() {
	gameBoard[0][3] = -2;
	gameBoard[1][3] = -2;
	gameBoard[1][4] = -2;
	gameBoard[1][5] = -2;
}

void dropLBlock() {
	gameBoard[0][5] = -3;
	gameBoard[1][3] = -3;
	gameBoard[1][4] = -3;
	gameBoard[1][5] = -3;
}

void dropOBlock() {
	gameBoard[0][4] = -4;
	gameBoard[1][4] = -4;
	gameBoard[0][5] = -4;
	gameBoard[1][5] = -4;
}

void dropSBlock() {
	gameBoard[0][5] = -5;
	gameBoard[0][4] = -5;
	gameBoard[1][4] = -5;
	gameBoard[1][3] = -5;
}

void dropTlock() {
	gameBoard[0][5] = -6;
	gameBoard[1][3] = -6;
	gameBoard[1][4] = -6;
	gameBoard[1][5] = -6;
}

void dropZBlock() {
	gameBoard[0][3] = -7;
	gameBoard[0][4] = -7;
	gameBoard[1][4] = -7;
	gameBoard[1][5] = -7;
}

void tetris() {
    bool gameOver = false;  //checks if game is over
    int prevBlock = 0; //store previous block id
    int num = 0; //random block generation
    int frameDelay = 48; //value to adjust speed of block drop
    int frame = 0; //current frame value for block drop
    bool newBlock = true; //determine when a new block is needed
    int prevKey = 0; //key pressed in previous frame
    int DAS = 16; //held key move delay
    bool stopDrop = false; //flag to stop falling piece
    while(!gameOver) {  //runs while game not over
    	keyboardDriver(); //get new keypress
    	//spawn new block
    	if(newBlock){
    		prevBlock = num;
    		num = spawnBlock(prevBlock);
    	}

    	//handle keyboard movement
    	if(keypress == 80 && prevKey == 80){ //movement left if key is held
    		if(DAS == 0){
    			shiftLeft();
    			DAS = 6;
    		}
    		DAS--;
    	}
    	else if(keypress == 80){ //movement when left key pressed
    		shiftLeft();
    		prevKey = 80;
    	}
    	else if(keypress == 79 && prevKey == 79){ // movement right if key is held
    		if(DAS == 0){
    			shiftRight();
    			DAS = 6;
    		}
    		DAS--;
    	}
    	else if (keypress == 79){ //movement when right key pressed
    		shiftRight();
    		prevKey = 79;
    	}
    	else{ //reset if no key pressed
    		prevKey = 0;
    		DAS = 16;
    	}

    	//handle block auto-drop
    	if(frameDelay == frame){
    		drop();
    		frame = 0;
    	}
    	frame++;
    }

}

int spawnBlock(int prevBlock){
	int num = 0;
	num = (rand() % 7) + 1;   //generate a random number corresponding to a block
	//reroll if block is same as previous, biases against repeat blocks but does not prevent them
	if(num == prevBlock){
		num = (rand() % 7) + 1;
	}
	switch(num) {
		case 1:
			dropIBlock();
			break;
		case 2:
			dropJBlock();
			break;
		case 3:
			dropLBlock();
			break;
		case 4:
			dopOBlock();
			break;
		case 5:
			dropSBlock();
			break;
		case 6:
			dropTBlock();
			break;
		case 7:
			dropZBlock();
			break;
	default:
	}
	return num;
}

void shiftLeft(){
	int count = 0; //when 4 cells have been shifted, shift is complete
	for(int i = 0; i < 20; i++){ //collision check
		if(gameBoard[i][0] < 0){ //left wall collision
			return;
		}
		for(int j = 1; j < 10; j++){
			if(gameBoard[i][j] < 0 && gameBoard[i][j - 1] > 0){ //piece collision
				return;
			}
		}
	}
	for(int i = 0; i < 20; i++){ //shift each cell of moving block
		for(int j = 1; j < 10; j++){
			int current = gameBoard[i][j];
			if(current < 0){
				gameBoard[i][j - 1] = current;
				gameBoard[i][j] = 0;
				count++;
			}
			if(count == 4){
				return;
			}
		}
	}
}

void shiftRight(){
	int count = 0; //when 4 cells have been shifted, shift is complete
	for(int i = 0; i < 20; i++){ //collision check
		if(gameBoard[i][9] < 0){ //right wall collision
 			return;
		}
		for(int j = 0; j < 9; j++){
			if(gameBoard[i][j] < 0 && gameBoard[i][j + 1 < 0]) //piece collision
				return;
		}
	}
	for(int i = 0; i < 20; i++){ //shift each cell of moving block
		for(int j = 8; j >= 0; j--){
			int current = gameBoard[i][j];
			if(current < 0){
				gameBoard[i][j + 1] = current;
				gameBoard[i][j] = 0;
				count++;
			}
			if(count == 4){
				return;
			}
		}
	}
}

//moves the piece DOWN, checks for collisions. Unfinished - need to check for bounds
void drop() {
	for(int )
}
