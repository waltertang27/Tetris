/*
 *  text_mode_vga.h
 *	Minimal driver for text mode VGA support, ECE 385 Summer 2021 Lab 6
 *  You may wish to extend this driver for your final project/extra credit project
 *
 *  Created on: Jul 17, 2021
 *      Author: zuofu
 */

#ifndef TEXT_MODE_VGA_COLOR_H_
#define TEXT_MODE_VGA_COLOR_H_

#define COLUMNS 40
#define ROWS 30

#include <system.h>
#include <alt_types.h>

struct TEXT_VGA_STRUCT {
	alt_u8 VRAM [ROWS*COLUMNS*2]; //Week 2 - extended VRAM
	//modify this by adding const bytes to skip to palette, or manually compute palette
	const alt_u8 UNUSED [5792];
	alt_u32 PALETTE [8];
};

struct COLOR{
	char name [20];
	alt_u8 red;
	alt_u8 green;
	alt_u8 blue;
};


//you may have to change this line depending on your platform designer
static volatile struct TEXT_VGA_STRUCT* vga_ctrl = VGA_TEXT_MODE_CONTROLLER_BASE;

//CGA colors with names
static struct COLOR colors[]={
    {"black",          0x0, 0x0, 0x0}, //0
	{"blue",           0x0, 0x0, 0xa}, //1
    {"green",          0x0, 0xa, 0x0}, //2
	{"cyan",           0x0, 0xa, 0xa}, //3
    {"red",            0xa, 0x0, 0x0}, //4
	{"magenta",        0xa, 0x0, 0xa}, //5
    {"brown",          0xa, 0x5, 0x0}, //6
	{"light gray",     0xa, 0xa, 0xa}, //7
    {"dark gray",      0x5, 0x5, 0x5}, //8
	{"light blue",     0x5, 0x5, 0xf}, //9
    {"light green",    0x5, 0xf, 0x5}, //10
	{"light cyan",     0x5, 0xf, 0xf}, //11
    {"light red",      0xf, 0x5, 0x5}, //12
	{"light magenta",  0xf, 0x5, 0xf}, //13
    {"yellow",         0xf, 0xf, 0x5}, //14
	{"white",          0xf, 0xf, 0xf} //15
};

void drawGrid(alt_u8 foreground, alt_u8 background, alt_u8 text);
void drawGame(int gameboard[20][10], alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2);
void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue); //Fill in this code
void drawLevel(int level); //draws the level number to the screen
void drawScore(int score); //draws score to the screen
void drawDigit(int digit, int location, alt_u8 text, alt_u8 background); //draws a single digit to the screen
void drawLines(int lines); //draws line count to screen
void drawStatBlocks(alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2); //draws blocks for block counter to screen
void drawNext(int block, alt_u8 fg1, alt_u8 bg1, alt_u8 fg2, alt_u8 bg2); //draw next block to screen
void drawStats(int T, int J, int Z, int O, int S, int L, int I); //draw block drop stats
void startScreen(); //runs title screen
void clearScreen(); //set screen to black
void endGame(); //draw game over text
int chooseLevel(); //level select box
void drawHighScore(); //draw high score & text to screen
void drawPause(); //draw paused text
void unpause(); //clear paused text
void enterHighScore(); //screen to enter high score name

#endif /* TEXT_MODE_VGA_COLOR_H_ */
