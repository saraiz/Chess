#include "guiBoard.h"


char colors[] = { 'b', 'w' };
char pice_types[] = { 'b', 'k', 'm', 'n', 'q', 'r' };
GuiBoardData GuiBData;


void GuiBoardStart(){
	
	//settings.gameMode = PLAYER_VS_AI; //TODO delete
	//settings.isUserBlack = 1;
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	GuiBData.surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);


	if (SDL_FillRect(GuiBData.surface, &rect, SDL_MapRGB(GuiBData.surface->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}
	load_all_pices();
	createButtens();
	int isPVC = settings.gameMode == PLAYER_VS_AI;
	int isCompFirst = settings.isUserBlack != game_board.isBlackTurn;
	GuiBData.pageID = isPVC &&  isCompFirst ? 2 : 0;
	int quit = 0;
	while (!quit){
		switch (GuiBData.pageID)
		{
		case 0:
			if (!pageID0()){
				quit = 1;
				break;
			}
			break;
		case 1:
			if (!pageID1()){
				quit = 1;
				break;
			}
			break;
		case 2:
			if (!pageID2()){
				quit = 1;
				break;
			}
			break;
		case 3:
			if (!pageID3()){
				quit = 1;
				break;
			}
			break;

		case -1:
			pageIDMinus1();
			quit = 1;
			break;
		}
	
	}

	free_all_pices();
}

int createButtens(){
	//return 0 error, 1 sababa
	SDL_Rect rOrigin = { 0, 0, 150, 42 };
	int btnNum;
	for (btnNum = 0; btnNum < 4; btnNum++){
		char path[500];
		sprintf( path,"./images/popupsAndButtons/btn%d.bmp", btnNum);
		GuiBData.boardBtn[btnNum] =  createButton(path, btnNum, 150, 42, 625, btnNum* 47);

	}
	if (!addButtons(GuiBData.boardBtn, 4, GuiBData.surface)){
		return 0;
	}
	return 1;
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
	//ret: 0 error, else 1
	int pice, color, bkg, isColored;
	for (pice = 0; pice < 6; pice++){
		for (color = 0; color < 2; color++){
			for (bkg = 0; bkg < 2; bkg++){
				for (isColored = 0; isColored < 2; isColored++){
					char path[500];
					sprintf(path, "./images/board/%c_%c_%c_%d.bmp", pice_types[pice], colors[color], colors[bkg],isColored);
					GuiBData.picess[pice][color][bkg][isColored] = loadImage(path);
					if (GuiBData.picess[pice][color][bkg] == NULL){
						return 0;
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
				return 0;
			}
		}
	}
	return 1;
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
	//ret 0 error 1 sababa
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
				if (!handleBoardButtonClicked(e)){
					return 0;
				}
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
	//return 0 erroe, 1 sababa
	if (e.button.x > 75 * BOARD_SIZE){
		//TODO -butten
	}
	else {
		switch (GuiBData.pageID)
		{
		case 0:
			return eventHendelPage0(e);
		case 1:
			return eventHendelPage1(e);
		case 3:
			while (!eventHendelPage3(e));
		case -1:
			return 1;
		}
	}
}

int eventHendelPage0(SDL_Event e){
	// 0 error, 1 sababa
	GuiBData.moveToDo.origin = whichSquerWasClicked(e);
	if (isSameColorAsMe(GuiBData.moveToDo.origin, game_board.isBlackTurn)){
		moveList* moves = getValidMovesForLocation(GuiBData.moveToDo.origin, 0);
		if (moves == NULL){
			return 0;
			//return ERROR;
		}
		if (!colorSquers(moves, GuiBData.moveToDo.origin)){
			freeAllMoveList(moves);
			return 0;
		}
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
		//moveList move;

		GuiBData.moveToDo.destination = clickedLoc;
		GuiBData.moveToDo.soldierToPromoteTo = EMPTY;

		if (isPromotion(GuiBData.moveToDo)){
			GuiBData.moveToDo.soldierToPromoteTo = 0;
			GuiBData.pageID = 3;
		}
		else{
			GuiBData.moveToDo.soldierToPromoteTo = EMPTY;
			if (!do_usr_move()){
				//ERROR
			}
			GuiBData.moveToDo.origin = GuiBData.moveToDo.destination = createLocationNode(-1, -1);
			GuiBData.moveToDo.soldierToPromoteTo = EMPTY;
			GuiBData.moveToDo.next = NULL;
			return 1;
		}

	}
}

int eventHendelPage3(SDL_Event e){
	//x,y are sdl base
	//ret 1 sababa 0 noting was pressed
	int wAndh = 74;
	int space = 21;
	int xStart = 120;
	int yStart = 284;

	SDL_Rect bRect;;
	SDL_Rect qRect;
	SDL_Rect nRect;
	SDL_Rect rRect;

	bRect.x = xStart;
	bRect.y = yStart;
	bRect.w = bRect.h = wAndh;

	rRect.x = bRect.x + bRect.w + space;
	rRect.y = yStart;
	rRect.w = rRect.h = wAndh;

	nRect.x = rRect.x + rRect.w + space;
	nRect.y = yStart;
	nRect.w = nRect.h = wAndh;

	qRect.x = nRect.x + nRect.w + space;
	qRect.y = yStart;
	qRect.w = qRect.h = wAndh;


	if (isClickInRect(e, bRect)){
		GuiBData.moveToDo.soldierToPromoteTo = WHITE_B;
		return 1;
	}
	if (isClickInRect(e, qRect)){
		GuiBData.moveToDo.soldierToPromoteTo = WHITE_Q;
		return 1;
	}
	if (isClickInRect(e, nRect)){
		GuiBData.moveToDo.soldierToPromoteTo = WHITE_N;
		return 1;
	}
	if (isClickInRect(e, rRect)){
		GuiBData.moveToDo.soldierToPromoteTo = WHITE_R;
		return 1;
	}

	return 0;
	
}

int colorASquere(locationNode loc){
	//x,y are gui base
	//return 0 if error, 1 sababa
	int x = loc.column * 75;
	int y = (BOARD_SIZE-1- loc.row) * 75;
	SDL_Rect rOrigin = { 0, 0, 75, 75 };
	SDL_Rect rDest = { 0, 0, 75, 75 };
	rDest.x = x;
	rDest.y = y;
	SDL_Surface* img = getPiceImage(loc.column, BOARD_SIZE - 1 - loc.row, 1);
	if (!addImageToSurface(img, &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}
	return 1;
}

int colorSquers(moveList* move,locationNode origin){
	//ret 0 error, 1 sababa
	if (!isEmptyMoveList(move)){
		moveList* cur = move;
		for (; cur != NULL; cur = cur->next){
			if (!colorASquere(cur->destination)){
				return 0;
			}
		}
		if (!colorASquere(origin)){
			return 0;
		}
		if (!updateSurface(GuiBData.surface)){
			return 0;
		}
	}
	return 1;
}

int pageID0(){
	//ret: 0 error, 1 ok
	createBoard(GuiBData.surface);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	int MateTieCheck = Mate_Tie_Check();
	if (MateTieCheck != 0){
		if (MateTieCheck == -1 || ! print_messege(MateTieCheck)){
			return 0;
		}
		if (MateTieCheck == MATE1 || MateTieCheck ==TIE1 ){
			GuiBData.pageID = -1;
			return 1;
		}
	}
	handleBoardEvents();
	return 1;
}

int pageID1(){
	// ret 0 error, 1 sababa
	handleBoardEvents();
	createBoard(GuiBData.surface);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	return 1;
}

int pageID2(){
	//TODO messege- wait
	//ret: 0 error, 1 ok
	int MateTieCheck = Mate_Tie_Check();
	if (MateTieCheck != 0){
		if (MateTieCheck == -1 || !print_messege(MateTieCheck)){
			return 0;
		}
		if (MateTieCheck == MATE1 || MateTieCheck == TIE1){
			GuiBData.pageID = -1;
			return 1;
		}
	}
	computerTurn(0);
	GuiBData.pageID = 0;
	game_board.isBlackTurn = game_board.isBlackTurn ? 0 : 1;
	createBoard(GuiBData.surface);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	return 1;
}

int pageID3(){
	//return: 0 error, 1 sababa
	char path[100];
	sprintf(path, "./images/popupsAndButtons/promotion_popup_%c.bmp", game_board.isBlackTurn ? 'b' : 'w');
	SDL_Surface* promotionScreen = loadImage(path);
	if (promotionScreen == NULL){
		return 0;
	}
	SDL_Rect rOrigin = { 0, 0, 400, 200 };
	SDL_Rect rDest = { 100, 200, 400, 200 };

	if (!addImageToSurface(promotionScreen, &rOrigin, GuiBData.surface, &rDest)){
		SDL_FreeSurface(promotionScreen);
		return 0;
	}
	SDL_FreeSurface(promotionScreen);

	if (!updateSurface(GuiBData.surface)){
		return 0;
	}

	handleBoardEvents();

	if (!do_usr_move()){
		return 0;
	}
	createBoard(GuiBData.surface);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	return 1;
}

int pageIDMinus1(){
	createBoard(GuiBData.surface);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	handleBoardEvents();

	return 1;
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
	int btnNum;
	for (btnNum = 0; btnNum < 4; btnNum++){
		SDL_FreeSurface(GuiBData.boardBtn[btnNum].img);
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
	//ret 0 if error, 1 SABABA
	printf("Mate_Tie_Check = %d", Mate_Tie_Check);
	return 1;

}

int isPromotion(moveList moveToDo){
	// moveToDo is the move you wish to test
	int is_man = tolower(getPice(moveToDo.origin)) == WHITE_P;
	int isDest = moveToDo.destination.row == (game_board.isBlackTurn ? 0 : 7);
	int isOri = moveToDo.origin.row == (game_board.isBlackTurn ? 1 : 6);
	int isCol = moveToDo.origin.column == moveToDo.destination.column;

	int is_to_promote = isDest && isOri&& isCol;
	return is_man && is_to_promote;
}

int do_usr_move(){
	//ret 0 error, 1 sababa
	moveList move = GuiBData.moveToDo;
	int isMoveValid = isValidMove(move, game_board.isBlackTurn, 0);
	if (isMoveValid==2){
		return 0;
	}
	if (isMoveValid){
		moveUser(move, game_board.isBlackTurn);
		GuiBData.pageID = settings.gameMode == TWO_PLAYERS ? 0 : 2;
		game_board.isBlackTurn = game_board.isBlackTurn ? 0 : 1;
	}
	else{

		GuiBData.pageID = 0;
	}

	return 1;	
}