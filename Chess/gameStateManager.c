#include "gameStateManager.h"

actionSummery readGameActions(){
	int isGameMate = 0;
	int isGameTie = 0;
	int isError = 0;
	actionSummery summery = createEmptySummery();

	char* input = (char*)myCalloc(sizeof(char), 500);
	if (input == NULL){
		summery.isError = 1;
		strcpy(summery.failedFunc, "calloc");

		return summery;
	}

	// Need to check if there is a mate or tie before the first player moves
	checkForMate_Tie_Check(1 - game_board.isBlackTurn, &isError, &isGameMate, &isGameTie);


	while (strcmp(input, "quit") != 0 && isGameMate == 0 && isGameTie == 0 && summery.isError == 0){

		if ((settings.gameMode == PLAYER_VS_AI && 
			((game_board.isBlackTurn == 1 && settings.isUserBlack == 1) ||
			(game_board.isBlackTurn == 0 && settings.isUserBlack == 0))) || 
			(settings.gameMode == TWO_PLAYERS)){
			
			// enter here if its a 2 player game OR we play against the computer, we are player color X and it's color X turn

			char *msg = (game_board.isBlackTurn ? "Black player - enter your move:\n" : "White player - enter your move:\n");
			print_message(msg);

			getInput(&input);
			if (strcmp(input, "quit") == 0){
				break;
			}

			summery = executeGameActions(input);
			if (summery.isExecuted == 1){
				// The player has finished his move
				checkForMate_Tie_Check(game_board.isBlackTurn, &isError, &isGameMate, &isGameTie);
				summery.isError = isError;
				
				// now it's the computer turn
				game_board.isBlackTurn = 1 - game_board.isBlackTurn;
			}

		}
		else{
			// it's the computer move
			computerTurn(1);
			print_board(game_board.board);
			
			checkForMate_Tie_Check(game_board.isBlackTurn, &isError, &isGameMate, &isGameTie);
			summery.isError = isError;

			game_board.isBlackTurn = 1 - game_board.isBlackTurn;
		}
	}

	// free the input
	myFree(input);

	return summery;
}

void checkForMate_Tie_Check(int isBlack, int *isError, int *isGameMate, int *isGameTie){
	*isGameMate = isMate(1 - isBlack, 1); // Check if there is a mate for the opponent
	if (*isGameMate == 2){
		// ERROR
		*isError = 1;
	}
	else if (*isGameMate == 0){
		*isGameTie = isTie(1 - isBlack, 1);
		if (*isGameTie == 2){
			// ERROR
			*isError = 1;
		}
		else if (*isGameTie == 0){
			int isInCheck = isCheck(1 - isBlack, 1);
			if (isInCheck == 2){
				// ERROR
				*isError = 1;
			}
		}
	}
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
		// if no action was found, it's an illigal move	
		print_message(ILLEGAL_COMMAND);
	}

	return summery;

}

actionSummery checkForGetMoves(char *input){
	char *loc = strstr(input, "get_moves");
	actionSummery summery = createEmptySummery();

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
				// Only if the typed location is valid we need to check gor possible moves
				moveList *lst = getValidMovesForLocation(origin,0);
				if (lst == NULL){
					// Error in getValidMovesForLocation
					summery.isError = 1;
					strcpy(summery.failedFunc, "malloc");

					return summery;
				}

				printAllPossibleMoves(lst);
				freeAllMoveList(lst);
			}
		}
	}

	return summery;
}

actionSummery checkForBestMoves(char *input){
	char *loc = strstr(input, "get_best_moves");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 14;
		loc = getNextChar(loc);
		char* bestDepth = strstr(loc, "best");
		double depth;
		if (bestDepth != NULL){
			depth = getBestDepth();
		}
		else{
			depth = strtod(loc, NULL);
		}

		moveList emptyMove;
		emptyMove.destination = createLocationNode(-1, -1);
		emptyMove.origin = createLocationNode(-1, -1);
		emptyMove.soldierToPromoteTo = EMPTY;
		emptyMove.next = NULL;

		minmaxValue result = minmax(getCurrentBoardData(), depth, 1, -99999, 99999, game_board.isBlackTurn, 0, emptyMove, 1, 1);
		moveList *bestMoves = result.bestMovesList;
		
		if (bestMoves == NULL){
			// ERROR

			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		int isSuccess = printAllPossibleMoves(bestMoves);
		freeAllMoveList(bestMoves);
		if (isSuccess == 0){
			// ERROR
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}
	}

	return summery;
}

actionSummery checkForGetScore(char *input){
	char *loc = strstr(input, "get_score");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 9;
		loc = getNextChar(loc);

		char *depthEnds = strstr(loc, " ");
		if (depthEnds != NULL){
			// shouldn't be NULL
			char *depth = getSubString(loc, depthEnds);
			double value;
			if (strcmp(depth, "best") == 0){
				value = getBestDepth();
			}
			else{
				value = strtod(depth, NULL);
			}

			++depthEnds;
			// Parse move 
			moveList move = parseMove(depthEnds);
			if (isValidMove(move, game_board.isBlackTurn, 1) == 1){
				// Only if the move is valid we need to check it score
				int score = getScore(move, value);
				printf("%d\n", score);
			}
			
		}

	}

	return summery;
}

