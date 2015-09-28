#ifndef GUI_BOARD
#define GUI_BOARD

#include "guiUtils.h"
#include "ChessLogic.h"
#include "gameStateManager.h"
#include "IO.h"

#define GUI_black 0
#define GUI_white 1
#define GUI_b 0
#define GUI_k 1
#define GUI_m 2
#define GUI_n 3
#define GUI_q 4
#define GUI_r 5

#define MATE1 1
#define CHECK1 2
#define TIE1 3



typedef struct {
	int pageID; 
	/*
	-1- quit,
	0- user need to chose what to move,
	1- user need to chose where to move,
	2- computer turn
	3- promotion
	4- -best moves
	5- save

	6- set: select placr
	7- set: select place
	8- set: error
	*/

	moveList moveToDo;
	int main_quit;
	int set_quit;

	SDL_Surface *surface;
	SDL_Surface* picess[6][2][2][2]; // [b/k/m/n/q/r][pice color b/w][bkg color b/w][isColored]
	SDL_Surface* emptys[2][2]; //[bkg color b/w][isColored]
	Button boardBtn[4]; // main menu, get best move, save game, quit
	SDL_Surface* sideBar[6]; //check, tie, mate b, mate w, empty, comp turn

	Button set_side_btn[2]; //next, cancel
	Button set_popup_pices[6][2]; //[b/k/m/n/q/r][pice color b/w]
	Button set_popup_btn[2]; //cancel, remove
	SDL_Surface* set_popup;

}GuiBoardData;

void GuiBoardStart(int isSetBoard);
SDL_Surface* getPiceImage(int x, int y, int isColored);
int Mate_Tie_Check();
int isPromotion(moveList moveToDo);
int do_usr_move();
int StartGame();

int load_all_pices();
void free_all_pices();

int createButtens();
int createButtens();
int colorSquers(moveList* move, locationNode origin);
int colorASquere(locationNode loc);
int print_side_bar(int Mate_Tie_Check);
int print_comp_turn(int is_comp_turn);
int create_set_side();
int create_set_popup();
int print_set_popup();

int handleBoardEvents();
int handleBoardButtonClicked(SDL_Event e);
int eventHendelPage0(SDL_Event e);
int eventHendelPage1(SDL_Event e);
int eventHendelPage3(SDL_Event e);
int eventHendelPage5(SDL_Event e);
int eventHendelPage6(SDL_Event e);
int eventHendelPage7(SDL_Event e);
int eventHendelPage8(SDL_Event e);
int handleSetButtonClicked(SDL_Event e);


int pageID2();
int pageID1();
int pageID0();
int pageID3();
int pageID4();
int pageID5();
int pageIDMinus1();

#endif
