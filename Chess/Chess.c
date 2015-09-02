#include "Chess.h"

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

int main()
{
	init_board(game_board.board);
	/*
	setSettings("clear\n"); //TODO delete
	setSettings("set <b,1> white pawn");
	//setSettings("set <b,2> black queen");
	//setSettings("set <a,2> white knight");
	//setSettings("game_mode 2");
	setSettings("set <g,1> white king");
	setSettings("set <a,8> black king");
	print_board(game_board.board);*/



	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			printAllPossibleMoves( getAllValidMoves(1,0)); //TODO delete
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

	getchar(); //TODO delete
	return 1;
}





