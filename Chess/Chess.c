#include "Chess.h"


#define WIN_W 800
#define WIN_H 600
#define IMG_W 240
#define IMG_H 296


gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

extern int countAlloc;
extern int countSurface;

int main(int argc, char* argv[])
{

	// If you run your code in nova you should put this lines in comment
	argc = 2;
	argv[1] = "gui";
	////////////////////////////////////////////////////////////////////

	init_board(game_board.board);

	if (argc == 2){
		char* mode = argv[1];
		if (strcmp(mode, "console") == 0){
			handleConsole();
		}
		else if (strcmp(mode, "gui") == 0){
			settings.isGUI = 1; // TBD - check if we need this
			handleGUI();
		}
	}
	else if (argc == 1){
		handleConsole();
	}
	return 1;
}

void handleConsole(){
	// Don't delete me. We need to print the board in the begining og the game according to the instructions. 
	print_board(game_board.board);

	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			summery = readGameActions();
			if (summery.isError){
				print_error_message(summery.failedFunc);
			}
		}
	}

	int extern countAlloc;//TODO delete
	printf("%d", countAlloc);//TODO delete
	//getchar(); //TODO delete
}


void handleGUI(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}
	atexit(SDL_Quit);

	buildSettingsWindow();

	printf("surface %d", countSurface);//TODO delete
	//getchar();	
}
