#include "GuiSettingsStateManager.h"

UserGuiSettings userGuiSettings = { 1, 0, 0, 0 };

int buildSettingsWindow(){
	int isSuccess = createMainContainer();
	if (isSuccess == 1){
		isSuccess = navigatToPage("mainMenu");
		if (isSuccess == 1){
			isSuccess = handleEvents();	
		}
	}

	removeCurrentPage();
	my_sdl_free(containerPage.page);

	if (isSuccess == 2){
		// navigate to start game
		GuiBoardStart(1);
	}
	else if (isSuccess == 3){
		// navigate to set board
	}

	return isSuccess;
}

int createMainContainer(){
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Surface *surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	containerPage.pageRect = rect;
	containerPage.page = surface;

	if (SDL_FillRect(containerPage.page, &containerPage.pageRect, SDL_MapRGB(containerPage.page->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

Page createMainMenuPage(){

	//Add Background
	SDL_Surface *bkg = loadImage(MAIN_MENU_BKG);
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	currentPage.bkg = bkg;

	// CreateButtons
	Button newGameBtn = createButton(NEW_GAME_BTN_URL, 1, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, 200);
	Button loadGameBtn = createButton(LOAD_GAME_BTN_URL, 2, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, newGameBtn.buttonsDestRect.y + newGameBtn.buttonsDestRect.h + SPACE);
	Button quitGameBtn = createButton(QUIT_GAME_BTN_URL, 3, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, loadGameBtn.buttonsDestRect.y + loadGameBtn.buttonsDestRect.h + SPACE);
	Button *btnLst = (Button*)myMalloc(sizeof(Button)*3);
	if (btnLst == NULL){
		currentPage.isError = 1;
		return currentPage;
	}

	btnLst[0] = newGameBtn;
	btnLst[1] = loadGameBtn;
	btnLst[2] = quitGameBtn;

	currentPage.btnList = btnLst;
	currentPage.btnListLen = 3;
	currentPage.id = 1;

	addButtons(btnLst, 3, containerPage.page);

	restorDefaultSettings();

	return currentPage;
}

Page createSelecetionPage(){

	//Add Background
	SDL_Surface *bkg = loadImage(SELECTION_WINDOW_BKG);
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	currentPage.bkg = bkg;

	// create game mode section
	Button game_mode_2PlayersMode = createButton(TWO_PLAYERS_BTN_URL, 1, 150, 42, 35, 50);
	Button game_mode_playerVsAI = createButton(AGAINS_COMPUTER_BTN_URL, 2, 150, 42, game_mode_2PlayersMode.buttonsDestRect.x + game_mode_2PlayersMode.buttonsDestRect.w + SPACE, 50);

	// create next player section
	Button next_player_white = createButton(WHITE_BTN_URL, 3, 150, 42, 35, 137);
	Button next_player_black = createButton(BLACK_BTN_URL, 4, 150, 42, next_player_white.buttonsDestRect.x + next_player_white.buttonsDestRect.w + SPACE, 137);

	// create next player section
	Button set_board_no = createButton(NO_BTN_URL, 5, 150, 42, 35, 222);
	Button set_board_yes = createButton(YES_BTN_URL, 6, 150, 42, set_board_no.buttonsDestRect.x + set_board_no.buttonsDestRect.w + SPACE, 222);

	// create ok and cancel buttons
	Button cancelButton = createButton(CANCEL_BTN_URL, 7, 150, 42, SCREEN_WIDTH - SPACE - 150, SCREEN_HEIGHT - 30 - 42);
	Button okButton = createButton(NEXT_BTN_URL, 8, 150, 42, cancelButton.buttonsDestRect.x - 30 - cancelButton.buttonsDestRect.w, cancelButton.buttonsDestRect.y);
	
	Button *btnLst = (Button*)myMalloc(sizeof(Button) * 8);
	if (btnLst == NULL){
		currentPage.isError = 1;
		return currentPage;
	}

	btnLst[0] = game_mode_2PlayersMode;
	btnLst[1] = game_mode_playerVsAI;
	btnLst[2] = next_player_black;
	btnLst[3] = next_player_white;
	btnLst[4] = set_board_yes;
	btnLst[5] = set_board_no;
	btnLst[6] = cancelButton;
	btnLst[7] = okButton;

	currentPage.btnList = btnLst;
	currentPage.btnListLen = 8;
	currentPage.id = 2;

	addButtons(btnLst, 8, containerPage.page);

	// set defaults
	Button defaultGameModeBtn = settings.gameMode == TWO_PLAYERS ? game_mode_2PlayersMode : game_mode_playerVsAI;
	char *defaultBtnSkin = settings.gameMode == TWO_PLAYERS ? TWO_PLAYERS_SELECTED_BTN_URL : AGAINS_COMPUTER_SELECTED_BTN_URL;
	int isSuccess = selectButton(defaultBtnSkin, defaultGameModeBtn);
	if (isSuccess == 1){
		userGuiSettings.gameMode = settings.gameMode;
		Button defaultNextPlayer = game_board.isBlackTurn == 1 ? next_player_black : next_player_white;
		defaultBtnSkin = game_board.isBlackTurn == 1 ? BLACK_SELECTED_BTN_URL : WHITE_SELECTED_BTN_URL;
		isSuccess = selectButton(defaultBtnSkin, defaultNextPlayer);
		if (isSuccess == 1){
			userGuiSettings.isNextPlayerBlack = game_board.isBlackTurn;
			isSuccess = selectButton(NO_SELECTED_BTN_URL, set_board_no);
			if (isSuccess == 1){
				userGuiSettings.isSetBoard = 0;
			}
		}
	}
	
	if (!isSuccess){
		currentPage.isError = 1;
		return currentPage;
	}
	
	updateSurface(containerPage.page);

	return currentPage;
}

Page createAiSettingsPage(){

	//Add Background
	SDL_Surface *bkg = loadImage(AI_SETTINGS_WINDOW);
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	currentPage.bkg = bkg;

	// create game mode section
	int y = 50;
	int x = 35;
	Button *btnLst = (Button*)myMalloc(sizeof(Button) * 9);
	if (btnLst == NULL){
		// TBD - free all images in current page
		currentPage.isError = 1;
		return currentPage;
	}

	char path[100];
	char *btn = "_btn.bmp";
	int width;
	int height;
		
	for (int i = 1; i <= 5; i++){
		if (i==5){
			sprintf(path, "%s", BEST_BTN_URL);
			width = 150;
			height = 42;
		}
		else{
			sprintf(path, "%s%d%s", SLOT_BTN_URL, i, btn);
			width = 50;
			height = 42;
		}
		
		Button difficulty = createButton(path, i, width, height, x, y);

		//y = y + slot.buttonsDestRect.h + SPACE;
		x = x + difficulty.buttonsDestRect.w + SPACE;
		btnLst[i - 1] = difficulty;
	}

	Button userColor_black = createButton(BLACK_BTN_URL, 6, 150, 42, 35, 145);
	Button userColor_white = createButton(WHITE_BTN_URL, 7, 150, 42, userColor_black.buttonsDestRect.x + userColor_black.buttonsDestRect.w + SPACE, 145);
	btnLst[5] = userColor_black;
	btnLst[6] = userColor_white;

	// create ok and cancel buttons
	Button cancelButton = createButton(CANCEL_BTN_URL, 8, 150, 42, SCREEN_WIDTH - SPACE - 150, SCREEN_HEIGHT - 30 - 42);
	Button okButton = createButton(NEXT_BTN_URL, 9, 150, 42, cancelButton.buttonsDestRect.x - 30 - cancelButton.buttonsDestRect.w, cancelButton.buttonsDestRect.y);
	btnLst[7] = cancelButton;
	btnLst[8] = okButton;


	currentPage.btnList = btnLst;
	currentPage.btnListLen = 9;
	currentPage.id = 4;

	addButtons(btnLst, 9, containerPage.page);

	// set defaults 
	unsigned int defaultDepth = settings.minmax_depth;
	if (defaultDepth == BEST){
		sprintf(path, "%s", BEST_SELECTED_BTN_URL);
		selectButton(path, btnLst[4]);
	}
	else{
		sprintf(path, "%s%d%s", SLOT_SELECTED_BTN_URL, defaultDepth, btn);
		selectButton(path, btnLst[defaultDepth-1]);
	}

	Button defaultUserColorBtn = settings.isUserBlack == 1 ? userColor_black : userColor_white;
	char *defaultBtnSkin = settings.isUserBlack == 1 ? BLACK_SELECTED_BTN_URL : WHITE_SELECTED_BTN_URL;

	selectButton(defaultBtnSkin, defaultUserColorBtn);
	userGuiSettings.difficulty = settings.minmax_depth;
	userGuiSettings.isUserColorBlack = settings.isUserBlack;

	updateSurface(containerPage.page);

	return currentPage;
}

Page createLoadFromSlotPage(){

	//Add Background
	SDL_Surface *bkg = loadImage(LOAD_FROM_SLOT_WINDOW);
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	currentPage.bkg = bkg;

	// create game mode section
	int y = 60;
	int x = 35;
	Button *btnLst = (Button*)myMalloc(sizeof(Button) * (NUM_OF_SLOTS + 2));
	if (btnLst == NULL){
		// TBD - free all images in current page
		currentPage.isError = 1;
		return currentPage;
	}

	char path[100];
	char *btn = "_btn.bmp";
	for (int i = 1; i <= NUM_OF_SLOTS; i++){
		sprintf(path, "%s%d%s", SLOT_BTN_URL, i, btn);
		Button slot = createButton(path, i, 50, 42, x, y);

		//y = y + slot.buttonsDestRect.h + SPACE;
		x = x + slot.buttonsDestRect.w + SPACE;
		btnLst[i - 1] = slot;
	}

	// create ok and cancel buttons
	Button cancelButton = createButton(CANCEL_BTN_URL, NUM_OF_SLOTS + 1, 150, 42, SCREEN_WIDTH - SPACE - 150, SCREEN_HEIGHT - 30 - 42);
	Button okButton = createButton(NEXT_BTN_URL, NUM_OF_SLOTS + 2, 150, 42, cancelButton.buttonsDestRect.x - 30 - cancelButton.buttonsDestRect.w, cancelButton.buttonsDestRect.y);
	btnLst[NUM_OF_SLOTS] = cancelButton;
	btnLst[NUM_OF_SLOTS + 1] = okButton;


	currentPage.btnList = btnLst;
	currentPage.btnListLen = NUM_OF_SLOTS + 2;
	currentPage.id = 3;

	addButtons(btnLst, NUM_OF_SLOTS + 2, containerPage.page);

	// set default

	sprintf(path, "%s%d%s", SLOT_SELECTED_BTN_URL, 1, btn);
	selectButton(path, btnLst[0]);
	userGuiSettings.savedSlot = 1;

	updateSurface(containerPage.page);

	return currentPage;
}

int handleEvents(){
	SDL_Event e;
	int quit = 0; // When user asks to quit or there was an error

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
					quit = handleButtonClicked(e);
				break;
			default:
				break;
			}
		}
	}

	return quit;
}

int handleButtonClicked(SDL_Event e){
	int id = currentPage.id;
	int quit = 0;
	switch (id)
	{
	case 1:
		quit = handleButtonClicked_mainMenu(e);
		break;
	case 2:
		quit = handleButtonClicked_selectionWindow(e);
		break;
	case 3:
		quit = handleButtonClicked_loadFromSlotWindow(e);
		break;
	case 4:
		quit = handleButtonClicked_aiSettingsWindow(e);
		break;
	}

	return quit;
}

int handleButtonClicked_mainMenu(SDL_Event e){
	Button *lst = currentPage.btnList;
	int len = currentPage.btnListLen;
	Button curr;
	int quit = 0;
	int isSuccess = 1;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			switch (curr.id)
			{
			case 1:
				// Go To selection window
				isSuccess = navigatToPage("selectionWindow");
				break;
			case 2:
				// Go to load game screen
				isSuccess = navigatToPage("loadSlotWindow");
				break;
			case 3:
				quit = 1;
				break;
			}
		}
	}

	if (!isSuccess){
		quit = 1;
	}
	
	return quit;
	
}

