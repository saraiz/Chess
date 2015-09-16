#ifndef GUI_BOARD
#define GUI_BOARD

#include "guiUtils.h"
#include "utils.h"
#include "ChessLogic.h"
#include "gameStateManager.h"

#define GUI_black 0
#define GUI_white 1
#define GUI_b 0
#define GUI_k 1
#define GUI_m 2
#define GUI_n 3
#define GUI_q 4
#define GUI_r 5



typedef struct {
	int pageID; //-1- quit, 0- user need to chose what to move, 1- user need to chose where to move, 
	locationNode origin;
	SDL_Surface* picess[6][2][2][2]; // [b/k/m/n/q/r][pice color b/w][bkg color b/w][isColored]
	SDL_Surface* emptys[2][2]; //[bkg color b/w][isColored]
	SDL_Surface *surface;
}GuiBoardData;

GuiBoardData GuiBData;

int load_all_pices();
void GuiBoardStart();
SDL_Surface* getPiceImage(int x, int y, int isColored);
int createBoard();
int handleBoardEvents();
int handleBoardButtonClicked(SDL_Event e);
int colorSquers(moveList* move, locationNode origin);
int colorASquere(locationNode loc);
int eventHendelPage1(SDL_Event e);
int eventHendelPage0(SDL_Event e);
int free_all_pices();

int pageID1();
int pageID0();

#endif
