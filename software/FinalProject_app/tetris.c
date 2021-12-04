
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//I block: -1
//J Block: -2
//L block: -3
//O block: -4
//S-block: -5
//T block: -6
//Z block: -7

void dropIBlock() {
	gameBoard[0][5] = -1;
    gameBoard[1][5] = -1;
    gameBoard[2][5] = -1;
    gameBoard[3][5] = -1;
}

void dropJBlock() {
	gameBoard[0][4] = -2;
	gameBoard[1][4] = -2;
	gameBoard[2][4] = -2;
	gameBoard[2][3] = -2;
}

void dropLBlock() {
	gameBoard[0][4] = -3;
	gameBoard[1][4] = -3;
	gameBoard[2][4] = -3;
	gameBoard[2][5] = -3;
}

void dropOBlock() {
	gameBoard[0][4] = -4;
	gameBoard[1][4] = -4;
	gameBoard[0][5] = -4;
	gameBoard[1][5] = -4;
}

void dropSBlock() {
	gameBoard[0][4] = -5;
	gameBoard[1][4] = -5;
	gameBoard[1][5] = -5;
	gameBoard[2][5] = -5;
}

void dropTBlock() {
	gameBoard[0][4] = -6;
	gameBoard[1][5] = -6;
	gameBoard[1][4] = -6;
	gameBoard[1][3] = -6;
}

void dropZBlock() {
	gameBoard[0][5] = -7;
	gameBoard[1][5] = -7;
	gameBoard[1][4] = -7;
	gameBoard[2][4] = -7;
}

void tetris() {
	clock_t start, end; //keep track of time used for function (from https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/ )
	double time;
	start = clock();
    bool endGame = false;  //checks if game is over
    int prevBlock = 0; //store previous block id
    int num = 0; //random block generation
    int frameDelay = 48; //value to adjust speed of block drop
    int frame = 0; //current frame value for block drop
    bool newBlock = true; //determine when a new block is needed
    int prevKey = 0; //key pressed in previous frame
    int DAS = 16; //held key move delay
    bool stopDrop = false; //flag to stop falling piece
    int blockState = 0; //index of block state (all states shown here: https://harddrop.com/w/images/0/07/NESTetris-pieces.png )
    int blockY = 0; //falling block top left corner x coord
    int blockX = 0; //falling block top left corner y coord
    while(!endGame) {  //runs while game not over
    	keyboardDriver(); //get new keypress

    	//spawn new block
    	if(newBlock){
    		prevBlock = num;
    		num = spawnBlock(prevBlock);
    		blockY = 0;
    		blockX = 3;
    		switch(num){
    		case 1:
    			blockState = 2;
    			break;
    		case 2:
    			blockState = 5;
    			break;
    		case 3:
    			blockState = 11;
    			break;
    		case 4:
    			blockState = 3;
    			blockX = 4;
    			break;
    		case 5:
    			blockState = 13;
    			break;
    		case 6:
    			blockState = 16;
    			break;
    		case 7:
    			blockState = 19;
    			break;
    		default:
    			blockState = 0;
    			break;
    		}
    		newBlock = false;
    	}

    	//handle keyboard movement
    	if(keypress == 80 && prevKey == 80){ //movement left if key is held
    		if(DAS == 0){
    			blockX = shiftLeft(blockX);
    			DAS = 6;
    		}
    		DAS--;
    	}
    	else if(keypress == 80){ //movement when left key pressed
    		blockX = shiftLeft(blockX);
    		prevKey = 80;
    	}
    	else if(keypress == 79 && prevKey == 79){ // movement right if key is held
    		if(DAS == 0){
    			blockX = shiftRight(blockX);
    			DAS = 6;
    		}
    		DAS--;
    	}
    	else if (keypress == 79){ //movement when right key pressed
    		blockX = shiftRight(blockX);
    		prevKey = 79;
    	}
    	else if(keypress == 81 && prevKey == 81){ //soft drop when down key pressed
    		if(DAS == 0){
    			newBlock = drop();
    			DAS = frameDelay/2; //soft drop moves at double normal drop speed
    		}
    		DAS--;
    	}
    	else if(keypress == 81){
    		newBlock = drop();
    		if(!newBlock){
    			blockY += 1;
    		}
    		prevKey = 81;
    	}
    	else{ //reset if no key pressed
    		prevKey = 0;
    		DAS = 15;
    	}

    	//handle rotation (Right-Handed Nintendo Rotation System)
    	if(keypress == 29){ //rotate left on z
    		blockState = rotateLeft(blockState, blockX, blockY);
    	}
    	if(keypress == 27){ //rotate right on x
    		blockState = rotateRight(blockState, blockX, blockY);
    	}

    	//handle block auto-drop
    	if(frameDelay == frame){
    		newBlock = drop();
    		if(!newBlock){
    			blockY += 1;
    		}
    		frame = 0;
    	}
    	if(newBlock){
    		if(lineClear()){
    			usleep(160000); //10 frame drop delay
    			frame = 0; //reset drop timer
    		}
    		endGame = gameOver();
    	}
    	drawGame(gameBoard, 1, 15, 4, 15);
    	frame++;
    	end = clock();
    	time = ((double)(end - start))/CLOCKS_PER_SEC;
    	if(time < 16000){ //if function uses less than ~1/60 sec, wait until ~1/60 sec is complete
    		usleep(16000 - time);
    	}
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
			break;
	}
	return num;
}

