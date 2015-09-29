#include "guiBoard.h"


char colors[] = { 'b', 'w' };
char pice_types[] = { 'b', 'k', 'm', 'n', 'q', 'r' };
GuiBoardData GuiBData;


void GuiBoardStart(int isSetBoard){
	GuiBData.surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	GuiBData.main_quit = 0;
	GuiBData.set_quit = 0;

	load_all_pices();

	if (isSetBoard){
		startSet();
	}

	startGame();
}

int startGame(){
	//settings.gameMode = PLAYER_VS_AI; //TODO delete
	//settings.isUserBlack = 1;

	clear_screen();
	createButtens();
	createBoard();
	int isPVC = settings.gameMode == PLAYER_VS_AI;
	int isCompFirst = settings.isUserBlack != game_board.isBlackTurn;
	GuiBData.pageID = isPVC &&  isCompFirst ? 2 : 0;
	while (!GuiBData.main_quit){
		printf("%d\n", GuiBData.pageID);
		switch (GuiBData.pageID)
		{
		case 0:
			if (!pageID0()){
				GuiBData.main_quit = 1;
				break;
			}
			break;
		case 1:
			if (!pageID1()){
				GuiBData.main_quit = 1;
				break;
			}
			break;
		case 2:
			if (!pageID2()){
				GuiBData.main_quit = 1;
				break;
			}
			break;
		case 3:
			if (!pageID3()){
				GuiBData.main_quit = 1;
				break;
			}
			break;
		case 4:
			if (!pageID4()){
				GuiBData.main_quit = 1;
				break;
			}
			break;
		case 5:
			if (!pageID5()){
				GuiBData.main_quit = 1;
				break;
			}
			break;

		case -1:
			pageIDMinus1();
			GuiBData.main_quit = 1;
			break;
		}
	
	}

	free_all_pices();
}

int startSet(){
	GuiBData.pageID = 6;
	clear_screen();

	if (!create_set_side()){
		return 0;
	}

	createBoard();
	load_set_popup();

	while (!GuiBData.set_quit){
		printf("%d\n", GuiBData.pageID);
		switch (GuiBData.pageID){
		case 6:
			pageID6();
			break;
		case 7:
			pageID7();
			break;
		case 8:
			break;
		}
		

	}
	return 1;
}

