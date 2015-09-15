#include "guiBoard.h"

void haim_main(){
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Surface *surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);


	if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}
	load_all_pices();
	createPlayPage(surface);
	updateSurface(surface);
}


int createPlayPage(SDL_Surface* surface){
	SDL_Surface *bkg = loadImage("./images/settings/bkg/mainMenu_bkg.bmp");
	addImageToSurface(bkg, NULL, surface, NULL);
	createBoard(surface);
}

int createBoard(SDL_Surface* surface){
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	int x;
	for (x = 0; x < BOARD_SIZE; x++){
		int y;
		for (y = 0; y < BOARD_SIZE; y++){
			rDest.x = x * 75;
			rDest.y = y * 75;
			addImageToSurface(getPiceImage(x,y), &rOrigin, surface, &rDest);
		}
	}
}

int load_all_pices(){
	int pice, color, bkg;
	for (pice = 0; pice < 6; pice++){
		for (color = 0; color < 2; color++){
			for (bkg = 0; bkg < 2; bkg++){
				char path[500];
				sprintf(path, "./images/board/%c_%c_%c.bmp",pice_types[pice], colors[color], colors[bkg]);
				picess[pice][color][bkg] = loadImage(path);
				if (picess[pice][color][bkg] == NULL){
					//free_all_picess();
					return 1;
				}
			}
		}
	}

	for (bkg = 0; bkg < 2; bkg++){
		char path[500];
		sprintf(path, "./images/board/blank_%c.bmp", colors[bkg]);
		emptys[bkg] = loadImage(path);
		if (emptys[bkg] == NULL){
			//free_all_picess();
			return 1;
		}
	}
	return 0;
}

SDL_Surface* getPiceImage(int x, int y){ //x,y are GUI base
	int bkg = x % 2 == y % 2 ? GUI_white : GUI_black;
	char pice = getPice(createLocationNode( x, BOARD_SIZE-1-y));
	if (pice == EMPTY){
		return emptys[bkg];
	}
	else{
		int pice_color = isupper(pice) ? GUI_black : GUI_white;
		pice = tolower(pice);
		int pice_type;

		switch (pice)
		{
		case 'b':
			pice_type = GUI_b;
			break;
		case 'k':
			pice_type = GUI_k;
			break;
		case 'm':
			pice_type = GUI_m;
			break;
		case 'n':
			pice_type = GUI_n;
			break;
		case 'q':
			pice_type = GUI_q;
			break;
		case 'r':
			pice_type = GUI_r;
			break;
		}

	return picess[pice_type][pice_color][bkg];
	}
}

