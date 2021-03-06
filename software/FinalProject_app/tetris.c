
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "text_mode_vga.h"
#include <unistd.h>
#include "system.h"
#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "usb_kb/GenericMacros.h"
#include "usb_kb/GenericTypeDefs.h"
#include "usb_kb/HID.h"
#include "usb_kb/MAX3421E.h"
#include "usb_kb/transfer.h"
#include "usb_kb/usb_ch9.h"
#include "usb_kb/USB.h"
#include "usb_kb.h"
#include "text_mode_vga.h"

//I block: -1
//J Block: -2
//L block: -3
//O block: -4
//S-block: -5
//T block: -6
//Z block: -7

void dropIBlock() {
	draw2(3,0,-1);
}

void dropJBlock() {
	draw5(3,0,-2);
}

void dropLBlock() {
	draw11(3,0,-3);
}

void dropOBlock() {
	draw3(4,0,-4);
}

void dropSBlock() {
	draw13(3,0,-5);
}

void dropTBlock() {
	draw15(3,0,-6);
}

void dropZBlock() {
	draw19(3,0,-7);
}

int tetris(int startLevel) {
	srand(time(0)); //random seed based on time
	clock_t start, end; //keep track of time used for function (from https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/ )
	double time;
    bool endGame = false;  //checks if game is over
    int prevBlock = 0; //duplicate check
    int nextBlock = (rand() % 7) + 1; //random block generation
    int currBlock = 0;
    int level = startLevel;
    int frameDelay[19] = {24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 4, 4, 3, 3, 3, 2, 2, 2}; //value to adjust speed of block drop
    int softDrop[19] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1}; //softDrop speed by level
    int SDC; //soft drop frame counter
    int color1[10] = {1, 2, 5, 1, 12, 10, 4, 1, 9, 4}; //level color palette sets
    int color2[10] = {5, 10, 13, 2, 10, 9, 8, 4, 12, 14};
    int frame = 0; //current frame value for block drop
    bool newBlock = true; //determine when a new block is needed
    int prevKey = 0; //key pressed in previous frame
    int DAS = 6; //held key move delay
    int blockState = 0; //index of block state (all states shown here: https://harddrop.com/w/images/0/07/NESTetris-pieces.png )
    int Y = 0; //falling block top left corner x coord
    int X = 0; //falling block top left corner y coord
	int score = 0;
	int lineCount = 0; //overall line counter
	int levelLines = 0; //line counter for level up
	int I = 0; //piece counters for stats
	int J = 0;
	int L = 0;
	int O = 0;
	int S = 0;
	int T = 0;
	int Z = 0;
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			gameBoard[i][j] = 0;
		}
	}
	drawGrid(11, 0, 15);
	drawLevel(level);
	drawLines(lineCount);
	drawStatBlocks(color1[level % 10], 15, color2[level % 10], 15);
    while(!endGame) {  //runs while game not over
    	start = clock();
    	keycode = keyboardDriver(keycode); //get new keypress
    	//spawn new block
    	if(newBlock){
    		prevBlock = currBlock;
    		currBlock = nextBlock;
    		nextBlock = (rand() % 7) + 1;   //generate a random number corresponding to a block
    		//reroll if block is same as previous, biases against repeat blocks but does not prevent them
    		if(nextBlock == prevBlock){
    			nextBlock = (rand() % 7) + 1;
    		}
    		drawNext(0, 0, 0, 0, 0);
    		drawNext(nextBlock, color1[level % 10], 15, color2[level % 10], 15);
    		spawnBlock(currBlock);
    		Y = 0;
    		X = 3;
    		switch(currBlock){
    		case 1:
    			I++;
    			blockState = 2;
    			break;
    		case 2:
    			J++;
    			blockState = 5;
    			break;
    		case 3:
    			L++;
    			blockState = 11;
    			break;
    		case 4:
    			O++;
    			blockState = 3;
    			X = 4;
    			break;
    		case 5:
    			S++;
    			blockState = 13;
    			break;
    		case 6:
    			T++;
    			blockState = 15;
    			break;
    		case 7:
    			Z++;
    			blockState = 19;
    			break;
    		default:
    			blockState = 0;
    			break;
    		}
    		drawStats(T, J, Z, O, S, L, I);
    		newBlock = false;
    	}
    	//handle keyboard movement
    	if(keycode == 80 && prevKey == 80){ //movement left if key is held
    		if(DAS == 0){
    			if(shiftLeft(blockState, X, Y)){
    				X -= 1;
    			}
    			DAS = 3;
    		}
    		DAS--;
    	}
    	else if(keycode == 80){ //movement when left key pressed
			if(shiftLeft(blockState, X, Y)){
				X -= 1;
			}
    		prevKey = 80;
    	}
    	else if(keycode == 79 && prevKey == 79){ // movement right if key is held
    		if(DAS == 0){
    			if(shiftRight(blockState, X, Y)){
    				X += 1;
    			}
    			DAS = 3;
    		}
    		DAS--;
    	}
    	else if (keycode == 79){ //movement when right key pressed
			if(shiftRight(blockState, X, Y)){
				X += 1;
			}
    		prevKey = 79;
    	}
    	else if(keycode == 81 && prevKey == 81){ //soft drop when down key pressed
    		if(SDC == 0){
    			score += 1; //1 point per tile soft dropped
    			newBlock = drop(blockState, X, Y);
				if(!newBlock){
					Y += 1;
				}
    			if(level < 19){
    				SDC = softDrop[level] + 1; //soft drop moves at double normal drop speed
    			}
    			else{
    				SDC = 0;
    			}
    		}
    		SDC--;
    	}
    	else if(keycode == 81){
    		score += 1; //1 point per tile soft dropped
    		newBlock = drop(blockState, X, Y);
    		if(!newBlock){
    			Y += 1;
    		}
    		if(level < 19){
    			SDC = softDrop[level];
    		}
    		else{
    			SDC = 0;
    		}
    		prevKey = 81;
    	}
    	else if(keycode == 41){
    		drawPause();
    		usleep(100000);
    		keycode = keyboardDriver(keycode);
    		while(keycode != 41 && keycode != 40){ //enter and esc both unpause
    			keycode = keyboardDriver(keycode);
    		}
    		unpause();
    		usleep(100000);
    	}
    	else{ //reset if no key pressed
    		prevKey = 0;
    		DAS = 6;
    		if(level < 19){
    			SDC = softDrop[level];
    		}
    		else{
    			SDC = 0;
    		}
    	}

    	//handle rotation (Right-Handed Nintendo Rotation System)
    	if(keycode == 29){ //rotate left on z
    		blockState = rotateLeft(blockState, X, Y);
    	}
    	if(keycode == 27){ //rotate right on x
    		blockState = rotateRight(blockState, X, Y);
    	}

    	//handle block auto-drop
    	if(level < 19){
			if(frameDelay[level] == frame){
				newBlock = drop(blockState, X, Y);
				if(!newBlock){
					Y += 1;
				}
				frame = 0;
			}
    	}
    	else{
			if(1 == frame){
				newBlock = drop(blockState, X, Y);
				if(!newBlock){
					Y += 1;
				}
				frame = 0;
			}
    	}

    	//handle line clearing and end game
    	if(newBlock){
    		bool addscore = false;
    		int lines = lineClear(Y);
    		if(lines > 0){
    			addscore = true;
    		}
    		if(addscore){
        		lineCount += lines;
        		levelLines += lines;
        		if(lines == 1){
        			score += 40*(level + 1);
        		}
        		if(lines == 2){
        			score += 100*(level + 1);
        		}
        		if(lines == 3){
        			score += 300*(level + 1);
        		}
        		if(lines == 4){
        			score += 1200*(level + 1);
        		}
        		drawLines(lineCount);
    			//usleep(100000); //drop delay
    			frame = 0; //reset drop timer
    			/*if(level == startLevel){
    				if(lineCount >= (startLevel * 10) + 10 || lineCount >= max((startLevel*10) - 50), 100){
    					level += 1;
    					levelLines = 0;
    				}
    			}*/
    			if(levelLines > 10){ //level up every10 lines
    				level += 1;
    				drawLevel(level);
    				drawStatBlocks(color1[level % 10], 15, color2[level % 10], 15);
    				levelLines = 0;
    			}
    		}
    		endGame = gameOver();
    	}
    	drawGame(gameBoard, color1[level % 10], 15, color2[level % 10], 15);
    	drawScore(score);
    	frame++;
    	end = clock();
    	time = ((double) (end - start)) / CLOCKS_PER_SEC;
    	time *= 1000000;
    	if(time < 33000){
    		usleep(33000 - time);
    	}
    }
    return score;
}

