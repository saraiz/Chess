#include "guiBoard.h"


char colors[] = { 'b', 'w' };
char pice_types[] = { 'b', 'k', 'm', 'n', 'q', 'r' };


void GuiBoardStart(){
	
	//settings.gameMode = 0; //TODO delete
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	GuiBData.surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);


	if (SDL_FillRect(GuiBData.surface, &rect, SDL_MapRGB(GuiBData.surface->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}

	load_all_pices();
	//int quit = 0;
	while (GuiBData.pageID != -1){
		switch (GuiBData.pageID)
		{
		case 0:
			pageID0();
			break;
		case 1:
			pageID1();
			break;
		case 2:
			pageID2();
			break;


		case -1:
			//quit = pageIDMinus1();
			break;
		}
	
	}

	free_all_pices();
}

int createBoard(){ //0 if fail, 1 if ok
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	int x;
	for (x = 0; x < BOARD_SIZE; x++){
		int y;
		for (y = 0; y < BOARD_SIZE; y++){
			rDest.x = x * 75;
			rDest.y = y * 75;
			if (!addImageToSurface(getPiceImage(x, y, 0), &rOrigin, GuiBData.surface, &rDest)){
				return 0;
			}
		}
		
	}
	return 0;
}

int load_all_pices(){
	//ret: -1 error, else 0
	int pice, color, bkg, isColored;
	for (pice = 0; pice < 6; pice++){
		for (color = 0; color < 2; color++){
			for (bkg = 0; bkg < 2; bkg++){
				for (isColored = 0; isColored < 2; isColored++){
					char path[500];
					sprintf(path, "./images/board/%c_%c_%c_%d.bmp", pice_types[pice], colors[color], colors[bkg],isColored);
					GuiBData.picess[pice][color][bkg][isColored] = loadImage(path);
					if (GuiBData.picess[pice][color][bkg] == NULL){
						return -1;
					}
				}
			}
		}
	}

	for (bkg = 0; bkg < 2; bkg++){
		for (isColored = 0; isColored < 2; isColored++){
			char path[500];
			sprintf(path, "./images/board/blank_%c_%d.bmp", colors[bkg], isColored);
			GuiBData.emptys[bkg][isColored] = loadImage(path);
			if (GuiBData.emptys[bkg] == NULL){
				return -1;
			}
		}
	}
	return 0;
}

SDL_Surface* getPiceImage(int x, int y, int isColored){ //x,y are GUI base
	int bkg = x % 2 == y % 2 ? GUI_white : GUI_black;
	char pice = getPice(createLocationNode( x, BOARD_SIZE-1-y));
	if (pice == EMPTY){
		return GuiBData.emptys[bkg][isColored];
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

		return GuiBData.picess[pice_type][pice_color][bkg][isColored];
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
				GuiBData.pageID = -1;
				break;
			case (SDL_KEYUP) :
				//if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				handleBoardButtonClicked(e);
				quit = 1;
				break;
			default:
				break;
			}
		}
	}

	return 1;
}

int handleBoardButtonClicked(SDL_Event e){
	if (e.button.x > 75 * BOARD_SIZE){
		//TODO -butten
		return 1;
	}
	else {
		switch (GuiBData.pageID)
		{
		case 0:
			return eventHendelPage0(e);
		case 1:
			return eventHendelPage1(e);
		}
	}

	return 1;
}

int eventHendelPage0(SDL_Event e){
	GuiBData.origin = whichSquerWasClicked(e);
	if (isSameColorAsMe(GuiBData.origin, game_board.isBlackTurn)){
		moveList* moves = getValidMovesForLocation(GuiBData.origin, 0);
		if (moves == NULL){
			free_all_pices();
			//return ERROR;
		}
		colorSquers(moves, GuiBData.origin);
		//TODO is error?
		freeAllMoveList(moves);
		GuiBData.pageID = 1;
	}

	return 1;
}

int eventHendelPage1(SDL_Event e){
	if (e.button.x > 75 * BOARD_SIZE){
		//TODO -butten
	}
	else {
		locationNode clickedLoc = whichSquerWasClicked(e);
		moveList move;
		move.origin = GuiBData.origin;
		move.destination = clickedLoc; 
		move.soldierToPromoteTo = EMPTY;
		int isMoveValid = isValidMove(move, game_board.isBlackTurn, 0);
		if (isMoveValid){
			moveUser(move, game_board.isBlackTurn);
			GuiBData.pageID = settings.gameMode == TWO_PLAYERS? 0:2;
			game_board.isBlackTurn = game_board.isBlackTurn ? 0 : 1;
		}
		else{
			GuiBData.origin.column = -1;
			GuiBData.origin.row = -1;
			GuiBData.pageID = 0;
		}
	}

	return 1;
}

