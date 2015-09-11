#include "GuiSettingsStateManager.h"

int buildSettingsWindow(){
	navigatToPage("mainMenu");
	handleMainMenuEvents();
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
	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Surface *surface = createSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	Page emptyPage;

	if (surface == NULL){
		emptyPage.isError = 1;
		return emptyPage;
	}

	navigator.mainMenu.pageRect = rect;
	navigator.mainMenu.page = surface;

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

	addButtons(btnLst, 3, surface);

	return navigator.mainMenu;
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
		isSuceess = addImageToSurface(list[i].img, list[i].buttonsOriginRect, surface, list[i].buttonsDestRect);
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

int handleMainMenuEvents(){
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
				quit = handleButtonClicked_mainMenu(e);
				break;
			default:
				break;
			}
		}
	}
}

int handleButtonClicked_mainMenu(SDL_Event e){
	Button *lst = navigator.mainMenu.btnList;
	int len = navigator.mainMenu.btnListLen;
	Button curr;
	int quit = 0;

	for (int i = 0; i < len; i++){
		curr = lst[i];
		if ((e.button.x > curr.buttonsDestRect.x) && 
			(e.button.x < curr.buttonsDestRect.x + curr.buttonsDestRect.w) && 
			(e.button.y > curr.buttonsDestRect.y) && 
			(e.button.y < curr.buttonsDestRect.y + curr.buttonsDestRect.h)){
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

int navigatToPage(char* pageName){
	removeCurrentPage();
	if (strcmp(pageName, "mainMenu") == 0){
		navigator.currentPage = createMainMenuPage();
		if (navigator.currentPage.isError == 1){
			// ERROR
		}
	}
	else if (strcmp(pageName, "selectionWindow") == 0){

	}
	else if (strcmp(pageName, "loadSlotWindow") == 0){

	}
}

int removeCurrentPage(){
	Button *lst = navigator.currentPage.btnList;
	int len = navigator.currentPage.btnListLen;

	for (int i = 0; i < len; i++){
		SDL_FreeSurface(lst[i].img);
	}

	SDL_FreeSurface(navigator.currentPage.page);
}



