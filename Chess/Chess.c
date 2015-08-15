#include "Chess.h"
#include "ChessLogic.h"
#include "IO.h"

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };


void NOTmain(){ //TODO delete
	init_board(game_board.board);
	print_board(game_board.board);
	fileData tosave = { 1, 1, 1, 1 };
	saveGame(tosave, 2);
	getchar();
}


int main()
{
	init_board(game_board.board);
	print_board(game_board.board);

	setSettings("clear\nset <a,1> white pawn"); //TODO delete
	setSettings("set <b,2> black pawn");
	setSettings("game_mode 2");
	//setSettings("set <c,6> black rook");

	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			//printMoveList( getValidMovesForLocation(createLocationNode(3,3))); //TODO delete
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


	getchar(); //TODO delete
	return 1;
}





