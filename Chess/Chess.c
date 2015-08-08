#include "Chess.h"
#include "ChessLogic.h"

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };


int main()
{
	init_board(game_board.board);
	print_board(game_board.board);

	//setSettings("clear\nset <d,4> white bishop"); //TODO delete
	//setSettings("set <b,6> white queen");
	//setSettings("set <f,6> black queen");

	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			//printMoveList( getValidMovesForLocation(createLocationNode(3,3)));
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


	getchar();
	return 1;
}





