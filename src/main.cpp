#include "bios.h"
#include "kbase.h"

#define WIDTH (256)
#define HEIGHT (256)

// sdl vars
bios *eys_bios = NULL;

void init_eys();

int main(void)
{
	// init
	init_eys();
	// main
	bios_boot(eys_bios);
	// exit
	return 0;
}

void init_eys()
{
	eys_bios = new bios;
	bios_init(eys_bios,WIDTH,HEIGHT);
}
