#include "Chess.h"

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };


int main()
{
	init_board(game_board.board);
	print_board(game_board.board);

	//setSettings("clear\nset <a,3> black queen");

	actionSummery summery = readSettings();
	if (summery.isError){
		//print_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			summery = readGameActions();
			if (summery.isError == 1){
				//print_message(summery.failedFunc);
			}

		}
	}


	getchar();
	return 1;
}

int isLocationValid(locationNode node, int isPrintMessage){
	int isValid = 1;
	if ((node.row < 0) || (node.row > 9) || (node.column < 0) || (node.column > 9)){
		isValid = 0;
	}

	if (isValid == 0 && isPrintMessage == 1){
		print_message(WRONG_POSITION);
	}

	return isValid;
}

int isBoardValidAfterSet(char *type, char *color, int isShowMessage){
	int isValid = 1;

	if (strcmp(color, "black") == 0){

		if ((type == BISHOP && game_board.numOfBlackBishops == 2) ||
			(type == KING && game_board.numOfBlackKings == 1) ||
			(type == KNIGHT && game_board.numOfBlackKnights == 2) ||
			(type == PAWN && game_board.numOfBlackPawns == 8) ||
			(type == QUEEN && game_board.numOfBlackQueens == 1) ||
			(type == ROOK && game_board.numOfBlackRooks == 2)){
			isValid = 0;
		}
	}
	else{
		if ((type == BISHOP && game_board.numOfWhiteBishops == 2) ||
			(type == KING && game_board.numOfWhiteKings == 1) ||
			(type == KNIGHT && game_board.numOfWhiteKnights == 2) ||
			(type == PAWN && game_board.numOfWhitePawns == 8) ||
			(type == QUEEN && game_board.numOfWhiteQueens == 1) ||
			(type == ROOK && game_board.numOfWhiteRooks == 2)){
			isValid = 0;
		}
	}

	if (isValid == 0 && isShowMessage == 1){
		print_message(NO_PIECE);
	}

	return isValid;
}

int isBoardValidToStartGame(int isShowMessage){
	if ((game_board.numOfBlackKings < 1) || game_board.numOfWhiteKings < 1){
		if (isShowMessage == 1){
			print_message(WROND_BOARD_INITIALIZATION);
		}

		return 0;
	}

	return 1;
}





