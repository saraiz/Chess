#include "guiUtils.h"

int addImageToSurface(SDL_Surface *img, SDL_Rect *rOrigin, SDL_Surface *dest, SDL_Rect *rDest){
	int isSuccess = 1;
	if (SDL_BlitSurface(img, rOrigin, dest, rDest) != 0) {
		isSuccess = 0;
		SDL_FreeSurface(img);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
	}

	return isSuccess;
}

SDL_Surface* loadImage(char* url){
	SDL_Surface *img = SDL_LoadBMP(url);
	if (img == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
	}
	return img;
}

SDL_Surface* createSurface(int width, int height){
	SDL_Surface *surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (surface == NULL){
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
	}
	return surface;
}

int updateSurface(SDL_Surface *surface){
	int isSuccess = 1;
	if (SDL_Flip(surface) != 0) {
		isSuccess = 0;
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}

	return isSuccess;
}

int isClickInRect(SDL_Event e, SDL_Rect buttonRect){
	if ((e.button.x > buttonRect.x) &&
		(e.button.x < buttonRect.x + buttonRect.w) &&
		(e.button.y > buttonRect.y) &&
		(e.button.y < buttonRect.y + buttonRect.h)){
		return 1;
	}

	return 0;
}
