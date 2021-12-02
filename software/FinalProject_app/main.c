#include "tetris.h"
#include "text_mode_vga.h"
#include "usb_kb.c"

int main() {
	printf("initializing MAX3421E...\n");
	MAX3421E_init();
	printf("initializing USB...\n");
	USB_init();
	Test();
	return 0;
}
