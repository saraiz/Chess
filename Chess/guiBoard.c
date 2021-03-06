#include "guiBoard.h"


char colors[] = { 'b', 'w' };
char pice_types[] = { 'b', 'k', 'm', 'n', 'q', 'r' };
GuiBoardData GuiBData;


void GuiBoardStart(int isSetBoard){ //main
	GuiBData.surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	GuiBData.main_quit = 0;
	GuiBData.set_quit = 0;

	load_all_pices();

	if (isSetBoard){
		startSet(); //usr wanted to set board
	}

	startGame();
}

int startGame(){

	clear_My_screen();
	createButtens();
	createBoard();
	int isPVC = settings.gameMode == PLAYER_VS_AI;
	int isCompFirst = settings.isUserBlack != game_board.isBlackTurn;
	GuiBData.pageID = isPVC &&  isCompFirst ? 2 : 0;
	while (!GuiBData.main_quit){ // main loop
		//printf("%d\n", GuiBData.pageID);
		switch (GuiBData.pageID)
			/*
			-1- quit,
			0- user need to chose what to move,
			1- user need to chose where to move,
			2- computer turn
			3- PP promotion
			4- best moves
			5- PP save*/
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
	return 1;
}

int startSet(){
	GuiBData.pageID = 6;
	clear_My_screen();

	if (!create_set_side()){
		return 0;
	}

	createBoard();
	load_set_popup();

	while (!GuiBData.set_quit){ //main loop
		//printf("%d\n", GuiBData.pageID);
		switch (GuiBData.pageID){
			/*	6- set: select place
				7- set: PP select pice
				8- set: PP error
				*/
		case 6:
			pageID6();
			break;
		case 7:
			pageID7();
			break;
		case 8:
			pageID8();
			break;
		}
		

	}
	return 1;
}

int handleBoardEvents(){
	//ret 0 error 1 sababa
	SDL_Event e;
	GuiBData.pull_quit = 0;
	while (!GuiBData.pull_quit){ //event loop
		while (SDL_PollEvent(&e) != 0){
			switch (e.type) {
			case (SDL_QUIT) :
				GuiBData.pull_quit = 1;
				GuiBData.main_quit = 1;
				GuiBData.set_quit = 1;
				GuiBData.pageID = -1;
				break;
			case (SDL_KEYUP) :
				//if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) : //click
				if (GuiBData.pageID < 6){
					if (!handleBoardButtonClicked(e)){ //plaing mood
						return 0;
					}
				}
				else {
					if (!handleSetButtonClicked(e)){ //setting mode
						return 0;
					}
				}
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
	//play board click event hendler
	if (e.button.x > 75 * BOARD_SIZE && GuiBData.pageID != 3 && GuiBData.pageID != 5){
		//user pressed outside board
		int i,btnID = -1;
		for (i = 0; i < 4; i++){
			if (isClickInRect(e, GuiBData.boardBtn[i].buttonsDestRect)){
				btnID = GuiBData.boardBtn[i].id;
				break;
			}
		}
		switch (btnID){
			//button event hendler
		case 0: //main menu
			free_all_pices();
			buildSettingsWindow();
			GuiBData.main_quit = 1;
			GuiBData.pull_quit = 1;
			return 1;
		case 1: //best moves
			if (GuiBData.pageID == 0 || GuiBData.pageID == 1){
				GuiBData.pageID = 4;
				GuiBData.pull_quit = 1;
			}
			break;
		case 2: //save
			if (GuiBData.pageID == 0 || GuiBData.pageID == 1){
				GuiBData.pageID = 5;
				GuiBData.pull_quit = 1;
			}
			break;
		case 3: //quit
			GuiBData.main_quit = 1;
			GuiBData.pull_quit = 1;
			return 1;
		case -1: //empty
			break;
		}
	}
	else {
		// user pressed board
		switch (GuiBData.pageID)
			//board presses
		{
		case 0:
			return GuiBData.pull_quit = eventHendelPage0(e);
		case 1:
			return GuiBData.pull_quit = eventHendelPage1(e);
		case 3:
			return GuiBData.pull_quit = eventHendelPage3(e);
		case 4:
			return GuiBData.pull_quit = eventHendelPage4(e);
		case 5:
			return GuiBData.pull_quit = eventHendelPage5(e);
		case -1:
			return 1;
		}
	}
	return 1;
}

int handleSetButtonClicked(SDL_Event e){
	//same thing as before with set screen
	if (e.button.x > 75 * BOARD_SIZE&& GuiBData.pageID != 7 && GuiBData.pageID != 8 && GuiBData.pageID != 4){
		int i, btnID = -1;
		for (i = 0; i < 3; i++){
			if (isClickInRect(e, GuiBData.set_side_btn [i].buttonsDestRect)){
				btnID = GuiBData.set_side_btn[i].id;
				break;
			}
		}
		switch (btnID){
		case 0: //next
			if (GuiBData.pageID == 6){
				if (isBoardValidToStartGame(0)){
					GuiBData.set_quit = 1;
					GuiBData.pull_quit = 1;
				}
				else {
					GuiBData.set_which_error_to_print = 1;
					GuiBData.pageID = 8;
					GuiBData.pull_quit = 1;
				}
			}
			break;
		case 1:// clear
			rmAll();
			GuiBData.pageID = 6;
			GuiBData.pull_quit = 1;
			break;
		case 2: //cancel
			GuiBData.set_quit = 1;
			free_all_pices();
			buildSettingsWindow();
			GuiBData.main_quit = 1;
			GuiBData.pull_quit = 1;
			break;
		case -1: //noting minigfull was pressed
			break;
		}
	}
	else{
		switch (GuiBData.pageID)
		{
		case 6:
			return GuiBData.pull_quit = eventHendelPage6(e);
		case 7:
			return GuiBData.pull_quit = eventHendelPage7(e);
		case 8:
			return GuiBData.pull_quit = eventHendelPage8(e);
		}
	}
	return 1;
}


void free_all_pices(){
	//free everything
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

	for (btnNum = 0; btnNum < 3; btnNum++){
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

	int num;
	for (num = 0; num < 2; num++){
		my_sdl_free(GuiBData.getMove[num]);
		GuiBData.getMove[num] = NULL;
	}
	for (btnNum = 0; btnNum < 2; btnNum++){
		my_sdl_free (GuiBData.set_error[btnNum]);
		GuiBData.set_error[btnNum] = NULL;
	}
	

	int side;
	for (side = 0; side < 6; side++){
		my_sdl_free(GuiBData.sideBar[side]);
		GuiBData.sideBar[side] = NULL;
	}
	my_sdl_free(GuiBData.set_popup);
	GuiBData.set_popup = NULL;

	my_sdl_free(GuiBData.set_ok.img);
	GuiBData.set_ok.img = NULL;

	my_sdl_free(GuiBData.surface);
	GuiBData.surface = NULL;

	for (btnNum = 0; btnNum < 6; btnNum++){
		my_sdl_free(GuiBData.Diff_btn[btnNum].img);
		GuiBData.Diff_btn[btnNum].img = NULL;
	}

}

int load_all_pices(){
	//load everyting
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

	int i;
	for (i = 0; i < 2; i++){
		char path[200];
		sprintf(path, "./images/set/setError%d.bmp", i);
		GuiBData.set_error[i] = loadImage(path);
		if (GuiBData.set_error[i] == NULL){
			return 0;
		}
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
	sprintf(path, pathStart, "ChooseDifficulty");
	GuiBData.getMove[0] = loadImage(path);
	sprintf(path, pathStart, "pleaseWait");
	GuiBData.getMove[1] = loadImage(path);

	create_best_move_pp_btn();


	return 1;
}
 
int load_set_popup(){
	//load set popup btns
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

	GuiBData.set_ok = createButton("./images/set/set_ok.bmp",0,115,30,243 ,350);
	return 1;
}

int createButtens(){
	//create sidebar btn
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

void create_best_move_pp_btn(){
	int btnNum;
	char path[200];
	for (btnNum = 0; btnNum < 5; btnNum++){
		sprintf(path, "./images/popupsAndButtons/slot%d_btn.bmp",btnNum);
		GuiBData.Diff_btn[btnNum] = createButton(path, btnNum, 45, 40,115 + btnNum*80,270 );

	}
	sprintf(path, "./images/popupsAndButtons/slot%d_btn.bmp", 5);
	GuiBData.Diff_btn[5] = createButton(path, btnNum, 115, 30,242,350 );
}

SDL_Surface* getPiceImage(int x, int y, int isColored){ 
	//x,y are GUI base
	//return the img to pring at (x,y)

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

char get_pice_char_from_set_btn_id(int btnID){
	//return char of pice represented in btn
	int isBlack = btnID > 5 ? 0 : 1;
	if (!isBlack){
		btnID = btnID - 6;
	}
	char pice = pice_types[btnID];
	
	if (isBlack){
		pice = toupper(pice);
	}
	return pice;
}

int Mate_Tie_Check(){
	//test if current boars is  mate cheack or tie
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

int isPromotion(moveList moveToDo){
	// moveToDo is the move you wish to test
	//return true if move shuld have a promotion popup
	int is_man = tolower(getPice(moveToDo.origin)) == WHITE_P;
	int isDest = moveToDo.destination.row == (game_board.isBlackTurn ? 0 : 7);
	int isOri = moveToDo.origin.row == (game_board.isBlackTurn ? 1 : 6);
	int isCol = moveToDo.origin.column == moveToDo.destination.column;

	int is_to_promote = isDest && isOri&& isCol;
	return is_man && is_to_promote;
}

int do_usr_move(){
	//execute move
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

void rmAll(){
	//clear all picess on board
	int i;
	for (i = 0; i < BOARD_SIZE; i++){
		int j;
		for (j = 0; j < BOARD_SIZE; j++){
			locationNode loc = createLocationNode(i, j);
			removeUser(loc);
		}
	}
}


int createBoard(){ //0 if fail, 1 if ok
	//print board to screen
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
	//print save popup 
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

void clear_My_screen(){
	//delete everyting on screen
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
	//print set side bar
	int btnNum;
	int startY = 30;

	for (btnNum = 0; btnNum < 3; btnNum++){
		char path[500];
		sprintf(path, "./images/popupsAndButtons/btnset%d.bmp", btnNum);
		GuiBData.set_side_btn[btnNum] = createButton(path, btnNum, 150, 42, 625, btnNum * 47 + startY);

	}
	if (!addButtons(GuiBData.set_side_btn, 3, GuiBData.surface)){
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

int colorASquere(locationNode loc){
	//add yellow rect arrornd a squere
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
	//add yellow rect to evety move->dest and origin
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

int print_side_bar(int Mate_Tie_Check){
	//ret 0 if error, 1 SABABA
	// Mate_Tie_Check = 0 noting, 1 mate, 2 cheack, 3 tie
	//print Mate_Tie_Check banner
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
	//ret 0 if error, 1 SABABA
	// print or delete comp turn banner
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

int print_please_wait(){
	//ret 0 if error, 1 SABABA
	// print please wait for get moves
	SDL_Rect rDest = { 620, 220, 160, 180 };
	SDL_Rect rOrigin = { 0, 0, 160, 180 };
	SDL_Surface* image;

	image = GuiBData.getMove[1];



	if (!addImageToSurface(image, &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}
	return 1;

}

int printSetError(){
	//print  setting error- invalid board
	SDL_Rect rOrigin = { 0, 0, 400, 200 };
	SDL_Rect rDest = { 100, 200, 400, 200 };
	if (!addImageToSurface(GuiBData.set_error[GuiBData.set_which_error_to_print], &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}

	if (!addButtons(&GuiBData.set_ok, 1, GuiBData.surface)){
		return 0;
	}

	return 1;
}

int printDiffPP(){
	//print diffuclty popup for get moves
	SDL_Rect rOrigin = { 0, 0, 400, 200 };
	SDL_Rect rDest = { 100, 200, 400, 200 };
	if (!addImageToSurface(GuiBData.getMove[0], &rOrigin, GuiBData.surface, &rDest)){
		return 0;
	}

	if (!addButtons(GuiBData.Diff_btn, 6, GuiBData.surface)){
		return 0;
	}
	return 1;
}


//event hendlers for different states
int eventHendelPage0(SDL_Event e){
	// 0 error, 1 sababa
	createBoard();
	updateSurface(GuiBData.surface);

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

if (!isEmptyMoveList(moves)){
	GuiBData.pageID = 1;

}
freeAllMoveList(moves);
	}
	return 1;
}

int eventHendelPage1(SDL_Event e){

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
			return 0;
		}
		GuiBData.moveToDo.origin = GuiBData.moveToDo.destination = createLocationNode(-1, -1);
		GuiBData.moveToDo.soldierToPromoteTo = EMPTY;
		GuiBData.moveToDo.next = NULL;
	}

	return 1;
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

int eventHendelPage4(SDL_Event e){
	int btnNum;
	int isClicked=0;
	for (btnNum = 0; btnNum < 6; btnNum++){
		if (isClickInRect(e, GuiBData.Diff_btn[btnNum].buttonsDestRect)){
			isClicked = 1;
			break;
		}
	}
	if (isClicked){		
		GuiBData.minmaxDepth = btnNum < 4 ? btnNum + 1 : 3 - btnNum;
		return 1;
	}
	return 0;
}

int eventHendelPage5(SDL_Event e){
	//ret 1 sababa 0 noting was pressed 2 ERROR
	//int y = e.button.y;
	//int x = e.button.x;
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
	int wasClicked = -1;
	int color; //b,w
	for (color = 0; color < 2; color++){
		int pice; //[b / k / m / n / q / r]
		for (pice = 0; pice < 6; pice++){
			if (isClickInRect(e, GuiBData.set_popup_pices[pice][color].buttonsDestRect)){
				wasClicked = GuiBData.set_popup_pices[pice][color].id;
				break;
			}

		}
	}
	if (wasClicked != -1){
		GuiBData.toSet = get_pice_char_from_set_btn_id(wasClicked);
		return 1;
	}
	else{
		int btnID;
		for (btnID = 0; btnID < 2; btnID++){
			if (isClickInRect(e, GuiBData.set_popup_btn[btnID].buttonsDestRect)){
				wasClicked = GuiBData.set_popup_btn[btnID].id;
				break;
			}
		}
		if (wasClicked == 0){
			GuiBData.toSet = -1;
			return 1;
		}
		if (wasClicked == 1){
			GuiBData.toSet = EMPTY;
			return 1;
		}
		
	}
	return 0;
}

int eventHendelPage8(SDL_Event e){
	if (isClickInRect(e, GuiBData.set_ok.buttonsDestRect)){
		return 1;
	}
	return 0;
}


//page manegers
int pageID0(){
	//ret: 0 error, 1 ok

	int MateTieCheck = Mate_Tie_Check();

	if (!print_comp_turn(0)){
		return 0;
	}

	if (MateTieCheck == -1 || !print_side_bar(MateTieCheck)){
		return 0;
	}

	if (MateTieCheck == MATE1 || MateTieCheck ==TIE1 ){
		GuiBData.pageID = -1;
		return 1;
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
	while (SDL_PollEvent(&e) != 0);
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
		my_sdl_free(promotionScreen);
		return 0;
	}
	my_sdl_free(promotionScreen);

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


	if (settings.gameMode == PLAYER_VS_AI){
		GuiBData.minmaxDepth = settings.minmax_depth;
	}
	else{
		if (!printDiffPP()){
			return 0;
		}
		handleBoardEvents();
	}
	if (GuiBData.minmaxDepth != -2){

		if (!print_please_wait()){
			return 0;
		}

		createBoard(GuiBData.surface);

		if (!updateSurface(GuiBData.surface)){
			return 0;
		}

		moveList bestMove = GuiGetBestMove(GuiBData.minmaxDepth);
		bestMove.next = NULL;
		colorSquers(&bestMove, bestMove.origin);
		if (!updateSurface(GuiBData.surface)){
			return 0;
		}
	}
	else{
		createBoard(GuiBData.surface);

		if (!updateSurface(GuiBData.surface)){
			return 0;
		}

	}
	GuiBData.pageID = 0;
	return 1;
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

	if (GuiBData.toSet == -1){
		GuiBData.pageID = 6;
		return 1;
	}
	if (GuiBData.toSet != EMPTY){
		char* color;
		color = isupper(GuiBData.toSet) ? COLOR_BLACK : COLOR_WHITE;
		char* pice = convertSoldierTypeToSoldierName(GuiBData.toSet);
		if (isBoardValidAfterSet(pice, color, 0)){
			removeUser(GuiBData.wasClicked);
			addUser(GuiBData.wasClicked, color, pice);
			GuiBData.pageID = 6;
		}
		else{
			GuiBData.set_which_error_to_print = 0;
			GuiBData.pageID = 8;
			
		}
	myFree(pice);
	}
	else{
		removeUser(GuiBData.wasClicked);
		GuiBData.pageID = 6;
	}
	return 1;
}

int pageID8(){
	if (!printSetError()){
		return 0;
	}

	if (!updateSurface(GuiBData.surface)){
		return 0;
	}
	if (!handleBoardEvents()){
		return 0;
	}
	GuiBData.pageID = 6;
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