int max(int a, int b){
	if(a > b){
		return a;
	}
	return b;
}

void spawnBlock(int Block){
	switch(Block) {
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
			dropOBlock();
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
}

bool shiftLeft(int blockState, int X, int Y){
	switch(blockState){
	case 1:
		if(X == 0 || gameBoard[Y + 2][X - 1] > 0){
			return false;
		}
		draw1(X, Y, 0);
		draw1(X - 1, Y, -1);
		return true;
	case 2:
		if(X == -2){
			return false;
		}
		for(int i = 0; i < 4; i++){
			if(gameBoard[Y + i][X + 1] > 0){
				return false;
			}
		}
		draw2(X, Y, 0);
		draw2(X - 1, Y, -1);
		return true;
	case 3:
		if(X == 0 || gameBoard[Y][X - 1] > 0 || gameBoard[Y + 1][X - 1] > 0){
			return false;
		}
		draw3(X, Y, 0);
		draw3(X - 1, Y, -4);
		return true;
	case 4:
		if(X == 0 || gameBoard[Y + 1][X - 1] > 0 || gameBoard[Y + 2][X + 1] > 0){
			return false;
		}
		draw4(X, Y, 0);
		draw4(X - 1, Y, -2);
		return true;
	case 5:
		if(X == 0 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X - 1] > 0){
			return false;
		}
		draw5(X, Y, 0);
		draw5(X - 1, Y, -2);
		return true;
	case 6:
		if(X == 0 || gameBoard[Y][X - 1] > 0 || gameBoard[Y + 1][X - 1] > 0){
			return false;
		}
		draw6(X, Y, 0);
		draw6(X - 1, Y, -2);
		return true;
	case 7:
		if(X == -1 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw7(X, Y, 0);
		draw7(X - 1, Y, -2);
		return true;
	case 8:
		if(X == 0 || gameBoard[Y + 1][X - 1] > 0 || gameBoard[Y + 2][X - 1] > 0){
			return false;
		}
		draw8(X, Y, 0);
		draw8(X - 1, Y, -3);
		return true;
	case 9:
		if(X == 0 || gameBoard[Y][X - 1] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		for(int i = 0; i < 3; i++){
			gameBoard[Y + i][X + 1] = 0;
		}
		draw9(X, Y, 0);
		draw9(X - 1, Y, -3);
		return true;
	case 10:
		if(X == 0 || gameBoard[Y][X + 1] > 0 || gameBoard[Y + 1][X - 1] > 0){
			return false;
		}
		draw10(X, Y, 0);
		draw10(X - 1, Y, -3);
		return true;
	case 11:
		if(X == -1 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw11(X, Y, 0);
		draw11(X - 1, Y, -3);
		return true;
	case 12:
		if(X == 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X - 1] > 0){
			return false;
		}
		draw12(X, Y, 0);
		draw12(X - 1, Y, -5);
		return true;
	case 13:
		if(X == -1 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X + 1] > 0){
			return false;
		}
		draw13(X, Y, 0);
		draw13(X - 1, Y, -5);
		return true;
	case 14:
		if(X == 0 || gameBoard[Y + 1][X - 1] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw14(X, Y, 0);
		draw14(X - 1, Y, -6);
		return true;
	case 15:
		if(X == 0 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X - 1] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw15(X, Y, 0);
		draw15(X - 1, Y, -6);
		return true;
	case 16:
		if(X == 0 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X - 1] > 0){
			return false;
		}
		draw16(X, Y, 0);
		draw16(X - 1, Y, -6);
		return true;
	case 17:
		if(X == -1 || gameBoard[Y][X] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw17(X, Y, 0);
		draw17(X - 1, Y, -6);
		return true;
	case 18:
		if(X == 0 || gameBoard[Y + 1][X - 1] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw18(X, Y, 0);
		draw18(X - 1, Y, -7);
		return true;
	case 19:
		if(X == -1 || gameBoard[Y][X + 1] > 0 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 2][X] > 0){
			return false;
		}
		draw19(X, Y, 0);
		draw19(X - 1, Y, -7);
		return true;
	default:
		return false;
	}
}

bool shiftRight(int blockState, int X, int Y){
	switch(blockState){
	case 1:
		if(X == 6 || gameBoard[Y + 2][X + 4] > 0){
			return false;
		}
		draw1(X, Y, 0);
		draw1(X + 1, Y, -1);
		return true;
	case 2:
		if(X == 7){
			return false;
		}
		for(int i = 0; i < 4; i++){
			if(gameBoard[Y + i][X + 3] > 0){
				return false;
			}
		}
		draw2(X, Y, 0);
		draw2(X + 1, Y, -1);
		return true;
	case 3:
		if(X == 8 || gameBoard[Y][X + 2] > 0 || gameBoard[Y + 1][X + 2] > 0){
			return false;
		}
		draw3(X, Y, 0);
		draw3(X + 1, Y, -4);
		return true;
	case 4:
		if(X == 7 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 3] > 0){
			return false;
		}
		draw4(X, Y, 0);
		draw4(X + 1, Y, -2);
		return true;
	case 5:
		if(X == 8){
			return false;
		}
		for(int i = 0; i < 3; i++){
			if(gameBoard[Y + i][X + 2] > 0){
				return false;
			}
		}
		draw5(X, Y, 0);
		draw5(X + 1, Y, -2);
		return true;
	case 6:
		if(X == 7 || gameBoard[Y][X + 1] > 0 || gameBoard[Y + 1][X + 3] > 0){
			return false;
		}
		draw6(X, Y, 0);
		draw6(X + 1, Y, -2);
		return true;
	case 7:
		if(X == 7 || gameBoard[Y][X +3] > 0 || gameBoard[Y + 1][X + 2] > 0 || gameBoard[Y + 2][X + 2] > 0){
			return false;
		}
		draw7(X, Y, 0);
		draw7(X + 1, Y, -2);
		return true;
	case 8:
		if(X == 7 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 1] > 0){
			return false;
		}
		draw8(X, Y, 0);
		draw8(X + 1, Y, -3);
		return true;
	case 9:
		if(X == 8){
			return false;
		}
		for(int i = 0; i < 3; i++){
			if(gameBoard[Y + i][X + 2] > 0){
				return false;
			}
		}
		draw9(X, Y, 0);
		draw9(X + 1, Y, -3);
		return true;
	case 10:
		if(X == 7 || gameBoard[Y][X + 3] > 0 || gameBoard[Y + 1][X + 3] > 0){
			return false;
		}
		draw10(X, Y, 0);
		draw10(X + 1, Y, -3);
		return true;
	case 11:
		if(X == 7 || gameBoard[Y][X + 2] > 0 || gameBoard[Y + 1][X + 2] > 0 || gameBoard[Y + 2][X + 3] > 0){
			return false;
		}
		draw11(X, Y, 0);
		draw11(X + 1, Y, -3);
		return true;
	case 12:
		if(X == 7 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 2] > 0){
			return false;
		}
		draw12(X, Y, 0);
		draw12(X + 1, Y, -5);
		return true;
	case 13:
		if(X == 7 || gameBoard[Y][X + 2] > 0 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 3] > 0){
			return false;
		}
		draw13(X, Y, 0);
		draw13(X + 1, Y, -5);
		return true;
	case 14:
		if(X == 7 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 2] > 0){
			return false;
		}
		draw14(X, Y, 0);
		draw14(X + 1, Y, -6);
		return true;
	case 15:
		if(X == 8){
			return false;
		}
		for(int i = 0; i < 3; i++){
			if(gameBoard[Y + i][X + 2] > 0){
				return false;
			}
		}
		draw15(X, Y, 0);
		draw15(X + 1, Y, -6);
		return true;
	case 16:
		if(X == 7 || gameBoard[Y][X + 2] > 0 || gameBoard[Y + 1][X + 3] > 0){
			return false;
		}
		draw16(X, Y, 0);
		draw16(X + 1, Y, -6);
		return true;
	case 17:
		if(X == 7 || gameBoard[Y][X + 2] > 0 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 2] > 0){
			return false;
		}
		draw17(X, Y, 0);
		draw17(X + 1, Y, -6);
		return true;
	case 18:
		if(X == 7 || gameBoard[Y + 1][X + 2] > 0 || gameBoard[Y + 2][X + 3] > 0){
			return false;
		}
		draw18(X, Y, 0);
		draw18(X + 1, Y, -7);
		return true;
	case 19:
		if(X == 7 || gameBoard[Y][X + 3] > 0 || gameBoard[Y + 1][X + 3] > 0 || gameBoard[Y + 2][X + 2] > 0){
			return false;
		}
		draw19(X, Y, 0);
		draw19(X + 1, Y, -7);
		return true;
	default:
		return false;
	}
}

