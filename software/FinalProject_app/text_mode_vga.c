/*
 * text_mode_vga_color.c
 * Minimal driver for text mode VGA support
 * This is for Week 2, with color support
 *
 *  Created on: Oct 25, 2021
 *      Author: zuofu
 */

#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alt_types.h>
#include "text_mode_vga.h"
#include "tetris.h"
#include <time.h>

void drawGrid(alt_u8 foreground, alt_u8 background, alt_u8 text){
	//draw game grid
	//draw top left corner
	vga_ctrl->VRAM[174 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[174 * 2 + 1] = 0x03;
	//draw top wall
	for(int i = 0; i < 10; i++){
		vga_ctrl->VRAM[(175 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(175 + i) * 2 + 1] = 0x04;
	}
	//draw top right corner
	vga_ctrl->VRAM[185 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[185 * 2 + 1] = 0x05;
	//draw right wall
	for(int i = 0; i < 20; i++){
		vga_ctrl->VRAM[(225 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(225 + i * 40) * 2 + 1] = 0x06;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[1025 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[1025 * 2 + 1] = 0x07;
	//draw bottom wall
	for(int i = 0; i < 10; i++){
		vga_ctrl->VRAM[(1015 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(1015 + i) * 2 + 1] = 0x08;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[1014 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[1014 * 2 + 1] = 0x09;
	//draw left wall
	for(int i = 0; i < 20; i++){
		vga_ctrl->VRAM[(214 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(214 + i * 40) * 2 + 1] = 0x0a;
	}

	//draw next grid & words
	//draw top left corner
	vga_ctrl->VRAM[426 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[426 * 2 + 1] = 0x0b;
	//draw top wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(427 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(427 + i) * 2 + 1] = 0x0c;
	}
	//draw top right corner
	vga_ctrl->VRAM[431 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[431 * 2 + 1] = 0x0d;
	//draw right wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(471 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(471 + i * 40) * 2 + 1] = 0x0e;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[751 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[751 * 2 + 1] = 0x0f;
	//draw bottom wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(747 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(747 + i) * 2 + 1] = 0x10;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[746 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[746 * 2 + 1] = 0x11;
	//draw left wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(466 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(466 + i * 40) * 2 + 1] = 0x12;
	}
	//N
	vga_ctrl->VRAM[467 * 2] = text << 4 | background;
	vga_ctrl->VRAM[467 * 2 + 1] = 0x2d;
	//E
	vga_ctrl->VRAM[468 * 2] = text << 4 | background;
	vga_ctrl->VRAM[468 * 2 + 1] = 0x24;
	//X
	vga_ctrl->VRAM[469 * 2] = text << 4 | background;
	vga_ctrl->VRAM[469 * 2 + 1] = 0x37;
	//T
	vga_ctrl->VRAM[470 * 2] = text << 4 | background;
	vga_ctrl->VRAM[470 * 2 + 1] = 0x33;

	//draw stats grid & words
	//draw top left corner
	vga_ctrl->VRAM[41 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[41 * 2 + 1] = 0x0b;
	//draw top wall
	for(int i = 0; i < 11; i++){
		vga_ctrl->VRAM[(42 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(42 + i) * 2 + 1] = 0x0c;
	}
	//draw top right corner
	vga_ctrl->VRAM[53 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[53 * 2 + 1] = 0x0d;
	//draw right wall
	for(int i = 0; i < 24; i++){
		vga_ctrl->VRAM[(93 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(93 + i * 40) * 2 + 1] = 0x0e;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[1053 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[1053 * 2 + 1] = 0x0f;
	//draw bottom wall
	for(int i = 0; i < 11; i++){
		vga_ctrl->VRAM[(1042 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(1042 + i) * 2 + 1] = 0x10;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[1041 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[1041 * 2 + 1] = 0x11;
	//draw left wall
	for(int i = 0; i < 24; i++){
		vga_ctrl->VRAM[(81 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(81 + i * 40) * 2 + 1] = 0x12;
	}
	//S
	vga_ctrl->VRAM[123 * 2] = text << 4 | background;
	vga_ctrl->VRAM[123 * 2 + 1] = 0x32;
	//T
	vga_ctrl->VRAM[124 * 2] = text << 4 | background;
	vga_ctrl->VRAM[124 * 2 + 1] = 0x33;
	//A
	vga_ctrl->VRAM[125 * 2] = text << 4 | background;
	vga_ctrl->VRAM[125 * 2 + 1] = 0x20;
	//T
	vga_ctrl->VRAM[126 * 2] = text << 4 | background;
	vga_ctrl->VRAM[126 * 2 + 1] = 0x33;
	//I
	vga_ctrl->VRAM[127 * 2] = text << 4 | background;
	vga_ctrl->VRAM[127 * 2 + 1] = 0x28;
	//S
	vga_ctrl->VRAM[128 * 2] = text << 4 | background;
	vga_ctrl->VRAM[128 * 2 + 1] = 0x32;
	//T
	vga_ctrl->VRAM[129 * 2] = text << 4 | background;
	vga_ctrl->VRAM[129 * 2 + 1] = 0x33;
	//I
	vga_ctrl->VRAM[130 * 2] = text << 4 | background;
	vga_ctrl->VRAM[130 * 2 + 1] = 0x28;
	//C
	vga_ctrl->VRAM[131 * 2] = text << 4 | background;
	vga_ctrl->VRAM[131 * 2 + 1] = 0x22;

	//draw score grid & words
	//draw top left corner
	vga_ctrl->VRAM[66 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[66 * 2 + 1] = 0x0b;
	//draw top wall
	for(int i = 0; i < 9; i++){
		vga_ctrl->VRAM[(67 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(67 + i) * 2 + 1] = 0x0c;
	}
	//draw top right corner
	vga_ctrl->VRAM[76 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[76 * 2 + 1] = 0x0d;
	//draw right wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(116 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(116 + i * 40) * 2 + 1] = 0x0e;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[396 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[396 * 2 + 1] = 0x0f;
	//draw bottom wall
	for(int i = 0; i < 9; i++){
		vga_ctrl->VRAM[(387 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(387 + i) * 2 + 1] = 0x10;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[386 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[386 * 2 + 1] = 0x11;
	//draw left wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(106 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(106 + i * 40) * 2 + 1] = 0x12;
	}
	for(int i = 0; i < 5; i++){
		vga_ctrl->VRAM[108 + i * 2] = text << 4 | background;
		vga_ctrl->VRAM[228 + i * 2] = text << 4 | background;
	}
	//S
	vga_ctrl->VRAM[148 * 2] = text << 4 | background;
	vga_ctrl->VRAM[148 * 2 + 1] = 0x32;
	//C
	vga_ctrl->VRAM[149 * 2] = text << 4 | background;
	vga_ctrl->VRAM[149 * 2 + 1] = 0x22;
	//O
	vga_ctrl->VRAM[150 * 2] = text << 4 | background;
	vga_ctrl->VRAM[150 * 2 + 1] = 0x2e;
	//R
	vga_ctrl->VRAM[151 * 2] = text << 4 | background;
	vga_ctrl->VRAM[151 * 2 + 1] = 0x31;
	//E
	vga_ctrl->VRAM[152 * 2] = text << 4 | background;
	vga_ctrl->VRAM[152 * 2 + 1] = 0x24;
	//L
	vga_ctrl->VRAM[268 * 2] = text << 4 | background;
	vga_ctrl->VRAM[268 * 2 + 1] = 0x2b;
	//I
	vga_ctrl->VRAM[269 * 2] = text << 4 | background;
	vga_ctrl->VRAM[269 * 2 + 1] = 0x28;
	//N
	vga_ctrl->VRAM[270 * 2] = text << 4 | background;
	vga_ctrl->VRAM[270 * 2 + 1] = 0x2d;
	//E
	vga_ctrl->VRAM[271 * 2] = text << 4 | background;
	vga_ctrl->VRAM[271 * 2 + 1] = 0x24;
	//S
	vga_ctrl->VRAM[272 * 2] = text << 4 | background;
	vga_ctrl->VRAM[272 * 2 + 1] = 0x32;

	//draw level & words
	//draw top left corner
	vga_ctrl->VRAM[786 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[786 * 2 + 1] = 0x0b;
	//draw top wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(787 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(787 + i) * 2 + 1] = 0x0c;
	}
	//draw top right corner
	vga_ctrl->VRAM[794 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[794 * 2 + 1] = 0x0d;
	//draw right wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(834 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(834 + i * 40) * 2 + 1] = 0x0e;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[994 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[994 * 2 + 1] = 0x0f;
	//draw bottom wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(987 + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(987 + i) * 2 + 1] = 0x10;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[986 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[986 * 2 + 1] = 0x11;
	//draw left wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(826 + i * 40) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(826 + i * 40) * 2 + 1] = 0x12;
	}
	for(int i = 0; i < 5; i++){
		vga_ctrl->VRAM[868 + i * 2] = text << 4 | background;
	}
	//L
	vga_ctrl->VRAM[868 * 2] = text << 4 | background;
	vga_ctrl->VRAM[868 * 2 + 1] = 0x2b;
	//E
	vga_ctrl->VRAM[869 * 2] = text << 4 | background;
	vga_ctrl->VRAM[869 * 2 + 1] = 0x24;
	//V
	vga_ctrl->VRAM[870 * 2] = text << 4 | background;
	vga_ctrl->VRAM[870 * 2 + 1] = 0x35;
	//E
	vga_ctrl->VRAM[871 * 2] = text << 4 | background;
	vga_ctrl->VRAM[871 * 2 + 1] = 0x24;
	//L
	vga_ctrl->VRAM[872 * 2] = text << 4 | background;
	vga_ctrl->VRAM[872 * 2 + 1] = 0x2b;
}

void drawDigit(int digit, int location, alt_u8 text, alt_u8 background){
	alt_u8 ch;
	switch(digit){
	case 1:
		ch = 0x3a;
		break;
	case 2:
		ch = 0x3b;
		break;
	case 3:
		ch = 0x3c;
		break;
	case 4:
		ch = 0x3d;
		break;
	case 5:
		ch = 0x3e;
		break;
	case 6:
		ch = 0x3f;
		break;
	case 7:
		ch = 0x40;
		break;
	case 8:
		ch = 0x41;
		break;
	case 9:
		ch = 0x42;
		break;
	case 0:
		ch = 0x43;
		break;
	}
	vga_ctrl->VRAM[location * 2] = text << 4 | background;
	vga_ctrl->VRAM[location * 2 + 1] = ch;
}

void drawLevel(int level){
	int dig = level % 10;
	drawDigit(dig, 912, 15, 0);
	level /= 10;
	dig = level % 10;
	drawDigit(dig, 911, 15, 0);
}

void drawScore(int score){
	for(int i = 0; i < 7; i++){
		int dig = score % 10;
		drawDigit(dig, 194 - i, 15, 0);
		score /= 10;
	}
}

void drawLines(int lines){
	for(int i = 0; i < 4; i++){
		int dig = lines % 10;
		drawDigit(dig, 311 - i, 15, 0);
		lines /= 10;
	}
}

void drawGame(int gameboard[20][10], alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2){
	//iterate over full array
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			int block = abs(gameboard[i][j]);
			if(block == 0){
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = 0x0;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x0;
			}
			if(block == 1){ //draw I block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg1 << 4 | bg1;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x01;
			}
			else if(block == 2){ //draw J block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg1 << 4 | bg1;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x02;
			}
			else if(block == 3){ //draw L block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg2 << 4 | bg2;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x02;
			}
			else if(block == 4){ //draw O block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg1 << 4 | bg1;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x01;
			}
			else if(block == 5){ //draw S block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg1 << 4 | bg1;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x02;
			}
			else if(block == 6){ //draw T block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg1 << 4 | bg1;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x01;
			}
			else if(block == 7){ //draw Z block
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2] = fg2 << 4 | bg2;
				vga_ctrl->VRAM[(215 + j + 40 * i) * 2 + 1] = 0x2;
			}
		}
	}
}

void drawStatBlocks(alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2){
	//T block
	vga_ctrl->VRAM[204 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[204 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[205 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[205 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[206 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[206 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[245 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[245 * 2 + 1] = 0x01;

	//J block
	vga_ctrl->VRAM[324 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[324 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[325 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[325 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[326 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[326 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[366 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[366 * 2 + 1] = 0x02;

	//Z block
	vga_ctrl->VRAM[444 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[444 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[445 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[445 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[485 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[485 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[486 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[486 * 2 + 1] = 0x02;

	//O block
	vga_ctrl->VRAM[565 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[565 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[605 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[605 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[566 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[566 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[606 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[606 * 2 + 1] = 0x01;

	//S block
	vga_ctrl->VRAM[685 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[685 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[686 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[686 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[724 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[724 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[725 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[725 * 2 + 1] = 0x02;

	//L block
	vga_ctrl->VRAM[804 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[804 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[805 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[805 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[806 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[806 * 2 + 1] = 0x02;
	vga_ctrl->VRAM[844 * 2] = fg2 << 4 | bg2;
	vga_ctrl->VRAM[844 * 2 + 1] = 0x02;

	//I block
	vga_ctrl->VRAM[924 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[924 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[925 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[925 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[926 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[926 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[927 * 2] = fg1 << 4 | bg1;
	vga_ctrl->VRAM[927 * 2 + 1] = 0x01;
}

void drawNext(int block, alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2){
	switch(block){
	case 1:
		vga_ctrl->VRAM[548 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[548 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[588 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[588 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[628 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[668 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x01;
		break;
	case 2:
		vga_ctrl->VRAM[589 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[589 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[629 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[668 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[669 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x02;
		break;
	case 3:
		vga_ctrl->VRAM[588 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[588 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[628 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[668 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[669 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x02;
		break;
	case 4:
		vga_ctrl->VRAM[628 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[629 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[668 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[669 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x01;
		break;
	case 5:
		vga_ctrl->VRAM[588 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[588 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[628 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[629 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[669 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x02;
		break;
	case 6:
		vga_ctrl->VRAM[589 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[589 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[628 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[629 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[669 * 2] = fg1 << 4 | bg1;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x01;
		break;
	case 7:
		vga_ctrl->VRAM[589 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[589 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[628 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[629 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[668 * 2] = fg2 << 4 | bg2;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x02;
		break;
	default:
		vga_ctrl->VRAM[548 * 2] = 0x0;
		vga_ctrl->VRAM[548 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[549 * 2] = 0x0;
		vga_ctrl->VRAM[549 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[588 * 2] = 0x0;
		vga_ctrl->VRAM[588 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[589 * 2] = 0x0;
		vga_ctrl->VRAM[589 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[628 * 2] = 0x0;
		vga_ctrl->VRAM[628 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[629 * 2] = 0x0;
		vga_ctrl->VRAM[629 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[668 * 2] = 0x0;
		vga_ctrl->VRAM[668 * 2 + 1] = 0x0;
		vga_ctrl->VRAM[669 * 2] = 0x0;
		vga_ctrl->VRAM[669 * 2 + 1] = 0x0;
		break;
	}
}

void drawStats(int T, int J, int Z, int O, int S, int L, int I){
	for(int i = 0; i < 3; i++){
		int dig = T % 10;
		drawDigit(dig, 210 - i, 4, 0);
		T /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = J % 10;
		drawDigit(dig, 330 - i, 4, 0);
		J /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = Z % 10;
		drawDigit(dig, 450 - i, 4, 0);
		Z /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = O % 10;
		drawDigit(dig, 570 - i, 4, 0);
		O /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = S % 10;
		drawDigit(dig, 690 - i, 4, 0);
		S /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = L % 10;
		drawDigit(dig, 810 - i, 4, 0);
		L /= 10;
	}
	for(int i = 0; i < 3; i++){
		int dig = I % 10;
		drawDigit(dig, 930 - i, 4, 0);
		I /= 10;
	}
}

void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
{
	//fill in this function to set the color palette starting at offset 0x0000 2000 (from base)
	if(color % 2 == 0){
		vga_ctrl->PALETTE[color >> 1] = (red << 9) | (green << 5) | (blue << 1);
	}
	else{
		vga_ctrl->PALETTE[color >> 1] = vga_ctrl->PALETTE[color >> 1] | (red << 21) | (green << 17) | (blue << 13);
	}

}

void startScreen(){
    alt_u8 color1[10] = {1, 2, 5, 1, 12, 10, 4, 1, 9, 4}; //level color palette sets
    alt_u8 color2[10] = {5, 10, 13, 2, 10, 9, 8, 4, 12, 14};
    int count = 0;
    int count2 = 0;
	while(keycode != 40){
		keycode = keyboardDriver(keycode);
		alt_u8 c1 = color1[count % 10];
		alt_u8 c2 = color2[count % 10];
		//T
		for(int i = 0; i < 5; i++){
			vga_ctrl->VRAM[(322 + i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(322 + i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(324 + 40 * i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(324 + 40 * i) * 2 + 1] = 0x02;
		}
		//E
		for(int i = 0; i < 4; i++){
			vga_ctrl->VRAM[(329 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(329 + i) * 2 + 1] = 0x01;
			vga_ctrl->VRAM[(328 + 40 * i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(328 + 40 * i) * 2 + 1] = 0x01;
			vga_ctrl->VRAM[(408 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(408 + i) * 2 + 1] = 0x01;
			vga_ctrl->VRAM[(489 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(489 + i) * 2 + 1] = 0x01;
		}
		vga_ctrl->VRAM[488 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[488 * 2 + 1] = 0x01;
		//T
		for(int i = 0; i < 5; i++){
			vga_ctrl->VRAM[(334 + i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(334 + i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(336 + 40 * i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(336 + 40 * i) * 2 + 1] = 0x02;
		}
		//R
		for(int i = 0; i < 4; i++){
			vga_ctrl->VRAM[(340 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(340 + i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(420 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(420 + i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(380 + 40 * i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(380 + 40 * i) * 2 + 1] = 0x02;
		}
		vga_ctrl->VRAM[384 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[384 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[464 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[464 * 2 + 1] = 0x02;
		vga_ctrl->VRAM[504 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[504 * 2 + 1] = 0x02;
		//I
		for(int i = 0; i < 5; i++){
			vga_ctrl->VRAM[(346 + i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(346 + i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(348 + 40 * i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(348 + 40 * i) * 2 + 1] = 0x02;
			vga_ctrl->VRAM[(506 + i) * 2] = c2 << 4 | 15;
			vga_ctrl->VRAM[(506 + i) * 2 + 1] = 0x02;
		}
		//S
		for(int i = 0; i < 5; i++){
			vga_ctrl->VRAM[(352 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(352 + i) * 2 + 1] = 0x01;
			vga_ctrl->VRAM[(432 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(432 + i) * 2 + 1] = 0x01;
			vga_ctrl->VRAM[(512 + i) * 2] = c1 << 4 | 15;
			vga_ctrl->VRAM[(512 + i) * 2 + 1] = 0x01;
		}
		vga_ctrl->VRAM[392 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[392 * 2 + 1] = 0x01;
		vga_ctrl->VRAM[476 * 2] = c1 << 4 | 15;
		vga_ctrl->VRAM[476 * 2 + 1] = 0x01;
		//P
		vga_ctrl->VRAM[654 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[654 * 2 + 1] = 0x2f;
		//R
		vga_ctrl->VRAM[655 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[655 * 2 + 1] = 0x31;
		//E
		vga_ctrl->VRAM[656 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[656 * 2 + 1] = 0x24;
		//S
		vga_ctrl->VRAM[657 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[657 * 2 + 1] = 0x32;
		//S
		vga_ctrl->VRAM[658 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[658 * 2 + 1] = 0x32;
		//E
		vga_ctrl->VRAM[660 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[660 * 2 + 1] = 0x24;
		//N
		vga_ctrl->VRAM[661 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[661 * 2 + 1] = 0x2d;
		//T
		vga_ctrl->VRAM[662 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[662 * 2 + 1] = 0x33;
		//E
		vga_ctrl->VRAM[663 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[663 * 2 + 1] = 0x24;
		//R
		vga_ctrl->VRAM[664 * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[664 * 2 + 1] = 0x31;
		if(count2 % 200 == 0){
			count++;
		}
		count2++;
	}
}

void clearScreen(){
	for(int i = 0; i < 1200; i++){
		vga_ctrl->VRAM[i * 2] = 0;
		vga_ctrl->VRAM[i * 2 + 1] = 0;
	}
}

void endGame(){
	for(int i = 0; i < 10; i++){ //blink 5 times
		alt_u8 color;
		if(i % 2 == 0){
			color = 4;
		}
		else{
			color = 0;
		}
		//G
		vga_ctrl->VRAM[95 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[95 * 2 + 1] = 0x26;
		//A
		vga_ctrl->VRAM[96 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[96 * 2 + 1] = 0x20;
		//M
		vga_ctrl->VRAM[97 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[97 * 2 + 1] = 0x2c;
		//E
		vga_ctrl->VRAM[98 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[98 * 2 + 1] = 0x24;
		//O
		vga_ctrl->VRAM[100 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[100 * 2 + 1] = 0x2e;
		//V
		vga_ctrl->VRAM[101 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[101 * 2 + 1] = 0x35;
		//E
		vga_ctrl->VRAM[102 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[102 * 2 + 1] = 0x24;
		//R
		vga_ctrl->VRAM[103 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[103 * 2 + 1] = 0x31;
		//!
		vga_ctrl->VRAM[104 * 2] = color << 4 | 0;
		vga_ctrl->VRAM[104 * 2 + 1] = 0x45;
		usleep(100000);
	}
}

int chooseLevel(){
	//draw level & words
	//draw top left corner
	vga_ctrl->VRAM[786 * 2] = 11 << 4 | 0;
	vga_ctrl->VRAM[786 * 2 + 1] = 0x0b;
	//draw top wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(787 + i) * 2] = 11 << 4 | 0;
		vga_ctrl->VRAM[(787 + i) * 2 + 1] = 0x0c;
	}
	//draw top right corner
	vga_ctrl->VRAM[794 * 2] = 11 << 4 | 0;
	vga_ctrl->VRAM[794 * 2 + 1] = 0x0d;
	//draw right wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(834 + i * 40) * 2] = 11 << 4 | 0;
		vga_ctrl->VRAM[(834 + i * 40) * 2 + 1] = 0x0e;
	}
	//draw bottom right corner
	vga_ctrl->VRAM[994 * 2] = 11 << 4 | 0;
	vga_ctrl->VRAM[994 * 2 + 1] = 0x0f;
	//draw bottom wall
	for(int i = 0; i < 7; i++){
		vga_ctrl->VRAM[(987 + i) * 2] = 11 << 4 | 0;
		vga_ctrl->VRAM[(987 + i) * 2 + 1] = 0x10;
	}
	//draw bottom left corner
	vga_ctrl->VRAM[986 * 2] = 11 << 4 | 0;
	vga_ctrl->VRAM[986 * 2 + 1] = 0x11;
	//draw left wall
	for(int i = 0; i < 4; i++){
		vga_ctrl->VRAM[(826 + i * 40) * 2] = 11 << 4 | 0;
		vga_ctrl->VRAM[(826 + i * 40) * 2 + 1] = 0x12;
	}
	//L
	vga_ctrl->VRAM[868 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[868 * 2 + 1] = 0x2b;
	//E
	vga_ctrl->VRAM[869 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[869 * 2 + 1] = 0x24;
	//V
	vga_ctrl->VRAM[870 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[870 * 2 + 1] = 0x35;
	//E
	vga_ctrl->VRAM[871 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[871 * 2 + 1] = 0x24;
	//L
	vga_ctrl->VRAM[872 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[872 * 2 + 1] = 0x2b;
	int startLevel = 0;
	drawLevel(0);
	usleep(100000);
	keycode = keyboardDriver(keycode);
	while(keycode != 40){
		keycode = keyboardDriver(keycode);
		if(keycode == 82){
			if(startLevel < 19){
				startLevel++;
				drawLevel(startLevel);
				usleep(25000);
			}
		}
		if(keycode == 81){
			if(startLevel > 0){
				startLevel--;
				drawLevel(startLevel);
				usleep(25000);
			}
		}
	}
	return startLevel;
}

void drawHighScore(){
	//H
	vga_ctrl->VRAM[1049 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1049 * 2 + 1] = 0x27;
	//I
	vga_ctrl->VRAM[1050 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1050 * 2 + 1] = 0x28;
	//G
	vga_ctrl->VRAM[1051 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1051 * 2 + 1] = 0x26;
	//H
	vga_ctrl->VRAM[1052 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1052 * 2 + 1] = 0x27;
	//S
	vga_ctrl->VRAM[1053 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1053 * 2 + 1] = 0x32;
	//C
	vga_ctrl->VRAM[1054 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1054 * 2 + 1] = 0x22;
	//O
	vga_ctrl->VRAM[1055 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1055 * 2 + 1] = 0x2e;
	//R
	vga_ctrl->VRAM[1056 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1056 * 2 + 1] = 0x31;
	//E
	vga_ctrl->VRAM[1057 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1057 * 2 + 1] = 0x24;
	//:
	vga_ctrl->VRAM[1058 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1058 * 2 + 1] = 0x44;
	//name
	for(int i = 0; i < 3; i++){
		vga_ctrl->VRAM[(1060 + i) * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[(1060 + i) * 2 + 1] = scorer[i];
	}
	//number
	int temp = highscore;
	for(int i = 0; i < 7; i++){
		int dig = temp % 10;
		drawDigit(dig, 1070 - i, 15, 0);
		temp /= 10;
	}
}

void drawPause(){
	//P
	vga_ctrl->VRAM[657 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[657 * 2 + 1] = 0x2f;
	//A
	vga_ctrl->VRAM[658 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[658 * 2 + 1] = 0x20;
	//U
	vga_ctrl->VRAM[659 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[659 * 2 + 1] = 0x34;
	//S
	vga_ctrl->VRAM[660 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[660 * 2 + 1] = 0x32;
	//E
	vga_ctrl->VRAM[661 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[661 * 2 + 1] = 0x24;
	//D
	vga_ctrl->VRAM[662 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[662 * 2 + 1] = 0x23;
}

void unpause(){
	for(int i = 0; i < 6; i++){
		vga_ctrl->VRAM[(657 + i) * 2] = 0 << 4 | 0;
		vga_ctrl->VRAM[(657 + i) * 2 + 1] = 0x0;
	}
}

void enterHighScore(){
	//clear name
	for(int i = 0; i < 3; i++){
		scorer[i] = 0x20;
	}
	drawHighScore();
	//N
	vga_ctrl->VRAM[1045 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1045 * 2 + 1] = 0x2d;
	//E
	vga_ctrl->VRAM[1046 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1046 * 2 + 1] = 0x24;
	//W
	vga_ctrl->VRAM[1047 * 2] = 15 << 4 | 0;
	vga_ctrl->VRAM[1047 * 2 + 1] = 0x36;

	//arrows
	for(int i = 0; i < 3; i++){
		vga_ctrl->VRAM[(1020 + i) * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[(1020 + i) * 2 + 1] = 0x47;
		vga_ctrl->VRAM[(1100 + i) * 2] = 15 << 4 | 0;
		vga_ctrl->VRAM[(1100 + i) * 2 + 1] = 0x48;
	}
	int current = 0;
	while(keycode != 40){
		keycode = keyboardDriver(keycode);
		vga_ctrl->VRAM[(1100 + current) * 2 + 1] = 0x49; //draw current editing line
		if(keycode == 81 && scorer[current] < 0x46){ //handle scroll forward
			scorer[current] += 1;
		}
		else if(keycode == 82 && scorer[current] >= 0x20){ //handle scroll backward
			scorer[current] -= 1;
		}
		else if(keycode == 80 && current > 0){ //handle move left
			vga_ctrl->VRAM[(1100 + current) * 2 + 1] = 0x48;
			current -= 1;
		}
		else if(keycode == 79 && current < 2){ //handle move right
			vga_ctrl->VRAM[(1100 + current) * 2 + 1] = 0x48;
			current += 1;
		}
		drawHighScore(); //redraw score name
		usleep(20000);
	}
	usleep(20000);
	keycode = 0; //clear keycode
}