actionSummery checkForSave(char *input){
	char *loc = strstr(input, "save");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 4;
		loc = getNextChar(loc);
		char *path = loc;

		fileData data;
		data.difficulty = settings.minmax_depth;
		data.gameMode = settings.gameMode;
		data.isNextBlack = game_board.isBlackTurn;
		data.isUserColorBlack = settings.isUserBlack;
		
		int isFailure = saveGame(data, path);
		if (isFailure == 1){
			print_message(WRONG_FILE_NAME);
			return summery;
		}
	}

	return summery;
}

actionSummery checkForMove(char *input){
	char *loc = strstr(input, "move");
	char *loc2 = strstr(input, "get_moves");
	char *loc3 = strstr(input, "get_best_moves");
	char *loc4 = strstr(input, "get_score");

	actionSummery summery = createEmptySummery();
	if (loc != NULL && loc2 == NULL && loc3 == NULL && loc4 == NULL){
		// enter here only if it's a move action and not get_moves/get_best_moves/get_score which contains 
		// the word move as well
		summery.isFound = 1;
		
		moveList move = parseMove(loc);
		if (move.origin.row == -1 || move.destination.row == -1){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		int isValid = isValidMove(move, game_board.isBlackTurn, 1);
		if (isValid == 2){
			// ERROR
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		if (isValid == 1){
			summery.isExecuted = 1;
			moveUser(move, game_board.isBlackTurn);
			print_board(game_board.board);
		}
	}

	return summery;

}

/*
	get's a string and returns a struct which represent a move
	assuming that the string is representing a valid move format 
	as talked in the forum
*/
moveList parseMove(char* loc){
	moveList move;
	move.origin = createLocationNode(-1,-1);
	move.destination = createLocationNode(-1, -1);
	move.next = NULL;
	move.soldierToPromoteTo = EMPTY;

	loc = loc + 4;
	loc = getNextChar(loc);

	// Pars the soldier to move

	locationNode origin = getNextLocation(&loc);
	if (origin.column == -1 && origin.row == -1){
		return move;
	}

	++loc;
	loc = getNextChar(loc);
	loc = loc + 2; // pass the "to"
	loc = getNextChar(loc);

	locationNode destination = getNextLocation(&loc);
	if (origin.column == -1 && origin.row == -1){
		return move;
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

	
	move.origin = origin;
	move.destination = destination;
	move.soldierToPromoteTo = isNeedToPromote == 1 ? convertSoldierNameToSoldierType(soldierToPromoteTo, game_board.isBlackTurn) : 32;// 32 is the empty string

	return move;
}

/*
	searches for the next location in format <x,y>
	and returns a strucy which represent a location in the board
*/
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
		// The position is not inside the board
		print_message(WRONG_POSITION);
		return 0;
	}

	//(2)
	if (isPositionContainUserPiece(isBlack, soldierMove.origin, 0) == 0){
		// The soldier in that we try to move is not ours (or empty)!
		if (isShowMessage){
		print_message(NO_DICS);
		}
		return 0;
	}

	// (3)
	int isValid = 0;
	moveList *list = getValidMovesForLocation(soldierMove.origin,0);
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

	if (isValid == 0 && isShowMessage){
		// The move is not valid for the current user
		print_message(ILLEGAL_MOVE);
	}
	return isValid;

}

/*
	Checks if to moves are exactly the same
*/
int isMovesEquale(moveList move1, moveList move2){
	int isEquale = 0;
	if (isLocationNodeEquale(move1.origin, move2.origin) == 1 &&
		isLocationNodeEquale(move1.destination, move2.destination) == 1 &&
		move1.soldierToPromoteTo == move2.soldierToPromoteTo){
		isEquale = 1;
	}

	return isEquale;
}

/*
	Checks if 2 locations on the board are exactly the same
*/
int isLocationNodeEquale(locationNode node1, locationNode node2){
	int isEquale = 0;
	if (node1.column == node2.column && node1.row == node2.row){
		isEquale = 1;
	}

	return isEquale;
}

/*
	Checks if the position containes a user piece by it's color using isBlack
*/
int isPositionContainUserPiece(int isBlack, locationNode position, int isShowMessage){
	int isValid = 1;
	char soldierToMove = game_board.board[position.row][position.column];
	if ((soldierToMove == EMPTY) ||
		(isBlack == 1 && (soldierToMove == WHITE_B || soldierToMove == WHITE_K || soldierToMove == WHITE_N || soldierToMove == WHITE_P || soldierToMove == WHITE_Q || soldierToMove == WHITE_R)) ||
		(isBlack == 0 && (soldierToMove == BLACK_B || soldierToMove == BLACK_K || soldierToMove == BLACK_N || soldierToMove == BLACK_P || soldierToMove == BLACK_Q || soldierToMove == BLACK_R))){
		isValid = 0;
	}

	if (isValid == 0 && isShowMessage){
		// The position does not contains the user piece
		print_message(NO_DICS);
	}

	return isValid;
}

/*
	prints a list of moves
*/
int printAllPossibleMoves(moveList* moves){
	if (moves == NULL || isEmptyMoveList(moves)){
		return 0;
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

/*
	print one move in the right format
*/
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

/*
	check if the current situation is check
	0-NO
	1-YES
	2-ERROR
*/
int isCheck(int isBlack, int isShowMessage){
	//coppied to Mate_Tie_Check in guiBoard
	int isKingThreated = amIThreatened(isBlack);

	if (isKingThreated == 2){
		// ERROR
		return 2;
	}

	if (isKingThreated == 1 && isShowMessage == 1){
		print_message(CHECK);
	}

	return isKingThreated;
}

/*
	check if the current situation is mate
	0-NO
	1-YES
	2-ERROR
*/
int isMate(int isBlack, int isShowMessage){
	//coppied to Mate_Tie_Check in guiBoard
	int isCheck = amIThreatened(isBlack) == 1;

	if (isCheck == 2){
		// ERROR
		return 2;
	}
	moveList* list = getAllValidMoves(isBlack, 0);
	if (list == NULL){
		// ERROR
		return 2;
	}
	int isListEmpty = isEmptyMoveList(list);
	freeAllMoveList(list);

	int isMate = 0;

	if (isCheck && isListEmpty){
		// MATE - the king is threatened and the player has no valid moves
		isMate = 1;
	}
	
	if (isShowMessage == 1 && isMate){
		// if the black player has mate, the white player wins and the opposit. 
		char *msg = (isBlack ? "Mate! White player wins the game\n" : "Mate! Black player wins the game\n");
		print_message(msg);
	}

	return isMate;
}

/*
	check if the current situation is a tie
	0-NO
	1-YES
	2-ERROR
*/
int isTie(int isBlack, int isShowMessage){
	int isCheck = amIThreatened(isBlack) == 1;

	if (isCheck == 2){
		// ERROR
		return 0;
	}
	moveList* list = getAllValidMoves(isBlack, 0);
	if (list == NULL){
		// ERROR
		return 2;
	}
	int isListEmpty = isEmptyMoveList(list);
	freeAllMoveList(list);

	int isTie = 0;

	if (!isCheck && isListEmpty){
		// TIE - the king is not threatened and I don't have where to move
		isTie = 1;
	}

	if (isShowMessage == 1 && isTie){
		print_message(TIE);
	}

	return isTie;
}

/*
	searched for the king on the board and returns its location
*/
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

/*
	get's a char representing the soldier type and converting it to its full name
*/
char* convertSoldierTypeToSoldierName(char type){
	char *name = (char*)myCalloc(7, sizeof(char));
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


/*
	gets the soldier full name and returns its type (not case sensative, it's always in lower case)
*/
char convertSoldierNameToSoldierType(char *name, int isBlack){
	char type;

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

/*
	execute the coputer turn
*/
void computerTurn(int isToPrint){
	moveList emptyMove;
	emptyMove.origin = createLocationNode(-1, -1);
	emptyMove.destination = createLocationNode(-1, -1);
	emptyMove.soldierToPromoteTo = EMPTY;
	emptyMove.next = NULL;

	int depth = settings.minmax_depth;
	if (depth == BEST){
		depth = getBestDepth();
	}

	minmaxValue result = minmax(getCurrentBoardData(), depth , 1, -99999, 99999, 1 - settings.isUserBlack, 0, emptyMove, 0, 1);
	if (result.bestMove.destination.row != -1 && result.bestMove.destination.column != -1){
		moveUser(result.bestMove, 1 - settings.isUserBlack);

		if (isToPrint){
			printf("Computer: move ");
			printOneMove(result.bestMove);
		}
	}
}

/*
	check what is the best moves for the currnt user - used only for GUI
*/
moveList GuiGetBestMove(int depth){
	moveList emptyMove;
	emptyMove.destination = createLocationNode(-1, -1);
	emptyMove.soldierToPromoteTo = EMPTY;
	emptyMove.next = NULL;


	if (depth == BEST){
		depth = getBestDepth();
	}

	minmaxValue result = minmax(getCurrentBoardData(), depth, 1, -99999, 99999, game_board.isBlackTurn, 0, emptyMove, 0, 1);
	return result.bestMove;
}

/*
	move a soldier
	remove it from the prev location and set in in the new one.
	only handle promotion
*/
void moveUser(moveList userMove, int isBlack){
	char type = game_board.board[userMove.origin.row][userMove.origin.column];
	char destinationSoldierType = game_board.board[userMove.destination.row][userMove.destination.column];
	removeUser(userMove.origin);
	if (destinationSoldierType != EMPTY){
		removeUser(userMove.destination);
	}

	if (userMove.soldierToPromoteTo != EMPTY){
		// The soldier need to be promoted
		type = getSoldierTypeByColor(userMove.soldierToPromoteTo, isBlack);
	}
	addUserByValue(userMove.destination, type);
}

/*
	returns the score for the sent move
*/
int getScore(moveList move, int depth){
	minmaxValue result = minmax(getCurrentBoardData(), depth, 1, -99999, 99999, game_board.isBlackTurn, 1, move, 0, 1);

	return result.score;
}