void clear_screen(){

	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	if (SDL_FillRect(GuiBData.surface, &rect, SDL_MapRGB(GuiBData.surface->format, 244, 208, 159)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}

	SDL_Rect rect2 = { 600, 0, 3, 600 };

	if (SDL_FillRect(GuiBData.surface, &rect2, SDL_MapRGB(GuiBData.surface->format, 185, 122, 87)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}

}

int create_set_side(){
	int btnNum;
	int startY = 30;

	for (btnNum = 0; btnNum < 2; btnNum++){
		char path[500];
		sprintf(path, "./images/popupsAndButtons/btnset%d.bmp", btnNum);
		GuiBData.set_side_btn[btnNum] = createButton(path, btnNum, 115, 30, 642, btnNum * 47 + startY);

	}
	if (!addButtons(GuiBData.set_side_btn, 2, GuiBData.surface)){
		return 0;
	}
	return 1;
}

int load_set_popup(){
	int btnID;
	for (btnID = 0; btnID < 2; btnID++){
		char path[500];
		sprintf(path, "./images/set/Btn%d.bmp", btnID);
		GuiBData.set_popup_btn[btnID] = createButton(path, btnID, 115, 30, 146 + 194 * btnID, 385);
	}

	int color; //b,w
	for (color = 0; color < 2; color++){
		int pice; //[b / k / m / n / q / r]
		for (pice = 0; pice < 6; pice++){
			char path[500];
			sprintf(path, "./images/set/%d_%d.bmp", pice,color);
			GuiBData.set_popup_pices[pice][color] = createButton(path,pice+6*color,75,75,20 +pice*95,182+110*color);
		}
	}
}

int createButtens(){
	//return 0 error, 1 sababa
	int btnNum;
	int startY = 30;

	for (btnNum = 0; btnNum < 4; btnNum++){
		char path[500];
		sprintf( path,"./images/popupsAndButtons/btn%d.bmp", btnNum);
		GuiBData.boardBtn[btnNum] =  createButton(path, btnNum, 150, 42, 625, btnNum* 47 + startY);

	}
	if (!addButtons(GuiBData.boardBtn, 4, GuiBData.surface)){
		return 0;
	}
	return 1;
}

int print_set_popup(){
	SDL_Rect rOrigin = { 0, 0, 580, 270 };
	SDL_Rect rDest = { 10, 165, 580, 270 };

	addImageToSurface(GuiBData.set_popup, &rOrigin, GuiBData.surface, &rDest);
	int pice;
	for (pice = 0; pice < 6; pice++){
		addButtons(GuiBData.set_popup_pices[pice], 2, GuiBData.surface);
	}
	addButtons(GuiBData.set_popup_btn, 2, GuiBData.surface);
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
	return 1;
}

int createSave(){
	// 0 eroor, 1 sabba
	SDL_Surface* img = loadImage("./images/popupsAndButtons/saveGame.bmp");
	SDL_Rect rOrigin = { 0, 0, 400, 200 };
	SDL_Rect rDest = { 100, 200, 400, 200 };
	if (!addImageToSurface(img, &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}


	my_sdl_free(img);
	return 1;
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

	GuiBData.set_popup = loadImage("./images/set/setPopup.bmp");
	if (GuiBData.set_popup == NULL){
		return 0;
	}

	char* pathStart = "./images/popupsAndButtons/%s.bmp";
	char path[200];
	sprintf(path, pathStart, "Check");
	GuiBData.sideBar[0] = loadImage(path);
	sprintf(path, pathStart, "tie");
	GuiBData.sideBar[1] = loadImage(path);
	sprintf(path, pathStart, "mate_b");
	GuiBData.sideBar[2] = loadImage(path);
	sprintf(path, pathStart, "mate_w");
	GuiBData.sideBar[3] = loadImage(path);
	sprintf(path, pathStart, "empty");
	GuiBData.sideBar[4] = loadImage(path);
	sprintf(path, pathStart, "computerPlaying");
	GuiBData.sideBar[5] = loadImage(path);

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
				GuiBData.main_quit = 1;
				GuiBData.set_quit = 1;
				GuiBData.pageID = -1;
				break;
			case (SDL_KEYUP) :
				//if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				if (GuiBData.pageID < 6){
					if (!handleBoardButtonClicked(e)){
						return 0;
					}
				}
				else {
					if (!handleSetButtonClicked(e)){
						return 0;
					}
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
		int i,btnID = -1;
		for (i = 0; i < 4; i++){
			if (isClickInRect(e, GuiBData.boardBtn[i].buttonsDestRect)){
				btnID = GuiBData.boardBtn[i].id;
				break;
			}
		}
		switch (btnID){
		case 0: //main menu
			free_all_pices();
			buildSettingsWindow();
			GuiBData.main_quit = 1;
			return 1;
		case 1: //best moves
			if (GuiBData.pageID == 0 || GuiBData.pageID == 1){
				GuiBData.pageID = 4;
			}
			break;
		case 2: //save
			if (GuiBData.pageID == 0 || GuiBData.pageID == 1){
				GuiBData.pageID = 5;
			}
			break;
		case 3: //quit
			GuiBData.main_quit = 1;
			return 1;
		case -1: //empty
			break;
		}
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
		case 5:
			while (!eventHendelPage5(e));
		case -1:
			return 1;
		}
	}
}

int handleSetButtonClicked(SDL_Event e){
	if (e.button.x > 75 * BOARD_SIZE){
		int i, btnID = -1;
		for (i = 0; i < 2; i++){
			if (isClickInRect(e, GuiBData.set_side_btn [i].buttonsDestRect)){
				btnID = GuiBData.boardBtn[i].id;
				break;
			}
		}
		switch (btnID){
		case 0: //next
			if (GuiBData.pageID = 6){
				GuiBData.set_quit = 1;
			}
			break;
		case 1: //cancel
			//TODO--???
			GuiBData.set_quit = 1;
			free_all_pices();
			buildSettingsWindow();
			GuiBData.main_quit = 1;
			break;
		case -1: //empty
			break;
		}
	}
	else{
		switch (GuiBData.pageID)
		{
		case 6:
			return eventHendelPage6(e);
		case 7:
			return eventHendelPage7(e);
		case 8:
			return eventHendelPage8(e);
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
		if (!isEmptyMoveList(moves)){
			GuiBData.pageID = 1;

		}
		freeAllMoveList(moves);
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

int eventHendelPage5(SDL_Event e){
	//ret 1 sababa 0 noting was pressed 2 ERROR
	int y = e.button.y;
	int x = e.button.x;
	SDL_Rect rSlot = { 88 ,276 , 44, 40 };
	int slotNum;
	int clickedSlot = -1;
	for (slotNum = 1; slotNum < 8; slotNum++){
		rSlot.x = rSlot.x + 44 + 7;
		if (isClickInRect(e, rSlot)){
			clickedSlot = slotNum;
			break;
		}
	}
	if (clickedSlot != -1){
		char path[200] = "./slots/slot%d.xml";
		sprintf(path, path, clickedSlot);
		fileData data;
		data.difficulty = settings.minmax_depth;
		data.gameMode = settings.gameMode;
		data.isNextBlack = game_board.isBlackTurn;
		data.isUserColorBlack = settings.isUserBlack;
		if (1 == saveGame(data, path)){
			print_message(WRONG_FILE_NAME);
			GuiBData.main_quit = 1;
			return 1;
		}

		return 1;
	}
	else{
		SDL_Rect rCancel = { 367, 358, 114, 29 };
		if (isClickInRect(e, rCancel)){

			return 1;
		}
	}
	return 0;
}

int eventHendelPage6(SDL_Event e){
	GuiBData.wasClicked = whichSquerWasClicked(e);
	GuiBData.pageID = 7;
	return 1;
}

int eventHendelPage7(SDL_Event e){

}

int eventHendelPage8(SDL_Event e){

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

	int MateTieCheck = Mate_Tie_Check();

	if (MateTieCheck == -1 || !print_side_bar(MateTieCheck)){
		return 0;
	}

	if (MateTieCheck == MATE1 || MateTieCheck ==TIE1 ){
		GuiBData.pageID = -1;
		return 1;
	}
	
	if (!print_comp_turn(0)){
		return 0;
	}

	if (!updateSurface(GuiBData.surface)){
		return 0;
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

	if (MateTieCheck == -1 || !print_side_bar(MateTieCheck)){
		return 0;
	}
	if (MateTieCheck == MATE1 || MateTieCheck == TIE1){
		GuiBData.pageID = -1;
		return 1;
	}

	if (!print_comp_turn(1)){
		return 0;
	}

	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	computerTurn(0);
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0); //TODO to remove?
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

int pageID4(){
	//return: 0 error, 1 sababa
	createBoard(GuiBData.surface);

	// TODO get best move and color squere
	moveList bestMove =  GuiGetBestMove();
	bestMove.next = NULL;
	colorSquers(&bestMove, bestMove.origin);
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	GuiBData.pageID = 0;

}

int pageID5(){
	if (!createSave()){
		return 0;
	}
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	handleBoardEvents();
	GuiBData.pageID = 0;
	createBoard();
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	return 1;
}

int pageID6(){
	createBoard();
	if (!updateSurface(GuiBData.surface)){
		return 0;
	}

	if (!handleBoardEvents()){
		return 0;
	}
	return 1;
}

int pageID7(){
	print_set_popup();

	if (!handleBoardEvents()){
		return 0;
	}
	// TODO:
	//if error
	//set if not
	//change page
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
						my_sdl_free( GuiBData.picess[pice][color][bkg][isColored]);
						GuiBData.picess[pice][color][bkg][isColored] = NULL;
					
				}
			}
		}
	}
	for (bkg = 0; bkg < 2; bkg++){
		for (isColored = 0; isColored < 2; isColored++){
			my_sdl_free(GuiBData.emptys[bkg][isColored]);
			GuiBData.emptys[bkg][isColored] = NULL;
		}
	}
	int btnNum;
	for (btnNum = 0; btnNum < 4; btnNum++){
		my_sdl_free(GuiBData.boardBtn[btnNum].img);
		GuiBData.boardBtn[btnNum].img = NULL;
	}

	for (btnNum = 0; btnNum < 2; btnNum++){
		my_sdl_free(GuiBData.set_side_btn[btnNum].img);
		GuiBData.set_side_btn[btnNum].img = NULL;
	}
	for (btnNum = 0; btnNum < 2; btnNum++){
		my_sdl_free(GuiBData.set_popup_btn[btnNum].img);
		GuiBData.set_popup_btn[btnNum].img = NULL;
	}

	for (color = 0; color < 2; color++){
		int pice; //[b / k / m / n / q / r]
		for (pice = 0; pice < 6; pice++){
			my_sdl_free(GuiBData.set_popup_pices[pice][color].img);
			GuiBData.set_popup_pices[pice][color].img = NULL;
		}
	}
	

	int side;
	for (side = 0; side < 6; side++){
		my_sdl_free(GuiBData.sideBar[side]);
		GuiBData.sideBar[side] = NULL;
	}
	my_sdl_free(GuiBData.set_popup);
	GuiBData.set_popup = NULL;
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


	int toreturn = 0;
	if (isCheck && isListEmpty){
		// MATE
		toreturn = 1;
	}

	if (isCheck && !isListEmpty){
		// CHECK
		toreturn = 2;
	}
	if (!isCheck && isListEmpty){
		// TIE
		toreturn = 3;
	}

	return toreturn;
}

int print_side_bar(int Mate_Tie_Check){
	//TODO - print status
	//ret 0 if error, 1 SABABA
	// Mate_Tie_Check = 0 noting, 1 mate, 2 cheack, 3 tie
	SDL_Rect rDest= {620 ,416 , 160, 180 };
	SDL_Rect rOrigin = { 0, 0, 160, 180 };
	SDL_Surface* image;
	switch (Mate_Tie_Check)
	{
	case 0:
		image = GuiBData.sideBar[4];
		break;
	case 1:
		image =game_board.isBlackTurn? GuiBData.sideBar[3] : GuiBData.sideBar[2];
		break;
	case 2:
		image =  GuiBData.sideBar[0];
		break;
	case 3:
		image = GuiBData.sideBar[1] ;
		break;

	}
	if (!addImageToSurface(image, &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}
	return 1;

}

int print_comp_turn(int is_comp_turn){
	//TODO - print status
	//ret 0 if error, 1 SABABA
	SDL_Rect rDest = { 620, 220, 160, 180 };
	SDL_Rect rOrigin = { 0, 0, 160, 180 };
	SDL_Surface* image;

	if (is_comp_turn){
		image = GuiBData.sideBar[5];
	}
	else {
		image = GuiBData.sideBar[4];
	}

	if (!addImageToSurface(image, &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}
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