int colorASquere(locationNode loc){
	//x,y are gui base
	int x = loc.column * 75;
	int y = (BOARD_SIZE-1- loc.row) * 75;
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	rDest.x = x;
	rDest.y = y;
	SDL_Surface* img = getPiceImage(loc.column, BOARD_SIZE - 1 - loc.row, 1);
	if (!addImageToSurface(img, &rOrigin, GuiBData.surface, &rDest)){
		free_all_pices();
		return 0;
	}
	return 1;
}

int colorSquers(moveList* move,locationNode origin){
	if (!isEmptyMoveList(move)){
		moveList* cur = move;
		for (; cur != NULL; cur = cur->next){
			colorASquere(cur->destination);
		}
	colorASquere(origin);
	updateSurface(GuiBData.surface);
	}

	return 1;
}

int pageID0(){
	//ret: -1 error, 0 ok
	int MateTieCheck = Mate_Tie_Check();
	if (MateTieCheck != 0){
		if (MateTieCheck == -1 || -1 == print_messege(MateTieCheck)){
			return -1;
		}
		if (MateTieCheck == MATE1 || MateTieCheck ==TIE1 ){
			GuiBData.pageID = -1;
			return 0;
		}
	}
	createBoard(GuiBData.surface);
	updateSurface(GuiBData.surface);
	handleBoardEvents();
	return 0;
}

int pageID1(){
	handleBoardEvents();
	createBoard(GuiBData.surface);
	updateSurface(GuiBData.surface);

	return 1;
}

int pageID2(){
	//TODO messege- wait
	//ret: -1 error, 0 ok
	int MateTieCheck = Mate_Tie_Check();
	if (MateTieCheck != 0){
		if (MateTieCheck == -1 ||-1 == print_messege(MateTieCheck)){
			return -1;
		}
		if (MateTieCheck == MATE1 || MateTieCheck == TIE1){
			GuiBData.pageID = -1;
			return 0;
		}
	}
	computerTurn(0);
	GuiBData.pageID = 0;
	game_board.isBlackTurn = game_board.isBlackTurn ? 0 : 1;
	createBoard(GuiBData.surface);
	updateSurface(GuiBData.surface);

	return 1;
}

int pageIDMinus1(){
	//TODO write
	return 0;
}

void free_all_pices(){
	int pice, color, bkg, isColored ; 
	for (pice = 0; pice < 6; pice++){
		for (color = 0; color < 2; color++){
			for (bkg = 0; bkg < 2; bkg++){
				for (isColored = 0; isColored < 2; isColored++){
					SDL_Surface* toFree = GuiBData.picess[pice][color][bkg][isColored];
					if (toFree != NULL){
						SDL_FreeSurface(toFree);
					}
				}
			}
		}
	}
	for (bkg = 0; bkg < 2; bkg++){
		for (isColored = 0; isColored < 2; isColored++){
			SDL_FreeSurface(GuiBData.emptys[bkg][isColored]);
		}
	}
}

int Mate_Tie_Check(){
	//ret= -1 if error, 0 noting, 1 mate, 2 cheack, 3 tie
	int isBlack = game_board.isBlackTurn;

	int isCheck = amIThreatened(isBlack) == 1;

	if (isCheck == 2){
		// ERROR
		return -1;
	}
	moveList* list = getAllValidMoves(isBlack, 0);
	if (list == NULL){
		// ERROR
		return -1;
	}
	int isListEmpty = isEmptyMoveList(list);
	freeAllMoveList(list);



	if (isCheck && isListEmpty){
		// MATE
		return 1;
	}

	if (isCheck && !isListEmpty){
		// CHECK
		return 2;
	}
	if (!isCheck && isListEmpty){
		// TIE
		return 3;
	}
	return 0;
}

int print_messege(int Mate_Tie_Check){
	//TODO - print status
	//ret -1 if error, 0 SABABA
	printf("Mate_Tie_Check = %d", Mate_Tie_Check);
	return 0;

}