int handleButtonClicked_selectionWindow(SDL_Event e){
	Button *lst = currentPage.btnList;
	int len = currentPage.btnListLen;
	Button curr;
	Button prev;
	int quit = 0;
	int isSuccess = 1;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			switch (curr.id)
			{
			case 1:
				// user choose player vs. player
				userGuiSettings.gameMode = TWO_PLAYERS;
				prev = getButtonAccordingToId(lst, len, 2);
				isSuccess = toggleButtons(prev, curr, AGAINS_COMPUTER_BTN_URL, TWO_PLAYERS_SELECTED_BTN_URL);

				break;
			case 2:
				// user choose player vs. AI
				userGuiSettings.gameMode = PLAYER_VS_AI;
				prev = getButtonAccordingToId(lst, len, 1);
				isSuccess = toggleButtons(prev, curr, TWO_PLAYERS_BTN_URL, AGAINS_COMPUTER_SELECTED_BTN_URL);
				break;
			case 4:
				// user choose next player black
				userGuiSettings.isNextPlayerBlack = 1;
				prev = getButtonAccordingToId(lst, len, 3);
				isSuccess = toggleButtons(prev, curr, WHITE_BTN_URL, BLACK_SELECTED_BTN_URL);
				break;
			case 3:
				// user choose next player white
				userGuiSettings.isNextPlayerBlack = 0;
				prev = getButtonAccordingToId(lst, len, 4);
				isSuccess = toggleButtons(prev, curr, BLACK_BTN_URL, WHITE_SELECTED_BTN_URL);
				break;
			case 5:
				// user choose to set board
				userGuiSettings.isSetBoard = 1;
				prev = getButtonAccordingToId(lst, len, 6);
				isSuccess = toggleButtons(prev, curr, YES_BTN_URL, NO_SELECTED_BTN_URL);
				break;
			case 6:
				// user choose not to set board
				userGuiSettings.isSetBoard = 0;
				prev = getButtonAccordingToId(lst, len, 5);
				isSuccess = toggleButtons(prev, curr, NO_BTN_URL, YES_SELECTED_BTN_URL);
				break;
			case 7:
				// user choose to cancel and return to the main menu
				isSuccess = navigatToPage("mainMenu");
				break;
			case 8:
				// user choose to aplly settings and continue to next step
				saveSettings(1);
				if (userGuiSettings.gameMode == PLAYER_VS_AI){
					// navigate to AI settings window
					isSuccess = navigatToPage("aiSettingsWindow");
				}
				else if(userGuiSettings.isSetBoard){
					// navigate to set board window
				}
				else{
					// navigate to start game window
					quit = 2;
				}
				break;
			}
		}
	}

	quit = quit != 0 ? quit : !isSuccess;
	return quit;

}

