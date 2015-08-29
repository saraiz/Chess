#include "settingsStateManager.h"

actionSummery readSettings(){
	actionSummery summery = createEmptySummery();

	char *input = myMalloc(sizeof(char) * 50);
	if (input == NULL){
		summery.isError = 1;
		strcpy(summery.failedFunc, "malloc");

		return summery;
	}

	int isStart = 0;
	int isError = 0;
	while (isStart == 0 && isStart == 0 && isError == 0){
		print_message(ENTER_SETTINGS);
		getInput(&input);

		if (strcmp(input, "quit") == 0){
			break;
		}

		summery = executeSettings(input);
		isStart = summery.isExecuted;
		isError = summery.isError;
	}

	// free the input
	myFree(input);

	return summery;
}

actionSummery executeSettings(char* input){
	actionSummery summery = checkForGameMode(input);
	if (summery.isFound == 0){
		summery = checkForDifficulty(input);
		if (summery.isFound == 0){
			summery = checkForUserColor(input);
			if (summery.isFound == 0){
				summery = checkForLoad(input);
				if (summery.isFound == 0){
					summery = checkForClear(input);
					if (summery.isFound == 0){
						summery = checkForNextPlayer(input);
						if (summery.isFound == 0){
							summery = checkForRemove(input);
							if (summery.isFound == 0){
								summery = checkForSet(input);
								if (summery.isFound == 0){
									summery = checkForPrint(input);
									if (summery.isFound == 0){
										summery = checkForStart(input);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (summery.isFound == 0){
		// means that the command is not legal
		print_message(ILLEGAL_COMMAND);
	}

	return summery;
}

actionSummery checkForPrint(char* input){
	char* loc = strstr(input, "print");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;

		print_board(game_board.board);
	}

	return summery;
}

actionSummery checkForStart(char* input){
	char* loc = strstr(input, "start");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;

		if (isBoardValidToStartGame(1) == 1){
			summery.isExecuted = 1;
		}
	}

	return summery;
}

actionSummery checkForSet(char* input){
	char* loc = strstr(input, "set");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 3;
		loc = getNextChar(loc);

		loc++; // pass "<"
		char* commaSign = strstr(loc, ",");
		char* columnChar = getSubString(loc, commaSign);
		if (columnChar == NULL){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		char* closureSign = strstr(loc, ">");
		char* rowStr = getSubString(commaSign + 1, closureSign);
		if (rowStr == NULL){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");
			myFree(columnChar);

			return summery;
		}

		double row = strtod(rowStr, NULL);

		++closureSign;
		char* nextChar = getNextChar(closureSign);
		char* nextSpace = strstr(nextChar, " ");
		char* color = getSubString(nextChar, nextSpace);
		if (color == NULL){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");
			myFree(columnChar);
			myFree(rowStr);

			return summery;
		}

		char* type = ++nextSpace;

		locationNode node = convertBoardLocationToNumericLocation(columnChar[0], row);
		int isValid = isLocationValid(node, 1);

		if (isValid)
		{
			isValid = isBoardValidAfterSet(type, color, 1);
			if (isValid){
				removeUser(node); // Call remove in order to set counters
				addUser(node, color, type);
			}
		}

		myFree(color);
		myFree(rowStr);
		myFree(columnChar);
	}

	return summery;
}

actionSummery checkForRemove(char *input){
	char* loc = strstr(input, "rm");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 2;
		loc = getNextChar(loc);

		loc++; // pass "<"
		char* commaSign = strstr(loc, ",");
		char* columnChar = getSubString(loc, commaSign);
		if (columnChar == NULL){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");

			return summery;
		}

		char* closureSign = strstr(loc, ">");
		char* rowStr = getSubString(commaSign + 1, closureSign);
		if (rowStr == NULL){
			summery.isError = 1;
			strcpy(summery.failedFunc, "calloc");
			myFree(columnChar);

			return summery;
		}

		double row = strtod(rowStr, NULL);

		locationNode node = convertBoardLocationToNumericLocation(columnChar[0], row);
		int isValid = isLocationValid(node, 1);

		if (isValid == 1){
			removeUser(node);
		}

		myFree(rowStr);
		myFree(columnChar);
	}

	return summery;
}

actionSummery checkForNextPlayer(char *input){
	char *loc = strstr(input, "next_player");
	actionSummery summery = createEmptySummery();
	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 11;
		loc = getNextChar(loc);

		if (strstr(loc, "white")){
			game_board.isBlackTurn = 0;
		}
		else if (strstr(loc, "black")){
			game_board.isBlackTurn = 1;
		}
	}

	return summery;
}


actionSummery checkForClear(char *input){
	char *loc = strstr(input, "clear");
	actionSummery summery = createEmptySummery();
	if (loc != NULL){
		summery.isFound = 1;

		clearBoard();
	}

	return summery;
}

actionSummery checkForLoad(char *input){
	char *loc = strstr(input, "load");
	actionSummery summery = createEmptySummery();
	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 4;
		loc = getNextChar(loc);

		fileData gameData = loadGame(loc);
		if (gameData.difficulty == -1 && 
			gameData.gameMode == -1 && 
			gameData.isNextBlack == -1 && 
			gameData.isUserColorBlack == -1){
			// ERROR

			summery.isError = 1;
			strcpy(summery.failedFunc, "fopen");

			return summery;
		}
	}

	return summery;
}
actionSummery checkForUserColor(char* input){
	actionSummery summery = createEmptySummery();

	if (settings.gameMode == PLAYER_VS_AI){
		char* loc = strstr(input, "user_color");

		if (loc != NULL){
			summery.isFound = 1;
			if (strstr(loc, "white")){
				settings.isUserBlack = 0;
			}
			else if (strstr(loc, "black")){
				settings.isUserBlack = 1;
			}
		}
	}

	return summery;
}

actionSummery checkForGameMode(char *input){
	char* loc = strstr(input, "game_mode");
	actionSummery summery = createEmptySummery();

	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 9;
		loc = getNextChar(loc);
		double value = strtod(loc, NULL);

		settings.gameMode = value;
		if (value == TWO_PLAYERS){
			print_message(TWO_PLAYERS_GAME_MODE);
		}
		else{
			print_message(PLAYER_VS_AI_GAME_MODE);
		}
	}

	return summery;
}

actionSummery checkForDifficulty(char *input){
	actionSummery summery = createEmptySummery();

	if (settings.gameMode == PLAYER_VS_AI){
		char* loc = strstr(input, "difficulty depth");
		char* loc2 = strstr(input, "difficulty best");

		if (loc != NULL){
			summery.isFound = 1;
			loc = loc + 16;
			loc = getNextChar(loc);
			double value = strtod(loc, NULL);

			// Check that the value is valid
			if (value < 1 || value > 4){
				print_message(WRONG_MINIMAX_DEPTH);
			}
			else{
				settings.minmax_depth = value;
			}
		}
		else if (loc2 != NULL){
			summery.isFound = 1;
			settings.minmax_depth = BEST;
		}
	}
	return summery;
}
void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
	
	// put EMPTY sign is all cells
	clearBoard();
	
	// place pawns
	int i;
	for (i = 0; i < BOARD_SIZE; i++){
		board[1][i] = WHITE_P;
		board[6][i] = BLACK_P;
		game_board.numOfBlackPawns = 8;
		game_board.numOfWhitePawns = 8;
	}

	// place Rooks
	board[0][0] = WHITE_R;
	board[0][7] = WHITE_R;
	board[7][0] = BLACK_R;
	board[7][7] = BLACK_R;
	game_board.numOfBlackRooks = 2;
	game_board.numOfWhiteRooks = 2;

	// place knights
	board[0][1] = WHITE_N;
	board[0][6] = WHITE_N;
	board[7][1] = BLACK_N;
	board[7][6] = BLACK_N;
	game_board.numOfBlackKnights = 2;
	game_board.numOfWhiteKnights = 2;

	// place bishops
	board[0][2] = WHITE_B;
	board[0][5] = WHITE_B;
	board[7][2] = BLACK_B;
	board[7][5] = BLACK_B;
	game_board.numOfBlackBishops = 2;
	game_board.numOfWhiteBishops = 2;

	// place Queen
	board[0][3] = WHITE_Q;
	board[7][3] = BLACK_Q;
	game_board.numOfBlackQueens = 1;
	game_board.numOfWhiteQueens = 1;

	// place king
	board[0][4] = WHITE_K;
	board[7][4] = BLACK_K;
	game_board.numOfBlackKings = 1;
	game_board.numOfWhiteKings = 1;
}

void setSettings(char* input){
	char* currentLine = input;
	char* nextLine = input;
	while (currentLine != NULL && *currentLine != '\0'){
		nextLine = getNextLine(currentLine);
		char* line = getSubString(currentLine, nextLine);
		executeSettings(line);

		myFree(line);
		currentLine = nextLine;
	}
}

char* getNextLine(char* input){
	char* nextLine = strstr(input, "\n");
	if (nextLine != NULL){
		return nextLine + 1;
	}
	else{
		return input + strlen(input);
	}
}

int isBoardValidAfterSet(char *type, char *color, int isShowMessage){
	int isValid = 1;

	if (strcmp(color, "black") == 0){

		if ((strcmp(type, BISHOP) == 0 && game_board.numOfBlackBishops == 2) ||
			(strcmp(type, KING) == 0 && game_board.numOfBlackKings == 1) ||
			(strcmp(type, KNIGHT) == 0 && game_board.numOfBlackKnights == 2) ||
			(strcmp(type, PAWN) == 0 && game_board.numOfBlackPawns == 8) ||
			(strcmp(type, QUEEN) == 0 && game_board.numOfBlackQueens == 1) ||
			(strcmp(type, ROOK) == 0 && game_board.numOfBlackRooks == 2)){
			isValid = 0;
		}
	}
	else{
		if ((strcmp(type, BISHOP) == 0 && game_board.numOfWhiteBishops == 2) ||
			(strcmp(type, KING) == 0 && game_board.numOfWhiteKings == 1) ||
			(strcmp(type, KNIGHT) == 0 && game_board.numOfWhiteKnights == 2) ||
			(strcmp(type, PAWN) == 0 && game_board.numOfWhitePawns == 8) ||
			(strcmp(type, QUEEN) == 0 && game_board.numOfWhiteQueens == 1) ||
			(strcmp(type, ROOK) == 0 && game_board.numOfWhiteRooks == 2)){
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
