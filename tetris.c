#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>

//I block: -1
//J Block: -2
//L block: -3
//O block: -4
//S-block: -5 
//T block: -6
//Z block: -7


//the next block to spawn on screen
void dropIBlock(int &board) {
    board[0][4] = -1;
    board[0][5] = -1;
    board[0][6] = -1;
    board[0][7] = -1;
}

void dropJBlock(int &board) {
    board[0][3] = -2;
    board[1][3] = -2;
    board[1][4] = -2;
    board[1][5] = -2;
}

void dropLBlock(int &board) {
    board[0][5] = -3;
    board[1][3] = -3;
    board[1][4] = -3;
    board[1][5] = -3;
}

void dropOBlock(int &board) {
    board[0][4] = -4;
    board[1][4] = -4;
    board[0][5] = -4;
    board[1][5] = -4;
}

void dropSBlock(int &board) {
    board[0][5] = -5;
    board[0][4] = -5;
    board[1][4] = -5;
    board[1][3] = -5;
}

void dropTlock(int &board) {
    board[0][5] = -6;
    board[1][3] = -6;
    board[1][4] = -6;
    board[1][5] = -6;
}

void dropZBlock(int &board) {
    board[0][3] = -7;
    board[0][4] = -7;
    board[1][4] = -7;
    board[1][5] = -7;
}

//function to test last one/two blocks are the same and adjust accordingly
void sameNums(int &sameNumFinder, int &counter, int &num) {
    if(counter % 3 == 0) {
        if(sameNumFinder[2] == num) {
            if(sameNumFinder[1] != num) {
                num = rand() % 7;
            }
        }
    }
    else if(counter % 3 == 1) {
        if(sameNumFinder[0] == num) {
            if(sameNumFinder[2] != num) {
                num = rand() % 7;
            }
        }
    }
    else if(counter % 3 == 2) {
        if(sameNumFinder[1] == num) {
            if(sameNumFinder[0] != num) {
                num = rand() % 7;
            }
        }
    }
    sameNumFinder[counter % 3] = num;
    counter++;
}

//updates the state of the board
void update(int &board) {

}

//checks if the rows on the board have been filled. if a row is full, clear it
void fullRow(int &board) {
    for(unsigned i = 0; i < 20; i++) {
        for(unsigned j = 0; j < 10; j++) {
            if(board[i][j] == 0) {
                break;
            }
            if(j == 9) {
                for(int k = 0; k < 10; k++) {
                    board[i][k] = 0;
                }
            }
        }
    }
}




int main() {
    int board[20][10];
    bool gameOver = false;  //checks if game is over
    int sameNumFinder[3] = {-1, -1, -1}; //tests if rand() generates the same random numbers in a row
    int num;
    int counter = 0; //to help test for same number twice in a row
    while(!gameOver) {  //runs while game not over
        num = rand() % 7;   //generate a random number corresponding to a block
        sameNums(sameNumFinder, counter, num);
        switch(num) {
            case 0:
                dropIBlock(board);
                break;
            case 1:
                dropJBlock(board);
                break;
            case 2:
                dropLBlock(board);
                break;
            case 3:
                dopOBlock(board);
                break;
            case 4:
                dropSBlock(board);
                break;
            case 5:
                dropTBlock(board);
                break;
            case 6:
                dropZBlock(board);
                break;
        default: 
        }
    }

}