int handleButtonClicked_loadFromSlotWindow(SDL_Event e){
	Button *lst = currentPage.btnList;
	int len = currentPage.btnListLen;
	Button curr;
	int quit = 0;
	int isSuccess = 1;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			if (curr.id >=1 && curr.id <=NUM_OF_SLOTS){
				// a slot was chosen - load from slot
				char path[100];
				if (userGuiSettings.savedSlot != 0){
					// need to deselect the prevButton
					Button prevChosenButton = getButtonAccordingToId(lst, len, userGuiSettings.savedSlot);
					sprintf(path, "%s%d%s", SLOT_BTN_URL, userGuiSettings.savedSlot, "_btn.bmp");
					isSuccess = deselectButton(path, prevChosenButton);
				}
				if (isSuccess == 1){
					userGuiSettings.savedSlot = curr.id;
					sprintf(path, "%s%d%s", SLOT_SELECTED_BTN_URL, userGuiSettings.savedSlot, "_btn.bmp");
					isSuccess = selectButton(path, curr);
				}
			}
			else{
				switch (curr.id)
				{
				case NUM_OF_SLOTS+1:
					isSuccess = navigatToPage("mainMenu");
					break;
				case NUM_OF_SLOTS+2:
					// load from chosen slot
					//check if there is no slot chosen
					if (userGuiSettings.savedSlot != 0){
						
						char path[100];
						char *slot = "./slots/slot";
						char *xmlStr = ".xml";
						sprintf(path, "%s%d%s", slot, userGuiSettings.savedSlot, xmlStr);
						fileData data = loadGame(path);
						isSuccess = saveLoadedData(data, 0);
						if (!isSuccess){
							// TBD - what do we need to do? Show a dialog?
						}

						// navigate to selection page
						isSuccess = navigatToPage("selectionWindow");
					}
					break;
				}
			}
		}
	}

	quit = !isSuccess;
	return quit;

}

