#include "guiUtils.h"


int addImageToSurface(SDL_Surface *img, SDL_Rect *rOrigin, SDL_Surface *dest, SDL_Rect *rDest){
	int isSuccess = 1;
	if (SDL_BlitSurface(img, rOrigin, dest, rDest) != 0) {
		isSuccess = 0;
		my_sdl_free(img);
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

Button createButton(char *imgUrl, int id, int width, int height, int x, int y){
	Button newGameBtn; //= { { 0, 0, 0, 0 }, NULL, 0 };
	newGameBtn.selectedImg = NULL;
	SDL_Rect emptyRect = { 0, 0, 0, 0 };
	newGameBtn.buttonsDestRect = newGameBtn.buttonsOriginRect = emptyRect;

	newGameBtn.id = id;
	SDL_Rect newGameBtnDestRect;
	newGameBtnDestRect.x = x;
	newGameBtnDestRect.y = y;
	newGameBtnDestRect.w = width;
	newGameBtnDestRect.h = height;

	SDL_Rect newGameBtnOriginRect = { 0, 0, 0, 0 };
	newGameBtnOriginRect.w = width;
	newGameBtnOriginRect.h = height;

	newGameBtn.img = loadImage(imgUrl);
	if (newGameBtn.img == NULL){
		return newGameBtn;
	}

	newGameBtn.buttonsDestRect = newGameBtnDestRect;
	newGameBtn.buttonsOriginRect = newGameBtnOriginRect;

	return newGameBtn;
}

Button createButton_settings(char *imgUrl, char *selectedImgUrl, int id, int width, int height, int x, int y){
	Button newGameBtn;
	SDL_Rect emptyRect = { 0, 0, 0, 0 };
	newGameBtn.buttonsDestRect = newGameBtn.buttonsOriginRect = emptyRect;

	newGameBtn.id = id;
	SDL_Rect newGameBtnDestRect;
	newGameBtnDestRect.x = x;
	newGameBtnDestRect.y = y;
	newGameBtnDestRect.w = width;
	newGameBtnDestRect.h = height;

	SDL_Rect newGameBtnOriginRect = { 0, 0, 0, 0 };
	newGameBtnOriginRect.w = width;
	newGameBtnOriginRect.h = height;

	newGameBtn.img = loadImage(imgUrl);
	if (newGameBtn.img == NULL){
		return newGameBtn;
	}

	if (selectedImgUrl != NULL){
		newGameBtn.selectedImg = loadImage(selectedImgUrl);
		if (newGameBtn.img == NULL){
			return newGameBtn;
		}
	}
	else{
		newGameBtn.selectedImg = NULL;
	}

	newGameBtn.buttonsDestRect = newGameBtnDestRect;
	newGameBtn.buttonsOriginRect = newGameBtnOriginRect;

	return newGameBtn;
}

int addButtons(Button *list, int len, SDL_Surface *surface){
	int isSuceess = 1;
	for (int i = 0; i < len; i++){
		isSuceess = addImageToSurface(list[i].img, &list[i].buttonsOriginRect, surface, &list[i].buttonsDestRect);
		if (!isSuceess){
			break;
		}
	}

	if (isSuceess){
		isSuceess = updateSurface(surface);
	}

	return isSuceess;
}

locationNode whichSquerWasClicked(SDL_Event e){
	//x,y are gui base
	int x = (int) e.button.x / 75;
	int y = (int)e.button.y / 75;
	return createLocationNode(x, BOARD_SIZE - 1 - y);

}

void my_sdl_free(SDL_Surface* toFree){
	if (toFree != NULL){
		SDL_FreeSurface(toFree);

	}
}