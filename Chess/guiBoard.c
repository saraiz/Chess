#include "guiBoard.h"

void haim_main(){
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);


	if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}
	load_all_pices();
	int isQuit = 0;
	if (settings.gameMode == TWO_PLAYERS){
		while (!isQuit){
			createPlayPage();
			updateSurface(surface);
			handleBoardEvents(); //chose a pice

			//TODO update game_board.turn
		}
	}
	else{
		//TODO p vs comp
	}

}

int createPlayPage( ){
	SDL_Surface *bkg = loadImage("./images/settings/bkg/mainMenu_bkg.bmp");
	addImageToSurface(bkg, NULL, surface, NULL);
	createBoard(surface);
}

int createBoard(){
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	int x;
	for (x = 0; x < BOARD_SIZE; x++){
		int y;
		for (y = 0; y < BOARD_SIZE; y++){
			rDest.x = x * 75;
			rDest.y = y * 75;
			addImageToSurface(getPiceImage(x,y,0), &rOrigin, surface, &rDest);
		}
	}
}

int load_all_pices(){
	int pice, color, bkg, isColored;
	for (pice = 0; pice < 6; pice++){
		for (color = 0; color < 2; color++){
			for (bkg = 0; bkg < 2; bkg++){
				for (isColored = 0; isColored < 2; isColored++){
					char path[500];
					sprintf(path, "./images/board/%c_%c_%c_%d.bmp", pice_types[pice], colors[color], colors[bkg],isColored);
					picess[pice][color][bkg][isColored] = loadImage(path);
					if (picess[pice][color][bkg] == NULL){
						//free_all_picess();
						return 1;
					}
				}
			}
		}
	}

	for (bkg = 0; bkg < 2; bkg++){
		for (isColored = 0; isColored < 2; isColored++){
			char path[500];
			sprintf(path, "./images/board/blank_%c_%d.bmp", colors[bkg], isColored);
			emptys[bkg][isColored] = loadImage(path);
			if (emptys[bkg] == NULL){
				//free_all_picess();
				return 1;
			}
		}
	}
	return 0;
}

SDL_Surface* getPiceImage(int x, int y, int isColored){ //x,y are GUI base
	int bkg = x % 2 == y % 2 ? GUI_white : GUI_black;
	char pice = getPice(createLocationNode( x, BOARD_SIZE-1-y));
	if (pice == EMPTY){
		return emptys[bkg][isColored];
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

		return picess[pice_type][pice_color][bkg][isColored];
	}
}

int handleBoardEvents(){
	SDL_Event e;
	int quit = 0;
	while (!quit){
		while (SDL_PollEvent(&e) != 0){
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				//if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				quit = handleBoardButtonClicked(e);
				break;
			default:
				break;
			}
		}
	}
}

int handleBoardButtonClicked(SDL_Event e){
	int quit = 0;
	if (e.button.x > 75*BOARD_SIZE){
		//TODO -butten
	}
	else {
		locationNode location = whichSquerWasClicked(e);
		if (isSameColorAsMe(location, game_board.isBlackTurn)){
			moveList* moves =  getValidMovesForLocation(location, 0);
			if (moves == NULL){
				//freeAllImages()
				//return ERROR;
			}
			colorSquers( moves, location);
			//TODO is error?
			freeAllMoveList(moves);

		}
	}

	return quit;

}

int colorSquere(locationNode loc){
	//x,y are gui base
	int x = loc.column * 75;
	int y = (BOARD_SIZE-1- loc.row) * 75;
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	rDest.x = x;
	rDest.y = y;
	SDL_Surface* img = getPiceImage(loc.column, BOARD_SIZE - 1 - loc.row, 1);
	addImageToSurface(img, &rOrigin, surface, &rDest);
}

int colorSquers(moveList* move,locationNode origin){
		if (!isEmptyMoveList(move)){
			moveList* cur = move;
			for (; cur != NULL; cur = cur->next){
				colorSquere(cur->destination);
			}
		}
		colorSquere(origin);
		updateSurface(surface);
	}
