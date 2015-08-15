#include "gameStateManager.h"

actionSummery readGameActions(){
	int isGameMate = 0;
	int isGameTie = 0;
	int isError = 0;
	actionSummery summery = { 0, 0, 0, 0 };

	char* input = (char*)myCalloc(sizeof(char), 50);
	if (input == NULL){
		summery.isError = 1;
		strcpy(summery.failedFunc, "calloc");

		return summery;
	}

	while (strcmp(input, "quit") != 0 && isGameMate == 0 && isGameTie == 0 && summery.isError == 0){

		char *msg = (game_board.isBlackTurn ? "black player - enter your move!\n" : "white player - enter your move!\n");
		print_message(msg);

		if ((settings.gameMode == PLAYER_VS_AI && 
			((game_board.isBlackTurn == 1 && settings.isUserBlack == 1) ||
			(game_board.isBlackTurn == 0 && settings.isUserBlack == 0))) || 
			(settings.gameMode == TWO_PLAYERS)){
			
			// enter here if its a 2 player game OR we play against the computer, we are player color X and it's color X turn

			getInput(&input);
			if (strcmp(input, "quit") == 0){
				break;
			}

			summery = executeGameActions(input);
			if (summery.isExecuted == 1){
				isGameMate = isMate(1 - game_board.isBlackTurn, 1); // Check if there is a mante for the opponent
				if (isGameMate == 2){
					// ERROR
					summery.isError = 1;
				}
				else if (isGameMate == 0){
					isGameTie = isTie(1 - game_board.isBlackTurn, 1);
					if (isGameTie == 2){
						// ERROR
						summery.isError = 1;
					}
				}
				// now it's the computer turn
				game_board.isBlackTurn = 1 - game_board.isBlackTurn;
			}

		}
		else{
			// it's the computer move
			computerTurn();
			print_board(game_board.board);
			
			isGameMate = isMate(1 - game_board.isBlackTurn, 1); // Check if there is a mante for the opponent
			if (isGameMate == 2){
				// ERROR
				summery.isError = 1;
			}
			else if (isGameMate == 0){
				isGameTie = isTie(1 - game_board.isBlackTurn, 1);
				if (isGameTie == 2){
					// ERROR
					summery.isError = 1;
				}
			}

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
		summery.isFound = 1;
		loc = loc + 9;
		loc = getNextChar(loc);
		locationNode origin = getNextLocation(&loc);
		if (origin.row == -1 && origin.column == -1){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		int isValid = isLocationValid(origin, 1);
		if (isValid){
			isValid = isPositionContainUserPiece(game_board.isBlackTurn, origin, 1);
			if (isValid){
				moveList *lst = getValidMovesForLocation(origin);
				if (lst == NULL){
					// Error in getValidMovesForLocation
					summery.isError = 1;
					strcpy(summery.failedFunc, "malloc");

					return summery;
				}

				int isSuccess = printAllPossibleMoves(lst);
				freeAllMoveList(lst);
				if (isSuccess == 0){
					// ERROR
					summery.isError = 1;
					strcpy(summery.failedFunc, "calloc");

					return summery;
				}
				
			}
		}
	}

	return summery;
}

actionSummery checkForBestMoves(char *input){
	char *loc = strstr(input, "get_best_moves");
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound = 1;
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
		summery.isFound = 1;
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
		summery.isFound = 1;
		loc = loc + 4;
		loc = getNextChar(loc);
		char *path = loc;

		// Files - Haim
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
		int isNeedToPromote = 0;
		if (loc != NULL && *loc != '\0'){
			// the user asked for promotion
			soldierToPromoteTo = loc;
			isNeedToPromote = 1;
		}

		moveList move;
		move.origin = origin;
		move.destination = destination;
		move.soldierToPromoteTo = isNeedToPromote == 1 ? convertSoldierNameToSoldierType(loc, game_board.isBlackTurn) : 32;// 32 is the empty string

		int isValid = isValidMove(move, game_board.isBlackTurn, 1);
		if (isValid == 2){
			// ERROR
			summery.isError = 1;
			strcmp(summery.failedFunc, "calloc");

			return summery;
		}

		if (isValid == 1){
			summery.isExecuted = 1;
			moveUser(move, settings.isUserBlack);
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
	// 0 - NO, 1-YES, 2-ERROR
	
	//(1)
	int isOriginValid = isLocationValid(soldierMove.origin, 0);
	int isDestinationValid = isLocationValid(soldierMove.destination, 0);
	if (isOriginValid == 0 || isDestinationValid == 0){
		print_message(WRONG_POSITION);
		return 0;
	}

	//(2)
	if (isPositionContainUserPiece(isBlack, soldierMove.origin, 0) == 0){
		// The soldier in that we try to move is not ours (or empty)!

		print_message(NO_DICS);
		return 0;
	}

	// (3)
	int isValid = 0;
	moveList *list = getValidMovesForLocation(soldierMove.origin);
	if (list == NULL){
		// ERROR
		isValid = 2;
	}
	int isEmpty = isEmptyMoveList(list);
	if (isEmpty == 1){
		// There is no Valid moves to take
		isValid = 0;
	}
	else{
		moveList *move = list;
		int isCurrentMoveEqaule = 0;
		while (move != NULL && isCurrentMoveEqaule == 0){
			isCurrentMoveEqaule = isMovesEquale(soldierMove, *move);
			move = move->next;
		}

		isValid = isCurrentMoveEqaule;
		
	}

	freeAllMoveList(list);

	if (isValid == 0){
		print_message(ILLEGAL_MOVE);
	}
	return isValid;

}

int isMovesEquale(moveList move1, moveList move2){
	int isEquale = 0;
	if (isLocationNodeEquale(move1.origin, move2.origin) == 1 &&
		isLocationNodeEquale(move1.destination, move2.destination) == 1 &&
		move1.soldierToPromoteTo == move2.soldierToPromoteTo){
		isEquale = 1;
	}

	return isEquale;
}

int isLocationNodeEquale(locationNode node1, locationNode node2){
	int isEquale = 0;
	if (node1.column == node2.column && node1.row == node2.row){
		isEquale = 1;
	}

	return isEquale;
}

int isPositionContainUserPiece(int isBlack, locationNode position, int isShowMessage){
	int isValid = 1;
	char soldierToMove = game_board.board[position.row][position.column];
	if ((isBlack == 1 && (soldierToMove == WHITE_B || soldierToMove == WHITE_K || soldierToMove == WHITE_N || soldierToMove == WHITE_P || soldierToMove == WHITE_Q || soldierToMove == WHITE_R)) ||
		(isBlack == 0 && (soldierToMove == BLACK_B || soldierToMove == BLACK_K || soldierToMove == BLACK_N || soldierToMove == BLACK_P || soldierToMove == BLACK_Q || soldierToMove == BLACK_R))){
		isValid = 0;
	}

	if (isValid == 0){
		print_message(NO_DICS);
	}

	return isValid;
}

int printAllPossibleMoves(moveList* moves){
	if (moves == NULL || isEmptyMoveList(moves)){
		return;
	}

	moveList* current = moves;
	while (current != NULL){
		int isSuccess = printOneMove(*current);
		if (isSuccess == 0){
			//ERROR
			return 0;
		}

		current = current->next;
	}

	return 1;
}

int printOneMove(moveList move){
	// print origin
	locationInLetters originToPrint = convertNumericLocationToBoardLocation(move.origin.column, move.origin.row);
	locationInLetters destinationToPrint = convertNumericLocationToBoardLocation(move.destination.column, move.destination.row);
	if (move.soldierToPromoteTo == 32){ // 32 is empty strings
		printf("<%c,%d> to <%c,%d>", originToPrint.column, originToPrint.row, destinationToPrint.column, destinationToPrint.row);
	}
	else{
		char *soldierToPromoteTo = convertSoldierTypeToSoldierName(move.soldierToPromoteTo);
		if (soldierToPromoteTo == NULL){
			// ERROR
			return 0;
		}
		printf("<%c,%d> to <%c,%d> %s", originToPrint.column, originToPrint.row, destinationToPrint.column, destinationToPrint.row, soldierToPromoteTo);
		myFree(soldierToPromoteTo);
	}

	printf("\n");
	return 1;
}

// Haim wrote it in ChessLogic.c
int isCheck(int isBlack, int isShowMessage){
	return 0;
}

int isMate(int isBlack, int isShowMessage){
	// There is a mate if my king is threatened and I don't have where to move
	locationNode node = getKingLocation(isBlack);
	if (node.row == -1 || node.column == -1){
		// ERROR
		return 0;
	}

	// 0- black, 1- white
	int isCheck = amITretrnd(node, 1 - isBlack) == 1;
	if (isCheck == 2){
		// ERROR
		return 2;
	}
	moveList* list = getAllValidMoves(isBlack);
	if (list == NULL){
		// ERROR
		return 2;
	}
	int isListEmpty = isEmptyMoveList(list);
	int isMate = 0;

	if (isCheck && isListEmpty){
		// MATE
		isMate = 1;
	}
	
	if (isShowMessage == 1 && isMate){
		// if the black player has mate, the white player wins and the opposit. 
		char *msg = (isBlack ? "Mate! white player wins the game\n" : "Mate! black player wins the game\n");
		print_message(msg);
	}

	return isMate;
}

int isTie(int isBlack, int isShowMessage){
	// There is a tie if my king is not threatened and I don't have where to move

	locationNode node= getKingLocation(isBlack);

	// 0- black, 1- white
	int isCheck = amITretrnd(node, 1 - isBlack) == 1;
	if (isCheck == 2){
		// ERROR
		return 0;
	}
	moveList* list = getAllValidMoves(isBlack);
	if (list == NULL){
		// ERROR
		return 2;
	}
	int isListEmpty = isEmptyMoveList(list);
	int isTie = 0;

	if (!isCheck && isListEmpty){
		// TIE
		isTie = 1;
	}

	if (isShowMessage == 1 && isTie){
		print_message(TIE);
	}

	return isTie;
}

locationNode getKingLocation(int isBlack){
	locationNode kingLoc = {-1,-1};
	int isFound = 0;

	// search the king on the board
	for (int row = 0; row < BOARD_SIZE; row++){
		for (int col = 0; col < BOARD_SIZE; col++){
			if (isBlack && game_board.board[row][col] == BLACK_K){
				kingLoc.row = row;
				kingLoc.column = col;
				isFound = 1;
				break;
			}
			else if (!isBlack && game_board.board[row][col] == WHITE_K){
				kingLoc.row = row;
				kingLoc.column = col;
				isFound = 1;
				break;
			}
		}
		if (isFound == 1){
			break;
		}
	}
	return kingLoc;
}

char* convertSoldierTypeToSoldierName(char type){
	char *name = (char*)calloc(7, sizeof(char));
	if (name == NULL){
		// ERROR
		return NULL;
	}

	switch (type){
	case BLACK_B:
	case WHITE_B:
		strcpy(name, BISHOP);
		break;
	case BLACK_K:
	case WHITE_K:
		strcpy(name, KING);
		break;
	case BLACK_N:
	case WHITE_N:
		strcpy(name, KNIGHT);
		break;
	case BLACK_P:
	case WHITE_P:
		strcpy(name, PAWN);
		break;
	case BLACK_Q:
	case WHITE_Q:
		strcpy(name, QUEEN);
		break;
	case BLACK_R:
	case WHITE_R:
		strcpy(name, ROOK);
		break;
	}

	return name;
}

char convertSoldierNameToSoldierType(char *name, int isBlack){
	char type;

	/*if (strcmp(name, PAWN) == 0 && !isBlack){
		type = WHITE_P;
	}
	else if (strcmp(name, KNIGHT) == 0 && !isBlack){
		type = WHITE_N;
	}
	else if (strcmp(name, BISHOP) == 0 && !isBlack){
		type = WHITE_B;
	}
	else if (strcmp(name, ROOK) == 0 && !isBlack){
		type = WHITE_R;
	}
	else if (strcmp(name, QUEEN) == 0 && !isBlack){
		type = WHITE_Q;
	}
	else if (strcmp(name, KING) == 0 && !isBlack){
		type = WHITE_K;
	}
	else if (strcmp(name, PAWN) == 0 && isBlack){
		type = BLACK_P;
	}
	else if (strcmp(name, KNIGHT) == 0 && isBlack){
		type = BLACK_N;
	}
	else if (strcmp(name, BISHOP) == 0 && isBlack){
		type = BLACK_B;
	}
	else if (strcmp(name, ROOK) == 0 && isBlack){
		type = BLACK_R;
	}
	else if (strcmp(name, QUEEN) == 0 && isBlack){
		type = BLACK_Q;
	}
	else if (strcmp(name, KING) == 0 && !isBlack){
		type = BLACK_K;
	}*/


	if (strcmp(name, PAWN) == 0){
		type = WHITE_P;
	}
	else if (strcmp(name, KNIGHT) == 0){
		type = WHITE_N;
	}
	else if (strcmp(name, BISHOP) == 0){
		type = WHITE_B;
	}
	else if (strcmp(name, ROOK) == 0){
		type = WHITE_R;
	}
	else if (strcmp(name, QUEEN) == 0){
		type = WHITE_Q;
	}
	else if (strcmp(name, KING) == 0){
		type = WHITE_K;
	}

	return type;
}

char getSoldierTypeByColor(char type, int isBlack){
	int typeByColor;
	switch (type){
	case WHITE_B:
		typeByColor = isBlack ? BLACK_B : WHITE_B;
		break;
	case WHITE_K:
		typeByColor = isBlack ? BLACK_K : WHITE_K;
		break;
	case WHITE_N:
		typeByColor = isBlack ? BLACK_N : WHITE_N;
		break;
	case WHITE_P:
		typeByColor = isBlack ? BLACK_P : WHITE_P;
		break;
	case WHITE_Q:
		typeByColor = isBlack ? BLACK_Q : WHITE_Q;
		break;
	case WHITE_R:
		typeByColor = isBlack ? BLACK_R : WHITE_R;
		break;
	}

	return typeByColor;
}

void computerTurn(){

	minmaxValue result = minmax(getCurrentBoardData(), settings.minmax_depth, 1, -99999, 99999, 1 - settings.isUserBlack);


	/*if (result.bestMove.path != NULL){
	moveUser(result.bestMove.path);

	printf("Computer: move ");
	printOneMove(result.bestMove.path);

	freeLocList(result.bestMove.path);
	}*/
}

void moveUser(moveList userMove, int isBlack){
	char type = game_board.board[userMove.origin.row][userMove.origin.column];
	char destinationSoldierType = game_board.board[userMove.destination.row][userMove.destination.column];
	removeUser(userMove.origin);
	if (destinationSoldierType != EMPTY){
		removeUser(userMove.destination);
	}

	if (userMove.soldierToPromoteTo != EMPTY){
		// The soldier need to be promoted
		type = getSoldierTypeByColor(userMove.soldierToPromoteTo, isblank);
	}
	addUserByValue(userMove.destination, type);
}

