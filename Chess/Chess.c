#include "Chess.h"

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

int main()
{
	init_board(game_board.board);
	
	setSettings("clear");
	setSettings("game_mode 1");
	//setSettings("next_player black");
	setSettings("set <a,1> white king");
	setSettings("set <b,1> black rook");
	setSettings("set <h,1> black king");
	setSettings("set <b,3> black queen");

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
	int extern countAlloc;//TODO delete
	printf("%d", countAlloc);//TODO delete
	getchar(); //TODO delete
	return 1;
}





