#ifndef GUI_SETTINGS
#define GUI_SETTINGS

#include "guiUtils.h"

#define SPACE 45
#define NUM_OF_SLOTS 7

typedef struct Page{
	SDL_Rect pageRect;
	SDL_Surface *page;
	Button *btnList;
	SDL_Surface *bkg;
	int btnListLen;
	int isError;
	int id;
}Page;


typedef struct UserGuiSettings{
	int gameMode;
	int isNextPlayerBlack;
	int isSetBoard;
}UserGuiSettings;

Page currentPage;
Page containerPage;
UserGuiSettings userGuiSettings;

void createMainContainer();
Page createMainMenuPage();
Page createSelecetionPage();
Page createLoadFromSlotPage();
Page createAiSettingsPage();
int buildSettingsWindow();
int handleEvents();
int handleButtonClicked(SDL_Event e);
int handleButtonClicked_mainMenu(SDL_Event e);
int handleButtonClicked_selectionWindow(SDL_Event e);
int navigatToPage(char* pageName);
int removeCurrentPage();

#endif