bool drop(int blockState, int X, int Y) {
	bool stopDrop = false;
	switch(blockState){
	case 1:
		if(Y == 17){
			stopDrop = true;
		}
		else{
			for(int i = 0; i < 4; i++){
				if(gameBoard[Y + 3][X + i] > 0){
					stopDrop = true;
				}
			}
		}
		if(stopDrop){
			draw1(X, Y, 0);
			draw1(X, Y, 1);
			return true;
		}
		else{
			draw1(X, Y, 0);
			draw1(X, Y + 1, -1);
			return false;
		}
	case 2:
		if(Y == 16 || gameBoard[Y + 4][X + 2] > 0){
			draw2(X, Y, 0);
			draw2(X, Y, 1);
			return true;
		}
		else{
			draw2(X, Y, 0);
			draw2(X, Y + 1, -1);
			return false;
		}
	case 3:
		if(Y == 18 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 1] > 0){
			draw3(X, Y, 0);
			draw3(X, Y, 4);
			return true;
		}
		else{
			draw3(X, Y, 0);
			draw3(X, Y + 1, -4);
			return false;
		}
	case 4:
		if(Y == 17 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 3][X + 2] > 0){
			draw4(X, Y, 0);
			draw4(X, Y, 2);
			return true;
		}
		else{
			draw4(X, Y, 0);
			draw4(X, Y + 1, -2);
			return false;
		}
	case 5:
		if(Y == 17 || gameBoard[Y + 3][X] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw5(X, Y, 0);
			draw5(X, Y, 2);
			return true;
		}
		else{
			draw5(X, Y, 0);
			draw5(X, Y + 1, -2);
			return false;
		}
	case 6:
		if(Y == 18 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw6(X, Y, 0);
			draw6(X, Y, 2);
			return true;
		}
		else{
			draw6(X, Y, 0);
			draw6(X, Y + 1, -2);
			return false;
		}
	case 7:
		if(Y == 17 || gameBoard[Y + 1][X + 2] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw7(X, Y, 0);
			draw7(X, Y, 2);
			return true;
		}
		else{
			draw7(X, Y, 0);
			draw7(X, Y + 1, -2);
			return false;
		}
	case 8:
		if(Y == 17 || gameBoard[Y + 3][X] > 0 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw8(X, Y, 0);
			draw8(X, Y, 3);
			return true;
		}
		else{
			draw8(X, Y, 0);
			draw8(X, Y + 1, -3);
			return false;
		}
	case 9:
		if(Y == 17 || gameBoard[Y + 1][X] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw9(X, Y, 0);
			draw9(X, Y, 3);
			return true;
		}
		else{
			draw9(X, Y, 0);
			draw9(X, Y + 1, -3);
			return false;
		}
	case 10:
		if(Y == 18 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw10(X, Y, 0);
			draw10(X, Y, 3);
			return true;
		}
		else{
			draw10(X, Y, 0);
			draw10(X, Y + 1, -3);
			return false;
		}
	case 11:
		if(Y == 17 || gameBoard[Y + 3][X + 1] > 0 || gameBoard[Y + 3][X + 2] > 0){
			draw11(X, Y, 0);
			draw11(X, Y, 3);
			return true;
		}
		else{
			draw11(X, Y, 0);
			draw11(X, Y + 1, -3);
			return false;
		}
	case 12:
		if(Y == 17 || gameBoard[Y + 3][X] > 0 || gameBoard[Y + 3][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw12(X, Y, 0);
			draw12(X, Y, 5);
			return true;
		}
		else{
			draw12(X, Y, 0);
			draw12(X, Y + 1, -5);
			return false;
		}
	case 13:
		if(Y == 17 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 3][X + 2] > 0){
			draw13(X, Y, 0);
			draw13(X, Y, 5);
			return true;
		}
		else{
			draw13(X, Y, 0);
			draw13(X, Y + 1, -5);
			return false;
		}
	case 14:
		if(Y == 17 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 2] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw14(X, Y, 0);
			draw14(X, Y, 6);
			return true;
		}
		else{
			draw14(X, Y, 0);
			draw14(X, Y + 1, -6);
			return false;
		}
	case 15:
		if(Y == 17 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw15(X, Y, 0);
			draw15(X, Y, 6);
			return true;
		}
		else{
			draw15(X, Y, 0);
			draw15(X, Y + 1, -6);
			return false;
		}
	case 16:
		if(Y == 18 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 2][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw16(X, Y, 0);
			draw16(X, Y, 6);
			return true;
		}
		else{
			draw16(X, Y, 0);
			draw16(X, Y + 1, -6);
			return false;
		}
	case 17:
		if(Y == 17 || gameBoard[Y + 3][X + 1] > 0 || gameBoard[Y + 2][X + 2] > 0){
			draw17(X, Y, 0);
			draw17(X, Y, 6);
			return true;
		}
		else{
			draw17(X, Y, 0);
			draw17(X, Y + 1, -6);
			return false;
		}
	case 18:
		if(Y == 17 || gameBoard[Y + 2][X] > 0 || gameBoard[Y + 3][X + 1] > 0 || gameBoard[Y + 3][ X + 2] > 0){
			draw18(X, Y, 0);
			draw18(X, Y, 7);
			return true;
		}
		else{
			draw18(X, Y, 0);
			draw18(X, Y + 1, -7);
			return false;
		}
	case 19:
		if(Y == 17 || gameBoard[Y + 2][X + 2] > 0 || gameBoard[Y + 3][X + 1] > 0){
			draw19(X, Y, 0);
			draw19(X, Y, 7);
			return true;
		}
		else{
			draw19(X, Y, 0);
			draw19(X, Y + 1, -7);
			return false;
		}
	default:
		return false;
	}
}

