#ifndef GUI_SETTINGS
#define GUI_SETTINGS

#include "guiUtils.h"
#include "IO.h"
#include "settingsStateManager.h"

#define SPACE 45
#define NUM_OF_SLOTS 7

#define MAIN_MENU_BKG "./images/settings/bkg/mainMenu_bkg.bmp"
#define SELECTION_WINDOW_BKG "./images/settings/bkg/selectionWindow_bkg.bmp"
#define AI_SETTINGS_WINDOW "./images/settings/bkg/AiSettingsWindow_bkg.bmp"
#define LOAD_FROM_SLOT_WINDOW "./images/settings/bkg/loadFromSlot_bkg.bmp"
#define REG_BTN_URL "./images/settings/btn/button.bmp"
#define REG_BTN_SELECTED_URL "./images/settings/btn/button_selected.bmp"
#define SLOT_BTN_URL "./images/settings/btn/slot.bmp"
#define SLOT_BTN_SELECTED_URL "./images/settings/btn/slot_selected.bmp"

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
int handleButtonClicked_mainMenu(SDL_Event e);
int handleButtonClicked_selectionWindow(SDL_Event e);
int navigatToPage(char* pageName);
int removeCurrentPage();
void saveSettings(int isSelectionWinsow);
int selectButton(char *url, Button button);
int deselectButton(char *url, Button button);
Button getButtonAccordingToId(Button list[], int len, int id);
int handleButtonClicked_aiSettingsWindow(SDL_Event e);

#endif
