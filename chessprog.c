#include "Chess.h"

int main()
{
	init_board(game_board.board);
	print_board(game_board.board);

	actionSummery summery = readSettings();
	if (summery.isError){
		//perror_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			//readGameActions();
		}
	}
	

	getchar();
	return 1;
}

actionSummery readSettings(){
	actionSummery summery = { 0, 0, 0, 0 };

	char *input = myMalloc(sizeof(char)*50);
	if (input == NULL){
		summery.isError = 1;
		strcpy(summery.failedFunc, "malloc");

		return summery;
	}

	int isStart = 0;
	while (isStart == 0){
		print_message(ENTER_SETTINGS);
		getInput(&input);

		if (strcmp(input, "quit") == 0){
			break;
		}

			summery = executeSettings(input);
	}

	// free the input
	//myFree(input);

	return summery;
}

void getInput(char** inputPointer){
	char* input = *inputPointer;
	int curSize = 0;
	do{
		input[curSize] = getchar();
		curSize++;
	} while (input[curSize - 1] != '\n');

	input[curSize - 1] = '\0';
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
	actionSummery summery = { 0, 0, 0, 0 };

	if (loc != NULL){
		summery.isFound = 1;

		print_board(game_board.board);
	}

	return summery;
}

actionSummery checkForStart(char* input){
	char* loc = strstr(input, "start");
	actionSummery summery = { 0, 0, 0, 0 };

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
	actionSummery summery = { 0, 0, 0, 0 };

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
			isValid = isBoardValidAfterSet(type[0], color, 1);
			if (isValid){
				//removeUser(node); // Call remove in order to set counters
				//addUser(node, color, *type);
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
	actionSummery summery = { 0, 0, 0, 0 };

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
			//removeUser(node);
		}

		myFree(rowStr);
		myFree(columnChar);
	}

	return summery;
}

actionSummery checkForNextPlayer(char *input){
	char *loc = strstr(input, "next_player");
	actionSummery summery = { 0, 0, 0, 0 };
	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 11;
		loc = getNextChar(loc);

		if (strstr(loc, "white")){
			game_board.isBlackTurn = 0;
		}
		else if (strstr(loc, "black")){
			settings.isUserBlack = 1;
		}
	}

	return summery;
}


actionSummery checkForClear(char *input){
	char *loc = strstr(input, "clear");
	actionSummery summery = { 0, 0, 0, 0 };
	if (loc != NULL){
		summery.isFound = 1;

		// Clear the board
	}

	return summery;
}

actionSummery checkForLoad(char *input){
	char *loc = strstr(input, "load");
	actionSummery summery = { 0, 0, 0, 0 };
	if (loc != NULL){
		summery.isFound = 1;
		loc = loc + 4;
		loc = getNextChar(loc);

		// load file - HAIM
	}

	return summery;
}
actionSummery checkForUserColor(char* input){
	actionSummery summery = { 0, 0, 0, 0 };

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
	actionSummery summery = {0,0,0,0};

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
	actionSummery summery = { 0, 0, 0, 0 };

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

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[j][i]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

locationNode convertBoardLocationToNumericLocation(char column, int row){
	locationNode node;
	node.column = column - 97;
	node.row = row - 1;

	return node;
}

locationInLetters convertNumericLocationTpBoardLocation(int column, int row){
	locationInLetters node;
	node.column = column + 97;
	node.row = row + 1;

	return node;
}

char* getNextChar(char* str){
	char* currentChar = str;
	// continue till we find a char
	while (currentChar != NULL && *currentChar != '\0')
	{
		char c = *currentChar;
		if (c == ' ' || c == '\t'){
			++currentChar;
		}
		else{
			break;
		}
	}

	return currentChar;
}

void* myCalloc(int x, int y){
	void* alloc = calloc(x, y);
	//myAssert(alloc != NULL, "calloc");
	//numOfTotalAllocs++;
	//countAlloc++;
	return alloc;
}

void* myMalloc(int x){
	return myCalloc(1, x);
}

void myFree(void* tofree){
	//countAlloc--;
	free(tofree);
}

void* myRealloc(void* point, int x){

	void* new = realloc(point, x);
	//myAssert(new != NULL, "reallloc");
	return new;
}

char* getSubString(const char* input, const  char* Loc){
	int len = Loc - input;
	char* newArr = (char*)myCalloc(sizeof(char), len + 1);
	strncpy(newArr, input, len);
	newArr[len] = '\0';
	return newArr;
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

int isBoardValidAfterSet(char type, char *color, int isShowMessage){
	int isValid = 1;
	
	if (strcmp(color,"black") == 0){
		
		if ((type == BLACK_B && game_board.numOfBlackBishops == 2)||
			(type == BLACK_K && game_board.numOfBlackKings == 1)  ||
			(type == BLACK_N && game_board.numOfBlackKnights == 2)||
			(type == BLACK_P && game_board.numOfBlackPawns == 8)  ||
			(type == BLACK_Q && game_board.numOfBlackQueens == 1) ||
			(type == BLACK_R && game_board.numOfBlackRooks == 2)){
			isValid = 0;
		}		
	}
	else{
		if ((type == WHITE_B && game_board.numOfWhiteBishops == 2) ||
			(type == WHITE_K && game_board.numOfWhiteKings == 1) ||
			(type == WHITE_N && game_board.numOfWhiteKnights == 2) ||
			(type == WHITE_P && game_board.numOfWhitePawns == 8) ||
			(type == WHITE_Q && game_board.numOfWhiteQueens == 1) ||
			(type == WHITE_R && game_board.numOfWhiteRooks == 2)){
			isValid = 0;
		}
	}

	if (isValid == 0 && isShowMessage == 1){
		print_message(NO_PIECE);
	}

	return isValid;
}

int isBoardValidToStartGame(int isShowMessage){
	if ((game_board.numOfBlackKings < 1 ) || game_board.numOfWhiteKings < 1){
		if (isShowMessage == 1){
			print_message(WROND_BOARD_INITIALIZATION);
		}

		return 0;
	}

	return 1;
}