int lineClear(int Y){
	int clears = 0; //line clear counter
	int clearLines[4] = {0}; //line clear indexes
	for(int i = 0; i < 4; i++){ //for all possible lines to clear, find full lines and add them to clearLines array
		for(int j = 0; j < 10; j++){
			if(gameBoard[Y + i][j] <= 0){
				break;
			}
			if(j == 9){
				clearLines[clears] = Y + i;
				clears++;
			}
		}
	}
	if(clears == 0) {return false;}
	for(int i = 0; i < 5; i++){ //line clearing animation
		for(int j = 0; j < clears; j++){
			vga_ctrl->VRAM[(215 + (4 - i) + 40 * clearLines[j]) * 2] = 0x0;
			vga_ctrl->VRAM[(215 + (4 - i) + 40 * clearLines[j]) * 2 + 1] = 0x0;
			vga_ctrl->VRAM[(215 + (5 + i) + 40 * clearLines[j]) * 2] = 0x0;
			vga_ctrl->VRAM[(215 + (5 + i) + 40 * clearLines[j]) * 2 + 1] = 0x0;
			if(clears == 4){ //flashing animation for 4 clears
				if(i % 2 == 1){
					for(int k = 0; k < 10; k++){
						vga_ctrl->VRAM[(215 + k + 40 * clearLines[j]) * 2] = 15 << 4 | 15;
					}
				}
				else{
					for(int k = 0; k < 10; k++){
						vga_ctrl->VRAM[(215 + k + 40 * clearLines[j]) * 2] = 0 << 4 | 0;
					}
				}
			}
		}
		usleep(10000);
	}
	for(int i = 0; i < clears; i++){ //for all marked lines, move all lines above down by 1
		for(int j = clearLines[i]; j > 0; j--){
			for(int k = 0; k < 10; k++){
				gameBoard[j][k] = gameBoard[j-1][k];
			}
		}
	}
	return clears;
}