int handleButtonClicked_aiSettingsWindow(SDL_Event e){
	Button *lst = currentPage.btnList;
	int len = currentPage.btnListLen;
	Button curr;
	Button prev;
	int quit = 0;
	int isSuccess = 1;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		char path[100];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			if (curr.id >= 1 && curr.id <= 5){
				// a difficulty was chosen
				int isSuccess = 1;
				if (userGuiSettings.difficulty != 0){
					// need to deselect the prevButton
					int difficulty = userGuiSettings.difficulty == BEST ? 5 : userGuiSettings.difficulty;
					Button prevChosenButton = getButtonAccordingToId(lst, len, difficulty);
					sprintf(path, "%s%d%s", SLOT_BTN_URL, difficulty, "_btn.bmp");
					isSuccess = deselectButton(path, prevChosenButton);
				}
				if (isSuccess == 1){
					userGuiSettings.difficulty = curr.id < 5 ? curr.id : BEST;
					sprintf(path,"%s%d%s", SLOT_SELECTED_BTN_URL, curr.id, "_btn.bmp");
					if (userGuiSettings.difficulty == BEST){
						isSuccess = selectButton(BEST_SELECTED_BTN_URL, curr);
					}
					else{
						isSuccess = selectButton(path, curr);
					}
				}
				quit = !isSuccess;
			}
			else{
				switch (curr.id){
				case 6:
					userGuiSettings.isUserColorBlack = 1;
					prev = getButtonAccordingToId(lst, len, 7);
					isSuccess = toggleButtons(prev, curr, WHITE_BTN_URL, BLACK_SELECTED_BTN_URL);
					break;
				case 7:
					userGuiSettings.isUserColorBlack = 0;
					prev = getButtonAccordingToId(lst, len, 6);
					isSuccess = toggleButtons(prev, curr, BLACK_BTN_URL, WHITE_SELECTED_BTN_URL);
					break;
				case 8:
					isSuccess = navigatToPage("mainMenu");
					break;
				case 9:
					saveSettings(0);
					if (userGuiSettings.isSetBoard == 1){
						// navigate to set board window
						quit = 3;
					}
					else{
						// navigate to start game window
						quit = 2;
					}
					break;
				}
			}
		}
	}

	quit = quit != 0 ? quit : !isSuccess;
	return quit;

}

