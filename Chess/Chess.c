#include "Chess.h"
#include <SDL.h>
#include <SDL_video.h>


#define WIN_W 800
#define WIN_H 600
#define IMG_W 240
#define IMG_H 296


gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

int main(int argc, char* argv[])
{

	//SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_Quit();

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
			handleGUI();
		}
	}
	return 1;
}

void handleConsole(){

	/*setSettings("clear");
	setSettings("game_mode 1");
	setSettings("next_player white");
	setSettings("set <b,8> black king");
	setSettings("set <e,7> white queen");
	setSettings("set <d,6> white knight");
	setSettings("set <b,6> white rook");
	setSettings("set <h,1> white king");*/

	// Don't delete me. We need to print the board in the begining og the game according to the instructions. 
	print_board(game_board.board);

	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			//printAllPossibleMoves( getAllValidMoves(1,0)); //TODO delete
			//print_board(game_board.board); //TODO delete
			summery = readGameActions();
			if (summery.isError){
				print_error_message(summery.failedFunc);
			}
			else{
				// game ended because: someone won/it's a tie/quit
				// Do we need to do something? 
			}

		}
	}
	//int extern countAlloc;//TODO delete
	//printf("%d", countAlloc);//TODO delete
	getchar(); //TODO delete
}


int handleGUI(){
	/*setSettings("clear");
	setSettings("game_mode 1");
	setSettings("next_player white");
	setSettings("set <a,1> black king");
	setSettings("set <a,2> black pawn");
	setSettings("set <b,1> black pawn");
	setSettings("set <b,2> black pawn");
	setSettings("set <e,7> white queen");
	setSettings("set <d,6> white knight");
	//setSettings("set <b,6> white knight");
	setSettings("set <h,1> white king");
	*/

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	GuiBoardStart(); //TODO delete
	//buildSettingsWindow();
	
	
}