bool gameOver(){
	for(int i = 0; i < 10; i++){
		if(gameBoard[0][i] != 0){
			return true;
		}
	}
	return false;
}

int rotateLeft(int blockState, int X, int Y){
	int newState = blockState;
	switch(blockState){
	case 1:
		if(X >= 0  && X < 7){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y  + 1][X + 2] == 0 && gameBoard[Y + 3][X + 2] == 0){
				newState = 2;
				draw1(X, Y, 0);
				draw2(X, Y, -1);
			}
		}
		break;
	case 2:
		if(X >= 0 && X < 7){
			if(gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0 && gameBoard[Y + 2][X + 3] == 0){
				newState = 1;
				draw2(X, Y, 0);
				draw1(X, Y, -1);
			}
		}
		break;
	case 3:
		newState = blockState;
		break;
	case 4:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y][X + 2] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 7;
				draw4(X, Y, 0);
				draw7(X, Y, -2);
			}
		}
		break;
	case 5:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 4;
				draw5(X, Y, 0);
				draw4(X, Y, -2);
			}
		}
		break;
	case 6:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 5;
				draw6(X, Y, 0);
				draw5(X, Y, -2);
			}
		}
		break;
	case 7:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X] == 0 && gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0){
				newState = 6;
				draw7(X, Y, 0);
				draw6(X, Y, -2);
			}
		}
		break;
	case 8:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 1] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 11;
				draw8(X, Y, 0);
				draw11(X, Y, -3);
			}
		}
		break;
	case 9:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0 && gameBoard[Y + 2][X] == 0){
				newState = 8;
				draw9(X, Y, 0);
				draw8(X, Y, -3);
			}
		}
		break;
	case 10:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X] == 0 && gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 9;
				draw10(X, Y, 0);
				draw9(X, Y, -3);
			}
		}
		break;
	case 11:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0){
				newState = 10;
				draw11(X, Y, 0);
				draw10(X, Y, -3);
			}
		}
		break;
	case 12:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 13;
				draw12(X, Y, 0);
				draw13(X, Y, -5);
			}
		}
		break;
	case 13:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 12;
				draw13(X, Y, 0);
				draw12(X, Y, -5);
			}
		}
		break;
	case 14:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0){
				newState = 17;
				draw14(X, Y, 0);
				draw17(X, Y, -6);
			}
		}
		break;
	case 15:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X + 2] == 0){
				newState = 14;
				draw15(X, Y, 0);
				draw14(X, Y, -6);
			}
		}
		break;
	case 16:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 2][X + 1] == 0){
				newState = 15;
				draw16(X, Y, 0);
				draw15(X, Y, -6);
			}
		}
		break;
	case 17:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0){
				newState = 16;
				draw17(X, Y, 0);
				draw16(X, Y, -6);
			}
		}
		break;
	case 18:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y + 1][Y + 2] == 0){
				newState = 19;
				draw18(X, Y, 0);
				draw19(X, Y, -7);
			}
		}
		break;
	case 19:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 18;
				draw19(X, Y, 0);
				draw18(X, Y, -7);
			}
		}
		break;
	default:
		newState = blockState;
		break;
	}
	return newState;
}

