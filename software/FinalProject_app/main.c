#include "text_mode_vga.h"
#include <stdio.h>
#include "tetris.h"
#include "usb_kb/MAX3421E.h"
#include "usb_kb/transfer.h"
#include "usb_kb.h"

int main(){
	printf("initializing MAX3421E...\n");
	MAX3421E_init();
	printf("initializing USB...\n");
	USB_init();
	clearScreen(); //initialize screen
	for (int i = 0; i < 16; i++) //set color palette
	{
		setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);
	}
	highscore = 0; //initialize high score
	for(int i = 0; i < 3; i++){ //initialize high scorer
		scorer[i] = 0;
	}
	while(1){ //run game
		drawHighScore(); //draw high score on title screen
		startScreen(); //show title screen
		int startLv = chooseLevel(); //run level select
		clearScreen(); //empty screen
		int score = tetris(startLv); //run game
		endGame(); //show game over text
		if(score > highscore){ //update high score and allow user to enter name
			highscore = score;
			clearScreen();
			enterHighScore();
		}
		clearScreen(); //return to title screen
	}
	return 0;
}
