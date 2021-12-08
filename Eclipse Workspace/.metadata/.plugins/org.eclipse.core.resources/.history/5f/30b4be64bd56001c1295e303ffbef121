#include "tetris.h"
#include "text_mode_vga.h"
#include "usb_kb.c"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"


int main() {
	printf("initializing MAX3421E...\n");
	MAX3421E_init();
	printf("initializing USB...\n");
	USB_init();
	Mix_Music *music;
	music = Mix_LoadMUS("music.mp3");
	if(music) {
		Mix_PlayMusic(music, -1);
	}
	Test();
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	Mix_Quit();
	return 0;
}