int navigatToPage(char* pageName){
	int isSuccess = 1;
	isSuccess = removeCurrentPage();
	if (isSuccess == 1){
		if (strcmp(pageName, "mainMenu") == 0){
			currentPage = createMainMenuPage();
			if (currentPage.isError == 1){
				// ERROR
				isSuccess = 0;
			}
		}
		else if (strcmp(pageName, "selectionWindow") == 0){
			currentPage = createSelecetionPage();
			if (currentPage.isError == 1){
				// ERROR
				isSuccess = 0;
			}
		}
		else if (strcmp(pageName, "loadSlotWindow") == 0){
			currentPage = createLoadFromSlotPage();
			if (currentPage.isError == 1){
				// ERROR
				isSuccess = 0;
			}
		}
		else if (strcmp(pageName, "aiSettingsWindow") == 0){
			currentPage = createAiSettingsPage();
			if (currentPage.isError == 1){
				// ERROR
				isSuccess = 0;
			}
		}
	}

	return isSuccess;
}

int removeCurrentPage(){
	int isSuccess = 1;
	Button *lst = currentPage.btnList;
	int len = currentPage.btnListLen;

	for (int i = 0; i < len; i++){
		if (lst[i].img != NULL){
			my_sdl_free(lst[i].img);
			lst[i].img = NULL;
		}

		if (lst[i].selectedImg != NULL){
			my_sdl_free(lst[i].selectedImg);
			lst[i].selectedImg = NULL;
		}
	}

	currentPage.btnListLen = 0;

	myFree(lst);
	currentPage.btnList = NULL;

	my_sdl_free(currentPage.bkg);
	currentPage.bkg = NULL;

	// put white screen on top
	SDL_FillRect(containerPage.page, &containerPage.page->clip_rect, SDL_MapRGB(containerPage.page->format, 0xFF, 0xFF, 0xFF));
	isSuccess = updateSurface(containerPage.page);
	return isSuccess;
}

