#include "GuiSettingsStateManager.h"

int buildSettingsWindow(){
	createMainContainer();
	navigatToPage("mainMenu");
	handleEvents();
}

void createMainContainer(){
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Surface *surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	containerPage.pageRect = rect;
	containerPage.page = surface;

	if (SDL_FillRect(containerPage.page, &containerPage.pageRect, SDL_MapRGB(containerPage.page->format, 0, 0, 0)) != 0) {
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
	}
}

Page createMainMenuPage(){
	Page emptyPage;

	//Add Background
	SDL_Surface *bkg = loadImage("./images/settings/bkg/mainMenu_bkg.bmp");
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	navigator.mainMenu.bkg = bkg;

	// CreateButtons
	Button newGameBtn = createButton("./images/settings/btn/button.bmp", 1, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, 200);
	Button loadGameBtn = createButton("./images/settings/btn/button.bmp", 2, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, newGameBtn.buttonsDestRect.y + newGameBtn.buttonsDestRect.h + SPACE);
	Button quitGameBtn = createButton("./images/settings/btn/button.bmp", 3, 150, 42, SCREEN_WIDTH / 2 - 150 / 2, loadGameBtn.buttonsDestRect.y + loadGameBtn.buttonsDestRect.h + SPACE);
	Button *btnLst = (Button*)malloc(sizeof(Button)*3);
	if (btnLst == NULL){
		emptyPage.isError = 1;
		return emptyPage;
	}

	btnLst[0] = newGameBtn;
	btnLst[1] = loadGameBtn;
	btnLst[2] = quitGameBtn;

	navigator.mainMenu.btnList = btnLst;
	navigator.mainMenu.btnListLen = 3;
	navigator.mainMenu.id = 1;

	addButtons(btnLst, 3, containerPage.page);

	return navigator.mainMenu;
}

Page createSelecetionPage(){
	Page emptyPage;

	//Add Background
	SDL_Surface *bkg = loadImage("./images/settings/bkg/selectionWindow_bkg.bmp");
	addImageToSurface(bkg, NULL, containerPage.page, NULL);
	navigator.playersSelecionWindow.bkg = bkg;

	// create game mode section
	Button game_mode_2PlayersMode = createButton("./images/settings/btn/button.bmp", 1, 150, 42, 30, 110);
	Button game_mode_playerVsAI = createButton("./images/settings/btn/button.bmp", 2, 150, 42, game_mode_2PlayersMode.buttonsDestRect.x + game_mode_2PlayersMode.buttonsDestRect.w + SPACE, 110);

	// create next player section
	Button next_player_black = createButton("./images/settings/btn/button.bmp", 3, 150, 42, 30, 220);
	Button next_player_white = createButton("./images/settings/btn/button.bmp", 4, 150, 42, next_player_black.buttonsDestRect.x + next_player_black.buttonsDestRect.w + SPACE, 220);

	// create next player section
	Button set_board_yes = createButton("./images/settings/btn/button.bmp", 5, 150, 42, 30, 330);
	Button set_board_no = createButton("./images/settings/btn/button.bmp", 6, 150, 42, set_board_yes.buttonsDestRect.x + set_board_yes.buttonsDestRect.w + SPACE, 330);

	// create ok and cancel buttons
	Button cancelButton = createButton("./images/settings/btn/button.bmp", 7, 150, 42, SCREEN_WIDTH - SPACE - 150, SCREEN_HEIGHT - 30 - 42);
	Button okButton = createButton("./images/settings/btn/button.bmp", 8, 150, 42, cancelButton.buttonsDestRect.x - 30 - cancelButton.buttonsDestRect.w, cancelButton.buttonsDestRect.y);
	
	Button *btnLst = (Button*)malloc(sizeof(Button) * 8);
	if (btnLst == NULL){
		emptyPage.isError = 1;
		return emptyPage;
	}

	btnLst[0] = game_mode_2PlayersMode;
	btnLst[1] = game_mode_playerVsAI;
	btnLst[2] = next_player_black;
	btnLst[3] = next_player_white;
	btnLst[4] = set_board_yes;
	btnLst[5] = set_board_no;
	btnLst[6] = cancelButton;
	btnLst[7] = okButton;

	navigator.playersSelecionWindow.btnList = btnLst;
	navigator.playersSelecionWindow.btnListLen = 8;
	navigator.playersSelecionWindow.id = 2;

	addButtons(btnLst, 8, containerPage.page);
	updateSurface(containerPage.page);

	return navigator.playersSelecionWindow;
}

