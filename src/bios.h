#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "sinlut.h"
#include "keine.h"
#include "mokou.h"
#include "kbase.h"
#include "matrix.h"

#ifndef BIOS_H
#define BIOS_H

/*	--	structs	--	*/
typedef struct bios {
	// screen vars
	SDL_Surface *window;
	keine *fb; // keine framebuffer
	uint32_t w,h; // width & height
	// event vars
	SDL_Event inpevent; // event for input
	uint8_t *keystate; // current key state
	uint32_t time; // time in frames
	bool quit; // quit flag
} bios;

/*	--	main funcs	--	*/
extern void bios_init(bios *kernel,u32 w,u32 h);

/*	--	update funcs	--	*/
extern void bios_boot(bios *kernel);
extern void bios_update(bios *kernel);
extern void bios_checkquit(bios *kernel);

/*	--	draw functions --	*/
extern void bios_draw(bios *kernel);
extern void bios_flip(bios *kernel);
extern void bios_blitkene(bios *kernel);
extern void bios_clearscreen(bios *kernel);

#endif