int rotateRight(int blockState, int X, int Y){
	int newState = blockState;
	switch(blockState){
	case 1:
		if(X >= 0  && X < 7){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y + 1][X + 2] == 0 && gameBoard[Y + 3][X + 2] == 0){
				newState = 2;
				draw1(X, Y, 0);
				draw2(X, Y, -1);
			}
		}
		break;
	case 2:
		if(X >= 0  && X < 7){
			if(gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0 && gameBoard[Y + 2][X + 3] == 0){
				newState = 1;
				draw2(X, Y, 0);
				draw1(X, Y, -1);
			}
		}
		break;
	case 3:
		newState = blockState;
		break;
	case 4:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 5;
				draw4(X, Y, 0);
				draw5(X, Y, -2);
			}
		}
		break;
	case 5:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X] == 0 && gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0){
				newState = 6;
				draw5(X, Y, 0);
				draw6(X, Y, -2);
			}
		}
		break;
	case 6:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y][X + 2] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 7;
				draw6(X, Y, 0);
				draw7(X, Y, -2);
			}
		}
		break;
	case 7:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 4;
				draw7(X, Y, 0);
				draw4(X, Y, -2);
			}
		}
		break;
	case 8:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X] == 0 && gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 9;
				draw8(X, Y, 0);
				draw9(X, Y, -3);
			}
		}
		break;
	case 9:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0){
				newState = 10;
				draw9(X, Y, 0);
				draw10(X, Y, -3);
			}
		}
		break;
	case 10:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 1] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 11;
				draw10(X, Y, 0);
				draw11(X, Y, -3);
			}
		}
		break;
	case 11:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 1][X + 2] == 0 && gameBoard[Y + 2][X] == 0){
				newState = 8;
				draw11(X, Y, 0);
				draw8(X, Y, -3);
			}
		}
		break;
	case 12:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 13;
				draw12(X, Y, 0);
				draw13(X, Y, -5);
			}
		}
		break;
	case 13:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 2][X] == 0 && gameBoard[Y + 2][X + 1] == 0){
				newState = 12;
				draw13(X, Y, 0);
				draw12(X, Y, -5);
			}
		}
		break;
	case 14:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 1] == 0){
				newState = 15;
				draw14(X, Y, 0);
				draw15(X, Y, -6);
			}
		}
		break;
	case 15:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X + 2] == 0){
				newState = 16;
				draw15(X, Y, 0);
				draw16(X, Y, -6);
			}
		}
		break;
	case 16:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 2][X + 1] == 0){
				newState = 17;
				draw16(X, Y, 0);
				draw17(X, Y, -6);
			}
		}
		break;
	case 17:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0){
				newState = 14;
				draw17(X, Y, 0);
				draw14(X, Y, -6);
			}
		}
		break;
	case 18:
		if(X >= 0 && X < 8){
			if(gameBoard[Y][X + 2] == 0 && gameBoard[Y + 1][Y + 2] == 0){
				newState = 19;
				draw18(X, Y, 0);
				draw19(X, Y, -7);
			}
		}
		break;
	case 19:
		if(X >= 0 && X < 8){
			if(gameBoard[Y + 1][X] == 0 && gameBoard[Y + 2][X + 2] == 0){
				newState = 18;
				draw19(X, Y, 0);
				draw18(X, Y, -7);
			}
		}
		break;
	default:
		newState = blockState;
		break;
	}
	return newState;
}