void saveSettings(int isSelectionWinsow){
	if (isSelectionWinsow){
		settings.gameMode = userGuiSettings.gameMode;
		game_board.isBlackTurn = userGuiSettings.isNextPlayerBlack;
	}
	else{
		settings.isUserBlack = userGuiSettings.isUserColorBlack;
		settings.minmax_depth = userGuiSettings.difficulty;
	}
}

void restorDefaultSettings(){
	userGuiSettings.gameMode = TWO_PLAYERS;
	game_board.isBlackTurn = 0;
	settings.isUserBlack = 0;
	settings.minmax_depth = 1;
}

int selectButton(char *url, Button button){
	SDL_FillRect(containerPage.page, &button.buttonsDestRect, SDL_MapRGB(containerPage.page->format, 0xFF, 0xFF, 0xFF));
	if (button.selectedImg == NULL){
		button.selectedImg = loadImage(url);
		if (button.selectedImg == NULL){
			return 0;
		}
	}
	int isSuccess = addImageToSurface(button.selectedImg, &button.buttonsOriginRect, containerPage.page, &button.buttonsDestRect);
	if (isSuccess == 1){
		updateSurface(containerPage.page);
	}
	
	return isSuccess;
}

int deselectButton(char *url, Button button){
	SDL_FillRect(containerPage.page, &button.buttonsDestRect, SDL_MapRGB(containerPage.page->format, 0xFF, 0xFF, 0xFF));
	if (button.img == NULL){
		button.img = loadImage(url);
		if (button.img == NULL){
			return 0;
		}
	}
	int isSuccess = addImageToSurface(button.img, &button.buttonsOriginRect, containerPage.page, &button.buttonsDestRect);
	if (isSuccess == 1){
		updateSurface(containerPage.page);
	}
	return isSuccess;
}

Button getButtonAccordingToId(Button list[], int len, int id){
	Button curr = { {0,0,0,0}, {0,0,0,0}, NULL, 0 };
	for (int i = 0; i < len; i++){
		curr = list[i]; 
		if (curr.id == id){
			return curr;
		}
	}

	return curr;
}

int toggleButtons(Button prev, Button curr, char *prevSkin, char *currSkin){
	int isSuccess = deselectButton(prevSkin, prev);
	if (isSuccess == 1){
		selectButton(currSkin, curr);
	}

	return isSuccess;
}