int shiftLeft(int blockX){
	int count = 0; //when 4 cells have been shifted, shift is complete
	for(int i = 0; i < 20; i++){ //collision check
		if(gameBoard[i][0] < 0){ //left wall collision
			return blockX;
		}
		for(int j = 1; j < 10; j++){
			if(gameBoard[i][j] < 0 && gameBoard[i][j - 1] > 0){ //piece collision
				return blockX;
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
				return blockX - 1;
			}
		}
	}
}

int shiftRight(int blockX){
	int count = 0; //when 4 cells have been shifted, shift is complete
	for(int i = 0; i < 20; i++){ //collision check
		if(gameBoard[i][9] < 0){ //right wall collision
 			return blockX;
		}
		for(int j = 0; j < 9; j++){
			if(gameBoard[i][j] < 0 && gameBoard[i][j + 1 < 0]) //piece collision
				return blockX;
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
				return blockX + 1;
			}
		}
	}
}

bool drop() {
	bool stopDrop = false; //collision flag
	int count = 0; //when 4 cells have been moved, move is complete
	for(int i = 0; i < 20; i++){ //collision check
		for(int j = 0; j < 10; j++){
			if(gameBoard[i][j] < 0){
				if(i == 20 || gameBoard[i+1][j] != 0){
					stopDrop = true;
				}
				count++;
				if(count == 4){
					count = 0;
					goto DONE;
				}
			}
		}
	}
	DONE:
	if(stopDrop){ //lock piece in place and return true
		for(int i = 0; i < 20; i++){
			for(int j = 0; j < 10; j++){
				int current = gameBoard[i][j];
				if(current < 0){
					gameBoard[i][j] = abs(current);
					count++;
				}
				if(count == 4){
					lineClear();
					return true;
				}
			}
		}
		return true;
	}
	else{ //drop piece by one and return false
		for(int i = 18; i >= 0; i--){
			for(int j = 0; j < 10; j++){
				int current = gameBoard[i][j];
				if(current < 0){
					gameBoard[i+1][j] = current;
					count++;
				}
				if(count == 4){
					return false;
				}
			}
		}
		return false;
	}
}

bool lineClear(){
	int clears = 0; //line clear counter
	int clearLines[4] = {0}; //line clear indexes
	for(int i = 0; i < 20; i++){ //loop over board and mark full rows
		for(int j = 0; j < 10; j++){
			if(gameBoard[i][j] == 0){
				clearLines[clears] = i;
				clears++;
				break;
			}
		}
		if(clears == 4){ //maximum number of simultaneous line clears is 4
			break;
		}
	}
	if(clears == 0) {return false;}
	for(int i = 0; i < clears; i++){ //for all marked lines, move all lines above down by 1
		for(int j = clearLines[i]; j > 0; j--){
			for(int k = 0; k < 10; k++){
				gameBoard[j][k] = gameBoard[j-1][k];
			}
		}
	}
	return true;
}

bool gameOver(){
	for(int i = 0; i < 10; i++){
		if(gameBoard[0][i] > 0){
			return true;
		}
	}
	return false;
}