void draw1(int X, int Y, int val) {
	for(int i = 0; i < 4; i++){
		gameBoard[Y + 2][X + i] = val;
	}
}

void draw2(int X, int Y, int val) {
	for(int i = 0; i < 4; i++){
		gameBoard[Y + i][X + 2] = val;
	}
}

void draw3(int X, int Y, int val) {
	gameBoard[Y][X] = val;
    gameBoard[Y + 1][X] = val;
    gameBoard[Y][X + 1] = val;
    gameBoard[Y + 1][X + 1] = val;
}

void draw4(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
    gameBoard[Y + 2][X + 2] = val;
}

void draw5(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
	gameBoard[Y + 2][X] = val;
}

void draw6(int X, int Y, int val) {
	gameBoard[Y][X] = val;
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
}

void draw7(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
	gameBoard[Y][X + 2] = val;
}

void draw8(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
	gameBoard[Y + 2][X] = val;
}

void draw9(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
	gameBoard[Y][X] = val;
}

void draw10(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
	gameBoard[Y][X + 2] = val;
}

void draw11(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
	gameBoard[Y + 2][X + 2] = val;
}

void draw12(int X, int Y, int val) {
	gameBoard[Y + 1][X + 1] = val;
	gameBoard[Y + 1][X + 2] = val;
	gameBoard[Y + 2][X] = val;
	gameBoard[Y + 2][X + 1] = val;
}

void draw13(int X, int Y, int val) {
	gameBoard[Y][X + 1] = val;
    gameBoard[Y + 1][X + 1] = val;
    gameBoard[Y + 1][X + 2] = val;
    gameBoard[Y + 2][X + 2] = val;
}

void draw14(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
	gameBoard[Y + 2][X + 1] = val;
}

void draw15(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
    gameBoard[Y + 1][X] = val;
}

void draw16(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + 1][X + i] = val;
	}
    gameBoard[Y][X + 1] = val;
}

void draw17(int X, int Y, int val) {
	for(int i = 0; i < 3; i++){
		gameBoard[Y + i][X + 1] = val;
	}
    gameBoard[Y + 1][X + 2] = val;
}

void draw18(int X, int Y, int val) {
	gameBoard[Y + 1][X] = val;
    gameBoard[Y + 1][X + 1] = val;
    gameBoard[Y + 2][X + 2] = val;
    gameBoard[Y + 2][X + 1] = val;
}

void draw19(int X, int Y, int val) {
	gameBoard[Y][X + 2] = val;
    gameBoard[Y + 1][X + 1] = val;
    gameBoard[Y + 1][X + 2] = val;
    gameBoard[Y + 2][X + 1] = val;
}
