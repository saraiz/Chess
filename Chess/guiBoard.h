#ifndef GUI_BOARD
#define GUI_BOARD
#define GUI_black 0
#define GUI_white 1
#define GUI_b 0
#define GUI_k 1
#define GUI_m 2
#define GUI_n 3
#define GUI_q 4
#define GUI_r 5

char colors[] = { 'b', 'w' };
char pice_types[] = { 'b', 'k', 'm', 'n', 'q', 'r' };

#include "guiUtils.h"
#include "utils.h"
#include "ChessLogic.h"
SDL_Surface* picess [6][2][2]; // [b/k/m/n/q/r][pice color b/w][bkg color b/w]
SDL_Surface* emptys[2];

int load_all_pices();
void haim_main();
int createPlayPage(SDL_Surface* surface);
SDL_Surface* getPiceImage(int x, int y);
int createBoard(SDL_Surface* surface);

#endif