int rotateLeft(int blockState, int blockX, int blockY){
	int newState = blockState;
	switch(blockState){
	case 1:
		if(blockX >= 0  && blockX < 7){
			if(gameBoard[blockY][blockX + 2] == 0 && gameBoard[blockY  + 1][blockX + 2] && gameBoard[blockY + 3][blockX + 2] == 0){
				newState = 2;
				gameBoard[blockY][blockX + 2] = -1;
				gameBoard[blockY  + 1][blockX + 2] = -1;
				gameBoard[blockY + 3][blockX + 2] = -1;
				gameBoard[blockY + 2][blockX] = 0;
				gameBoard[blockY + 2][blockX + 1] = 0;
				gameBoard[blockY + 2][blockX + 3] = 0;
			}
		}
		break;
	case 2:
		if(blockX >= 0  && blockX < 7){
			if(gameBoard[blockY + 2][blockX] == 0 && gameBoard[blockY + 2][blockX + 1] && gameBoard[blockY + 2][blockX + 3] == 0){
				newState = 1;
				gameBoard[blockY][blockX + 2] = 0;
				gameBoard[blockY  + 1][blockX + 2] = 0;
				gameBoard[blockY + 3][blockX + 2] = 0;
				gameBoard[blockY + 2][blockX] = -1;
				gameBoard[blockY + 2][blockX + 1] = -1;
				gameBoard[blockY + 2][blockX + 3] = -1;
			}
		}
		break;
	case 3:
		newState = blockState;
		break;
	case 4:
		if(blockX > 0 && blockX < 8){
			if(gameBoard[blockY][blockX + 1] == 0 && gameBoard[blockY][blockX + 2] == 0 && gameBoard[blockY + 2][blockX + 1] == 0){
				newState = 7;
				gameBoard[blockY][blockX + 1] = -2;
				gameBoard[blockY][blockX + 2] = -2;
				gameBoard[blockY + 2][blockX + 1] = -2;
				gameBoard[blockY + 1][blockX] = 0;
				gameBoard[blockY + 1][blockX + 2] = 0;
				gameBoard[blockY + 2][blockX + 2] = 0;
			}
		}
		break;
	case 5:
		if(blockX > 0 && blockX < 8){
			if(gameBoard[blockY + 1][blockX] == 0 && gameBoard[blockY + 1][blockX + 2] == 0 && gameBoard[blockY + 2][blockX + 2] == 0){
				newState = 4;
				gameBoard[blockY + 1][blockX] = -2;
				gameBoard[blockY + 1][blockX + 2] = -2;
				gameBoard[blockY + 2][blockX + 2] = -2;
				gameBoard[blockY][blockX + 1] = 0;
				gameBoard[blockY + 2][blockX] = 0;
				gameBoard[blockY + 2][blockX + 1] = 0;
			}
		}
		break;
	case 6:
		if(blockX > 0 && blockX < 8){
			if(gameBoard[blockY][blockX + 1] == 0 && gameBoard[blockY + 2][blockX] == 0 && gameBoard[blockY + 2][blockX + 1] == 0){
				newState = 5;
				gameBoard[blockY][blockX + 1] = -2;
				gameBoard[blockY + 2][blockX] = -2;
				gameBoard[blockY + 2][blockX + 1] = -2;
				gameBoard[blockY][blockX] = 0;
				gameBoard[blockY + 1][blockX] = 0;
				gameBoard[blockY + 1][blockX + 1] = 0;
			}
		}
		break;
	case 7:
		if(blockX > 0 && blockX < 8){
			if(gameBoard[blockY][blockX] == 0 && gameBoard[blockY + 1][blockX] == 0 && gameBoard[blockY + 1][blockX + 1]){
				newState = 6;
				gameBoard[blockY][blockX] = -2;
				gameBoard[blockY + 1][blockX] = -2;
				gameBoard[blockY + 1][blockX + 1] = -2;
				gameBoard[blockY][blockX + 1] = 0;
				gameBoard[blockY][blockX + 2] = 0;
				gameBoard[blockY + 2][blockX + 1] = 0;
			}
		}
		break;
	case 8:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 9:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 10:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 11:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 12:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 13:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 14:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 15:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 16:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 17:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 18:
		if(blockX > 0 && blockX < 8){

		}
		break;
	case 19:
		if(blockX > 0 && blockX < 8){

		}
		break;
	default:
		newState = blockState;
		break;
	}
	return newState;
}

int rotateRight(int blockState int blockX, int blockY){

}
