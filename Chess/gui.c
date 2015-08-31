# include "gui.h"


int haimstart(void) {
	SDL_Surface* hello = NULL;
	SDL_Surface* screen = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(640, 480, 24, SDL_SWSURFACE);
	hello = SDL_LoadBMP("Untitled.bmp");
	SDL_BlitSurface(hello, NULL, screen, NULL);

	//Update Screen
	SDL_Flip(screen);

	//Pause
	SDL_Delay(5000);

	SDL_FreeSurface(hello);

	//Quit SDL
	SDL_Quit();

	return 0;
}



