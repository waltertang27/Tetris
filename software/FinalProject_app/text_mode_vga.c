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

void textVGAColorClr()
{
	for (int i = 0; i<(ROWS*COLUMNS) * 2; i++)
	{
		vga_ctrl->VRAM[i] = 0x0;
	}
}

void textVGADrawColorText(char* str, int x, int y, alt_u8 background, alt_u8 foreground)
{
	alt_u8 st = 2;
	int i = 0;
	while (str[i]!=0)
	{
		vga_ctrl->VRAM[(y*COLUMNS + x + i) * 2] = foreground << 4 | background;
		vga_ctrl->VRAM[(y*COLUMNS + x + i) * 2 + 1] = st;
		i++;
	}
}

void Test(){
	printf("test running");
	textVGAColorClr();
	for (int i = 0; i < 16; i++)
	{
		setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);

	}
	drawGrid(11, 0);
	//draw a block of each type
	vga_ctrl->VRAM[538 * 2] = 0xf4;
	vga_ctrl->VRAM[538 * 2 + 1] = 0x01;
	vga_ctrl->VRAM[541 * 2] = 0xf4;
	vga_ctrl->VRAM[541 * 2 + 1] = 0x02;
}

void drawGrid(alt_u8 background, alt_u8 foreground){
	//draw top left corner
	vga_ctrl->VRAM[174 * 2] = foreground << 4 | background;
	vga_ctrl->VRAM[174 * 2 + 1] = 0x3;
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


void textVGAColorScreenSaver()
{
	//This is the function you call for your week 2 demo
	char color_string[80];
    int fg, bg, x, y;
	textVGAColorClr();
	//initialize palette
	for (int i = 0; i < 16; i++)
	{
		setColorPalette (i, colors[i].red, colors[i].green, colors[i].blue);

	}
	for (int i = 0; i < 8; i++){
		printf("REG %i : %08x\n", i, (unsigned int)vga_ctrl->PALETTE[i]);
	}
	while (1)
	{
		fg = rand() % 16;
		bg = rand() % 16;
		while (fg == bg)
		{
			fg = rand() % 16;
			bg = rand() % 16;
		}
		sprintf(color_string, "abcdefghijklmnopqrstuvwxyz");
		x = rand() % (40-strlen(color_string));
		y = rand() % 30;
		textVGADrawColorText (color_string, x, y, bg, fg);
		usleep (100000);
	}
}
