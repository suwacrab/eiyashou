#include "bios.h"

/*	--	 main functions	--	*/
void bios_init(bios *kernel,u32 w,u32 h)
{
	/*	--	sdl init	--	*/
	// main
	s32 sdlflags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	if( SDL_Init(sdlflags) < 0 )
	{ printf("couldn't init sdl: '%s'\n",SDL_GetError()); exit(-1); }
	// video
	s32 vidflags = SDL_SWSURFACE;
	kernel->window = SDL_SetVideoMode(w,h,16,vidflags);
	kernel->w = w;
	kernel->h = h;
	// event
	kernel->quit = false;
	kernel->time = 0;
	kernel->keystate = SDL_GetKeyState(NULL);
}

/*	--	update funcs	--	*/
void bios_boot(bios *kernel)
{
	while( !kernel->quit )
	{
		// update & draw
		bios_update(kernel);
		bios_draw(kernel);
		// vsync
		SDL_Flip(kernel->window);
		SDL_Delay( SDL_SECOND/60 );
		kernel->time++;
	}
}

void bios_update(bios *kernel)
{
	// update input
	SDL_PumpEvents();
	bios_checkquit(kernel);
}

void bios_checkquit(bios *kernel)
{
	kernel->quit = kernel->keystate[SDLK_ESCAPE];
}

eirin *testimg = NULL;
/*	--	draw funcs	--	*/
void bios_draw(bios *kernel)
{
	// clearing
	bios_clearscreen(kernel);
	// drawing some rectangle lmao
	if(testimg == NULL)
	{ 
		testimg = (eirin*)malloc(sizeof(testimg));
		eirin_loadimg(testimg,"gfx/testtex.png",EIRIN_PIXELFMT_RGB565);
	}

	// image dimensions: [$10,$10]
	// scale dimensions: [$40,$40]
	FIXED offset = (FIXED)( lu_sin(kernel->time<<10) );
	FIXED s = inttofixed(8,8) + (offset>>1);
	FIXED xs = inttofixed(4,8);
	FIXED ys = kernel->time<<3;
	xs = ys = s;
	FIXED w = fix_mul2(testimg->w,xs,8);
	FIXED h = fix_mul2(testimg->h,ys,8);
	printf("[%04X,%04X]\n",w,h);

	for(u32 ly=0; ly<h; ly++)
	{
		for(u32 lx=0; lx<w; lx++)
		{
			// scenario: scale is [4,4] and size is [$20,$10]
			// now the size is [$80,40] after scaling
			// also, the pos is [$30,20]
			// x would be $40/4, or $10
			u32 ox = (kernel->w>>1) + lx - (w>>1);
			u32 oy = (kernel->h>>1) + ly - (h>>1);
			SDL_Rect drect = { ox,oy,1,1 };
			u32 x = inttofixed(lx,8) / xs;
			u32 y = inttofixed(ly,8) / ys;
			SDL_FillRect(kernel->window,&drect,
				((RGB16*)testimg->m)[x + (y*testimg->w)]
			);
		}
	}

}

void bios_clearscreen(bios *kernel)
{
	SDL_Surface *window = kernel->window;
	// clear window
	SDL_Rect windowrect = { 0,0,kernel->w,kernel->h };
	SDL_FillRect(window,&windowrect,
		SDL_MapRGB(window->format,0,0,0)
	);
}
