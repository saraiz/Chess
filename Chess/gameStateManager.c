#include "gameStateManager.h"

actionSummery readGameActions(){

	char* input = (char*)myCalloc(sizeof(char), 50);
	// need to show a message if there was an error

	int isWin = 0;
	int isTie = 0;
	actionSummery summery = { 0, 0, 0, 0 };

	while (strcmp(input, "quit") != 0 && isWin == 0 && isTie == 0){
		char *msg = (game_board.isBlackTurn ? "black player - enter your move!\n" : "white player - enter your move!\n");
		print_message(msg);

		if ((settings.gameMode == PLAYER_VS_AI && 
			((game_board.isBlackTurn == 1 && settings.isUserBlack == 1) ||
			(game_board.isBlackTurn == 0 && settings.isUserBlack == 0))) || 
			(settings.gameMode == TWO_PLAYERS)){
			
			getInput(&input);
			if (strcmp(input, "quit") == 0){
				break;
			}

			summery = executeGameActions(input);
			if (summery.isExecuted == 1){
				//isWin = checkWin(game_board.isBlackTurn, 1);
				// now it's the computer turn
				game_board.isBlackTurn = 1 - game_board.isBlackTurn;
			}
		}
		else{
			// it's the computer move
			//computerTurn();
			print_board(game_board.board);
			//isWin = checkWin(game_board.isBlackTurn, 1);
			game_board.isBlackTurn = 1 - game_board.isBlackTurn;
		}
	}

	// free the input
	myFree(input);

	return summery;
}

actionSummery executeGameActions(char* input){
	actionSummery summery = checkForMove(input);
	if (summery.isFound == 0){
		summery = checkForGetMoves(input);
		if (summery.isFound == 0){
			summery = checkForBestMoves(input);
			if (summery.isFound == 0){
				summery = checkForGetScore(input);
				if (summery.isFound == 0){
					summery = checkForSave(input);
				}
			}
		}
	}

	if (summery.isFound == 0){
		print_message(ILLEGAL_COMMAND);
	}

	return summery;

}

actionSummery checkForGetMoves(char *input){
	char *loc = strstr(input, "get_moves");
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound == 1;
		loc = loc + 9;
		loc = getNextChar(loc);
		locationNode origin = getNextLocation(&loc);
		if (origin.row == -1 && origin.column == -1){
			summery.isError = 1;
			strcmp(summery.failedFunc, "calloc");

			return summery;
		}


		// Call Haim function
		// print all moves

	}

	return summery;
}

actionSummery checkForBestMoves(char *input){
	char *loc = strstr(input, "get_best_moves");
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound == 1;
		loc = loc + 14;
		loc = getNextChar(loc);
		double value = strtod(loc, NULL);

		// minmax??

	}

	return summery;
}

actionSummery checkForGetScore(char *input){
	char *loc = strstr(input, "get_score");
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound == 1;
		loc = loc + 9;
		loc = getNextChar(loc);

		// need to continue

	}

	return summery;
}


actionSummery checkForSave(char *input){
	char *loc = strstr(input, "save");
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound == 1;
		loc = loc + 4;
		loc = getNextChar(loc);
		char *path = loc;
	}

	return summery;
}


actionSummery checkForMove(char *input){
	char *loc = strstr(input, "move");
	char *loc2 = strstr(input, "get_moves");
	char *loc3 = strstr(input, "get_best_moves");

	actionSummery summery = { 0, 0, 0, 0 };
	if (loc != NULL && loc2 == NULL && loc3 == NULL){
		summery.isFound = 1;
		loc = loc + 4;
		loc = getNextChar(loc);

		// Pars the soldier to move

		locationNode origin = getNextLocation(&loc);
		if (origin.column == -1 && origin.row == -1){
			summery.isError = 1;
			strcmp(summery.failedFunc, "calloc");

			return summery;
		}

		++loc;
		loc = getNextChar(loc);
		loc = loc + 2; // pass "to"
		loc = getNextChar(loc);

		locationNode destination = getNextLocation(&loc);
		if (origin.column == -1 && origin.row == -1){
			summery.isError = 1;
			strcmp(summery.failedFunc, "calloc");

			return summery;
		}

		// Check for promotion
		++loc; // pass the > sign
		loc = getNextChar(loc);
		char *soldierToPromoteTo;
		if (loc != NULL && loc != "\0"){
			// the user asked for promotion
			soldierToPromoteTo = loc;
		}

		moveList move;
		move.origin = origin;
		move.destination = destination;
		strcmp(move.soldierToPromoteTo,soldierToPromoteTo);

		int isValid = isValidMove(move, game_board.isBlackTurn, 1);
		if (isValid == 1){
			summery.isExecuted = 1;
			moveUser(move);
			print_board(game_board.board);
		}
	}

	return summery;

}

locationNode getNextLocation(char **loc){
	++(*loc); // pass "<"
	char* commaSign = strstr(*loc, ",");
	char* columnChar = getSubString(*loc, commaSign);
	if (columnChar == NULL){
		locationNode errLoc = { -1, -1 };
		return errLoc;
	}

	char* closureSign = strstr(*loc, ">");
	char* rowStr = getSubString(commaSign + 1, closureSign);
	if(rowStr == NULL){
		myFree(columnChar);

		locationNode errLoc = { -1, -1 };
		return errLoc;
	}

	double row = strtod(rowStr, NULL);

	locationNode location = convertBoardLocationToNumericLocation(columnChar[0], row);

	*loc = strstr(*loc, ">");

	myFree(columnChar);
	myFree(rowStr);

	return location;
}

int isValidMove(moveList soldierMove, int isBlack, int isShowMessage){
	//(1)
	int isOriginValid = isLocationValid(soldierMove.origin, 0);
	int isDestinationValid = isLocationValid(soldierMove.destination, 0);
	if (isOriginValid == 0 || isDestinationValid == 0){
		print_message(WRONG_POSITION);
		return 0;
	}

	//(2)
	char soldierToMove = game_board.board[soldierMove.origin.column][soldierMove.origin.row];
	if ((isBlack == 1 && (soldierToMove == WHITE_B || soldierToMove == WHITE_K || soldierToMove == WHITE_N || soldierToMove == WHITE_P || soldierToMove == WHITE_Q || soldierToMove == WHITE_R)) ||
		(isBlack == 0 && (soldierToMove == BLACK_B || soldierToMove == BLACK_K || soldierToMove == BLACK_N || soldierToMove == BLACK_P || soldierToMove == BLACK_Q || soldierToMove == BLACK_R))){
		// The soldier in that we try to move is not ours (or empty)!

		print_message(NO_DICS);
		return 0;
	}

	// (3)
	// call Haim function get_All_Valid_Moves and check of my move is in the list
	return 1;

}