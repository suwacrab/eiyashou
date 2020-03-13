#include "eirin.h"

/*	--	init funcs -- */
size_t eirin_initPAL4(eirin *yago) { return sizeof(u8) * (yago->w>>1) * yago->h; }
size_t eirin_initPAL8(eirin *yago) { return sizeof(u8) * yago->w * yago->h; }
size_t eirin_initRGB565(eirin *yago) { return sizeof(u8) * (yago->w<<1) * yago->h; }

eirin_initfunc eirin_initfmts[] = {
	&eirin_initPAL4,&eirin_initPAL8,
	&eirin_initRGB565
};

/*	--	main funcs	--	*/
eirin *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt)
{
	// initializin
	yago->w = w;
	yago->h = h;
	yago->fmt = fmt;
	yago->palette = NULL;
	yago->m = NULL;
	
	// allocatin memory for pixels
	yago->m = calloc( 1,eirin_initfmts[fmt](yago) );
	return yago;
}
eirin *eirin_loadimg(eirin *yago,const char *fname,eirin_pixelfmt fmt)
{
	// vars n shit
	printf("loading image '%s'...   ",fname);
	SDL_Surface *loadimg = NULL; // loaded img
	SDL_Surface *convimg = NULL; // converted img
	// loadin
	loadimg = IMG_Load(fname);
	if(loadimg == NULL)
	{ printf("loading failed: %s\n",IMG_GetError()); exit(-1); }	
	else {
		if(fmt == EIRIN_PIXELFMT_RGB565)
		{
			SDL_PixelFormat sdlfmt = { 
				NULL,16,2, // pal,bpp,bytes
				3,2,3,0, // loss (rgba)
				11,5,0,0, // shift (rgba)
				31<<11,63<<5,31,0, // masks (rgba)
				0,0xFF // colorkey,alpha
			};
			convimg = SDL_ConvertSurface(loadimg,&sdlfmt,SDL_SWSURFACE);
			SDL_LockSurface(convimg);
			u32 w = convimg->w;
			u32 h = convimg->h;
			// ???
			eirin_init(yago,w,h,fmt);
			u16 *convpix = (u16*)convimg->pixels;
			u16 *yagopix = (u16*)yago->m;
			for(u32 i=0; i<(w*h); i++)
			{
				u8 r,g,b;
				SDL_GetRGB(convpix[i],&sdlfmt,&r,&g,&b);
				yagopix[i] = RGB565(r>>3,g>>2,b>>3);
			}
			SDL_FreeSurface(loadimg);
			SDL_FreeSurface(convimg);
		}
	}
	
	printf("image loaded! [%p]\n",yago);
	// return the loaded img
	return yago;
}
eirin *eirin_free(eirin *yago)
{
	if(yago->m != NULL) { free(yago->m); yago->m = NULL; }
	if(yago->palette != NULL) { free(yago->palette); yago->palette = NULL; }
}

/*	--	pixel funcs	--	*/


/*	--	fill funcs	--	*/
void eirin_trifillbot(eirin *yago,VEC2 *v,u32 c)
{
	FIXED inv1 = inttofixed(v[1].x - v[0].x,8) / (v[1].y - v[0].y);
	FIXED inv2 = inttofixed(v[2].x - v[0].x,8) / (v[2].y - v[0].y);
	
	printf("%04d,%04d (%04X,%04X)\n",inv1,inv2,inv1,inv2);
	FIXED curx1,curx2;
	curx1 = curx2 = v[0].x<<8;

	for(FIXED scany = v[0].y; scany < v[1].y; scany++)
	{
		// draw line
		for(s32 x = curx1; x<curx2; x += (1<<8))
		{ eirin_pset16(yago,x>>8,(u32)scany,c); }
		curx1 += inv1;
		curx2 += inv2;
	};
}

