#ifndef GUI_UTILS
#define GUI_UTILS

#include <SDL.h>
#include <SDL_video.h>
#include "utils.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct Button{
	SDL_Rect buttonsDestRect;
	SDL_Rect buttonsOriginRect;
	SDL_Surface *img;
	int id;
}Button;

int addImageToSurface(SDL_Surface *img, SDL_Rect rOrigin, SDL_Surface *dest, SDL_Rect rDest);
SDL_Surface* loadImage(char* url);
SDL_Surface* createSurface(int width, int height);
int updateSurface(SDL_Surface *surface);

#endif
