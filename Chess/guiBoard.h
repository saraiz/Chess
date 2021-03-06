#ifndef GUI_BOARD
#define GUI_BOARD

#include "guiUtils.h"
#include "GuiSettingsStateManager.h"
#include "ChessLogic.h"
#include "gameStateManager.h"
#include "IO.h"
#include "settingsStateManager.h"

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
	3- PP promotion
	4- PP best moves
	5- PP save

	6- set: select place
	7- set: PP select pice
	8- set: PP error
	*/

	moveList moveToDo;
	int main_quit;
	int set_quit;
	int pull_quit;
	locationNode wasClicked;
	char toSet;
	int set_which_error_to_print; //0 after set, 1 after start
	int minmaxDepth; //-2=cancel, else = depth

	SDL_Surface *surface;
	SDL_Surface* picess[6][2][2][2]; // [b/k/m/n/q/r][pice color b/w][bkg color b/w][isColored]
	SDL_Surface* emptys[2][2]; //[bkg color b/w][isColored]
	SDL_Surface* sideBar[6]; //check, tie, mate b, mate w, empty, comp turn
	SDL_Surface* set_popup;
	SDL_Surface* set_error[2]; // error0, error1
	SDL_Surface* getMove[2]; //popup, banner

	Button boardBtn[4]; // main menu, get best move, save game, quit
	Button set_side_btn[3]; //next,clear, cancel
	Button set_popup_pices[6][2]; //[b/k/m/n/q/r][pice color b/w]
	Button set_popup_btn[2]; //cancel, remove
	Button set_ok;
	Button Diff_btn[6]; //cancel, 1,2,3,4,best

}GuiBoardData;

void GuiBoardStart(int isSetBoard);
SDL_Surface* getPiceImage(int x, int y, int isColored);
int Mate_Tie_Check();
int isPromotion(moveList moveToDo);
int do_usr_move();
int startGame();
char get_pice_char_from_set_btn_id(int btnID);
void rmAll();
void clear_My_screen();
int startSet();



int load_all_pices();
void free_all_pices();

int createButtens();
int createButtens();
int createBoard();
int colorSquers(moveList* move, locationNode origin);
int colorASquere(locationNode loc);
int print_side_bar(int Mate_Tie_Check);
int print_comp_turn(int is_comp_turn);
int create_set_side();
int create_set_popup();
int print_set_popup();
int load_set_popup();
int print_please_wait();
void create_best_move_pp_btn();
int printDiffPP();


int handleBoardEvents();
int handleBoardButtonClicked(SDL_Event e);
int eventHendelPage0(SDL_Event e);
int eventHendelPage1(SDL_Event e);
int eventHendelPage3(SDL_Event e);
int eventHendelPage4(SDL_Event e);
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
int pageID6();
int pageID7();
int pageID8();
int pageIDMinus1();

#endif
