#ifndef GUI_SETTINGS
#define GUI_SETTINGS

#include "guiUtils.h"

#define SPACE 45

typedef struct Page{
	SDL_Rect pageRect;
	SDL_Surface *page;
	Button *btnList;
	SDL_Surface *bkg;
	int btnListLen;
	int isError;
	int id;
}Page;

typedef struct PagesNavigator{
	Page mainMenu;
	Page playersSelecionWindow;
	Page aiSettingsWindow;
	Page currentPage;
}PagesNavigator;

typedef struct UserGuiSettings{
	int gameMode;
	int isNextPlayerBlack;
	int isSetBoard;
}UserGuiSettings;


Page containerPage;
PagesNavigator navigator;
UserGuiSettings userGuiSettings;

void createMainContainer();
Page createMainMenuPage();
Page createSelecetionPage();
Button createButton(char *imgUrl, int id, int width, int height, int x, int y);
int addButtons(Button list[], int len, SDL_Surface *surface);
int buildSettingsWindow();
int handleEvents();
int handleButtonClicked(SDL_Event e);
int handleButtonClicked_mainMenu(SDL_Event e);
int handleButtonClicked_selectionWindow(SDL_Event e);
int navigatToPage(char* pageName);
int removeCurrentPage();

#endif
