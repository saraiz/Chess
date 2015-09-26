#ifndef GUI_SETTINGS
#define GUI_SETTINGS

#include "guiUtils.h"
#include "IO.h"
#include "settingsStateManager.h"
#include "guiBoard.h"

#define SPACE 45
#define NUM_OF_SLOTS 7

#define MAIN_MENU_BKG "./images/settings/bkg/mainMenu6_bkg.bmp"
#define SELECTION_WINDOW_BKG "./images/settings/bkg/selectionWindow3_bkg.bmp"
#define AI_SETTINGS_WINDOW "./images/settings/bkg/AiSettingsWindow2_bkg.bmp"
#define LOAD_FROM_SLOT_WINDOW "./images/settings/bkg/loadFromSlot2_bkg.bmp"

#define NEW_GAME_BTN_URL "./images/settings/btn/newGame_btn.bmp"
#define LOAD_GAME_BTN_URL "./images/settings/btn/loadGame_btn.bmp"
#define QUIT_GAME_BTN_URL "./images/settings/btn/quitGame_btn.bmp"

#define TWO_PLAYERS_BTN_URL "./images/settings/btn/2Players_btn.bmp"
#define TWO_PLAYERS_SELECTED_BTN_URL "./images/settings/btn/2Players_selected_btn.bmp"

#define AGAINS_COMPUTER_BTN_URL "./images/settings/btn/againtsComputer_btn.bmp"
#define AGAINS_COMPUTER_SELECTED_BTN_URL "./images/settings/btn/againtsComputer_selected_btn.bmp"

#define BLACK_BTN_URL "./images/settings/btn/Black_btn.bmp"
#define BLACK_SELECTED_BTN_URL "./images/settings/btn/Black_selected_btn.bmp"

#define WHITE_BTN_URL "./images/settings/btn/White_btn.bmp"
#define WHITE_SELECTED_BTN_URL "./images/settings/btn/White_selected_btn.bmp"

#define YES_BTN_URL "./images/settings/btn/yes_btn.bmp"
#define YES_SELECTED_BTN_URL "./images/settings/btn/yes_selected_btn.bmp"

#define NO_BTN_URL "./images/settings/btn/no_btn.bmp"
#define NO_SELECTED_BTN_URL "./images/settings/btn/no_selected_btn.bmp"

#define NEXT_BTN_URL "./images/settings/btn/next_btn.bmp"
#define NEXT_SELECTED_BTN_URL "./images/settings/btn/next_selected_btn.bmp"

#define CANCEL_BTN_URL "./images/settings/btn/cancel_btn.bmp"
#define CANCEL_SELECTED_BTN_URL "./images/settings/btn/cancel_selected_btn.bmp"

#define BEST_BTN_URL "./images/settings/btn/best_btn.bmp"
#define BEST_SELECTED_BTN_URL "./images/settings/btn/best_selected_btn.bmp"

#define SLOT_BTN_URL "./images/settings/btn/slot"
#define SLOT_SELECTED_BTN_URL "./images/settings/btn/slotSelected"

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
	int savedSlot;
	int difficulty;
	int isUserColorBlack;
}UserGuiSettings;

Page currentPage;
Page containerPage;
extern UserGuiSettings userGuiSettings;// = { 1, 0, 0, 0 };

int createMainContainer();
Page createMainMenuPage();
Page createSelecetionPage();
Page createLoadFromSlotPage();
Page createAiSettingsPage();
int buildSettingsWindow();
int handleEvents();
int handleButtonClicked(SDL_Event e);
int navigatToPage(char* pageName);
int removeCurrentPage();
void saveSettings(int isSelectionWinsow);
int selectButton(char *url, Button button);
int deselectButton(char *url, Button button);
Button getButtonAccordingToId(Button list[], int len, int id);
int handleButtonClicked_aiSettingsWindow(SDL_Event e);
int handleButtonClicked_mainMenu(SDL_Event e);
int handleButtonClicked_selectionWindow(SDL_Event e);

int handleButtonClicked_loadFromSlotWindow(SDL_Event e);
int toggleButtons(Button prev, Button curr, char *prevSkin, char *currSkin);

#endif
