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
	SDL_Surface *selectedImg;
	int id;
}Button;

int addImageToSurface(SDL_Surface *img, SDL_Rect *rOrigin, SDL_Surface *dest, SDL_Rect *rDest);
SDL_Surface* loadImage(char* url);
SDL_Surface* createSurface(int width, int height);
int updateSurface(SDL_Surface *surface);
int isClickInRect(SDL_Event e, SDL_Rect buttonRect);
Button createButton(char *imgUrl, int id, int width, int height, int x, int y);
int addButtons(Button list[], int len, SDL_Surface *surface);
locationNode whichSquerWasClicked(SDL_Event e);
void my_sdl_free(SDL_Surface* toFree);

#endif
