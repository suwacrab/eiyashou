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

	/*	--	mokokene init	--	*/
	kernel->fb = malloc(sizeof(keine));
	keine *fb = kernel->fb;
	keine_init(fb,w,h,KEINE_PIXELFMT_RGB565);
	mokou_pset16(fb,32,32,0x1F);
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
		bios_blitkene(kernel);
		bios_flip(kernel);
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

keine *testimg = NULL;
/*	--	draw funcs	--	*/
void bios_draw(bios *kernel)
{
	// clearing
	keine_clear(kernel->fb);
	bios_clearscreen(kernel);
	// drawing some rectangle lmao
	if(testimg == NULL)
	{
		testimg = (keine*)malloc(sizeof(keine));
		keine_loadimg(testimg,"gfx/testtex.png",KEINE_PIXELFMT_RGB565);
	}
	
	{
		SDL_Rect src = { 0,0,16,16 };
		mokou_sprattr attr = { mokou_sprpos(0,0),0b11 };
		mokou_spr16(testimg,kernel->fb,src,attr);

		src.x += 0x10; src.y += 0x10;
		mokou_spr16(testimg,kernel->fb,src,attr);
	}

	// image dimensions: [$10,$10]
	// scale dimensions: [$40,$40]
	FIXED offset = fix_mul(1<<12,lu_sin(kernel->time<<10),12);
	FIXED s = int2fx(8,12) + offset;
	FIXED xs = int2fx(4,12);
	FIXED ys = int2fx(4,12);
	xs = ys = s;
	FIXED w = fix_mul2(testimg->w,xs,12);
	FIXED h = fix_mul2(testimg->h,ys,12);

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
			u32 x = int2fx(lx,12) / xs;
			u32 y = int2fx(ly,12) / ys;
			SDL_FillRect(kernel->window,&drect,
				mokou_pget16(testimg,x,y)
			);
		}
	}

}

void bios_blitkene(bios *kernel)
{
	keine *curfb = kernel->fb;
	SDL_Rect drect = { 0,0,1,1 };
	for(u32 y=0; y<curfb->h; y++)
	{
		drect.y = y;
		for(u32 x=0; x<curfb->w; x++)
		{
			drect.x = x;
			SDL_FillRect(kernel->window,&drect,
				mokou_pget16(curfb,x,y)
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

void bios_flip(bios *kernel)
{
	SDL_Flip(kernel->window);
	SDL_Delay( SDL_SECOND/60 );
	kernel->time++;
}