Button createButton(char *imgUrl, int id, int width, int height, int x, int y){
	Button newGameBtn = { {0,0,0,0}, NULL };
	newGameBtn.id = id;
	SDL_Rect newGameBtnDestRect;
	newGameBtnDestRect.x = x;
	newGameBtnDestRect.y = y;
	newGameBtnDestRect.w = width;
	newGameBtnDestRect.h = height;

	SDL_Rect newGameBtnOriginRect = { 0, 0, 0, 0 };
	newGameBtnOriginRect.w = width;
	newGameBtnOriginRect.h = height;
	//newGameBtnOriginRect.x = 2;
	//newGameBtnOriginRect.y = 2;

	newGameBtn.img = loadImage(imgUrl);
	if (newGameBtn.img == NULL){
		return newGameBtn;
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

	// TBD - if there is an error I need to free al the buttons before
	return isSuceess;
}

int handleEvents(){
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
				quit = handleButtonClicked(e);
				break;
			default:
				break;
			}
		}
	}
}

int handleButtonClicked(SDL_Event e){
	int id = navigator.currentPage.id;
	int quit = 0;
	switch (id)
	{
	case 1:
		quit = handleButtonClicked_mainMenu(e);
		break;
	case 2:
		quit = handleButtonClicked_selectionWindow(e);
		break;
	}

	return quit;
}

int handleButtonClicked_mainMenu(SDL_Event e){
	Button *lst = navigator.mainMenu.btnList;
	int len = navigator.mainMenu.btnListLen;
	Button curr;
	int quit = 0;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			switch (curr.id)
			{
			case 1:
				// Go To selection window
				navigatToPage("selectionWindow");
				break;
			case 2:
				// Go to load game screen
				navigatToPage("loadSlotWindow");
				break;
			case 3:
				quit = 1;
				break;
			}
		}
	}
	return quit;
	
}

int handleButtonClicked_selectionWindow(SDL_Event e){
	Button *lst = navigator.playersSelecionWindow.btnList;
	int len = navigator.playersSelecionWindow.btnListLen;
	Button curr;
	int quit = 0;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if (isClickInRect(e, curr.buttonsDestRect) == 1){
			switch (curr.id)
			{
			case 1:
				// user choose player vs. player
				userGuiSettings.gameMode = TWO_PLAYERS;
				break;
			case 2:
				// user choose player vs. AI
				userGuiSettings.gameMode = PLAYER_VS_AI;
				break;
			case 3:
				// user choose next player black
				userGuiSettings.isNextPlayerBlack = 1;
				break;
			case 4:
				// user choose next player white
				userGuiSettings.isNextPlayerBlack = 0;
				break;
			case 5:
				// user choose to set board
				userGuiSettings.isSetBoard = 1;
				break;
			case 6:
				// user choose not to set board
				userGuiSettings.isSetBoard = 0;
				break;
			case 7:
				// user choose to cancel and return to the main menu
				navigatToPage("mainMenu");
				break;
			case 8:
				// user choose to aplly settings and continue to next step
				saveSettings();
				if (userGuiSettings.gameMode == PLAYER_VS_AI){
					// navigate to AI settings window
				}
				else if(userGuiSettings.isSetBoard){
					// navigate to set board window
				}
				else{
					// navigate to start game window
				}
				break;
			}
		}
	}
	return quit;

}

int navigatToPage(char* pageName){
	removeCurrentPage();
	if (strcmp(pageName, "mainMenu") == 0){
		navigator.currentPage = createMainMenuPage();
		if (navigator.currentPage.isError == 1){
			// ERROR
		}
	}
	else if (strcmp(pageName, "selectionWindow") == 0){
		navigator.currentPage = createSelecetionPage();
		if (navigator.currentPage.isError == 1){
			// ERROR
		}
	}
	else if (strcmp(pageName, "loadSlotWindow") == 0){

	}
}

int removeCurrentPage(){
	Button *lst = navigator.currentPage.btnList;
	int len = navigator.currentPage.btnListLen;

	for (int i = 0; i < len; i++){
		SDL_FreeSurface(lst[i].img);
		lst[i].buttonsOriginRect.h = 0;
		lst[i].buttonsOriginRect.w = 0;
	}

	myFree(lst);

	SDL_FreeSurface(navigator.currentPage.bkg);

	// put white screen on top
	SDL_FillRect(containerPage.page, &containerPage.page->clip_rect, SDL_MapRGB(containerPage.page->format, 0xFF, 0xFF, 0xFF));
	updateSurface(containerPage.page);
}

void saveSettings(){
	settings.gameMode = userGuiSettings.gameMode;
	game_board.isBlackTurn = userGuiSettings.isNextPlayerBlack;
}


