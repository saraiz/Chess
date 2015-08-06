#include "utils.h"

void getInput(char** inputPointer){
	char* input = *inputPointer;
	int curSize = 0;
	do{
		input[curSize] = getchar();
		curSize++;
	} while (input[curSize - 1] != '\n');

	input[curSize - 1] = '\0';
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

char* getSubString(const char* input, const  char* Loc){
	int len = Loc - input;
	char* newArr = (char*)myCalloc(sizeof(char), len + 1);
	strncpy(newArr, input, len);
	newArr[len] = '\0';
	return newArr;
}

void removeUser(locationNode location){
	char type = game_board.board[location.row][location.column];
	switch (type)
	{
	case BLACK_B:
		game_board.numOfBlackBishops--;
		break;
	case WHITE_B:
		game_board.numOfWhiteBishops--;
		break;
	case BLACK_K:
		game_board.numOfBlackKings--;
		break;
	case WHITE_K:
		game_board.numOfWhiteKings--;
		break;
	case BLACK_N:
		game_board.numOfBlackKnights--;
		break;
	case WHITE_N:
		game_board.numOfWhiteKnights--;
		break;
	case BLACK_P:
		game_board.numOfBlackPawns--;
		break;
	case WHITE_P:
		game_board.numOfWhitePawns--;
		break;
	case BLACK_Q:
		game_board.numOfBlackQueens--;
		break;
	case WHITE_Q:
		game_board.numOfWhiteQueens--;
		break;
	case BLACK_R:
		game_board.numOfBlackRooks--;
		break;
	case WHITE_R:
		game_board.numOfWhiteRooks--;
		break;
	}

	game_board.board[location.row][location.column] = EMPTY;
}

void addUser(locationNode node, char* color, char *type){
	if (strcmp(color, COLOR_BLACK) == 0){
		// add black soldie
		if (strcmp(type, KING) == 0){
			game_board.board[node.row][node.column] = BLACK_K;
			game_board.numOfBlackKings++;
		}
		else if (strcmp(type, QUEEN) == 0){
			game_board.board[node.row][node.column] = BLACK_Q;
			game_board.numOfBlackQueens++;
		}
		else if (strcmp(type, ROOK) == 0){
			game_board.board[node.row][node.column] = BLACK_R;
			game_board.numOfBlackRooks++;
		}
		else if (strcmp(type, KNIGHT) == 0){
			game_board.board[node.row][node.column] = BLACK_N;
			game_board.numOfBlackKnights++;
		}
		else if (strcmp(type, BISHOP) == 0){
			game_board.board[node.row][node.column] = BLACK_B;
			game_board.numOfBlackBishops++;
		}
		else if (strcmp(type, PAWN) == 0){
			game_board.board[node.row][node.column] = BLACK_P;
			game_board.numOfBlackPawns++;
		}
		
	}
	else{
		// add white solder

		if (strcmp(type, KING) == 0){
			game_board.board[node.row][node.column] = WHITE_K;
			game_board.numOfWhiteKings++;
		}
		else if (strcmp(type, QUEEN) == 0){
			game_board.board[node.row][node.column] = WHITE_Q;
			game_board.numOfWhiteQueens++;
		}
		else if (strcmp(type, ROOK) == 0){
			game_board.board[node.row][node.column] = WHITE_R;
			game_board.numOfWhiteRooks++;
		}
		else if (strcmp(type, KNIGHT) == 0){
			game_board.board[node.row][node.column] = WHITE_N;
			game_board.numOfWhiteKnights++;
		}
		else if (strcmp(type, BISHOP) == 0){
			game_board.board[node.row][node.column] = WHITE_B;
			game_board.numOfWhiteBishops++;
		}
		else if (strcmp(type, PAWN) == 0){
			game_board.board[node.row][node.column] = WHITE_P;
			game_board.numOfWhitePawns++;
		}
	}
}

void addUserByValue(locationNode node, char value){
	game_board.board[node.row][node.column] = value;

	switch (value)
	{
	case BLACK_B:
		game_board.numOfBlackBishops++;
		break;
	case WHITE_B:
		game_board.numOfWhiteBishops++;
		break;
	case BLACK_K:
		game_board.numOfBlackKings++;
		break;
	case WHITE_K:
		game_board.numOfWhiteKings++;
		break;
	case BLACK_N:
		game_board.numOfBlackKnights++;
		break;
	case WHITE_N:
		game_board.numOfWhiteKnights++;
		break;
	case BLACK_P:
		game_board.numOfBlackPawns++;
		break;
	case WHITE_P:
		game_board.numOfWhitePawns++;
		break;
	case BLACK_Q:
		game_board.numOfBlackQueens++;
		break;
	case WHITE_Q:
		game_board.numOfWhiteQueens++;
		break;
	case BLACK_R:
		game_board.numOfBlackRooks++;
		break;
	case WHITE_R:
		game_board.numOfWhiteRooks++;
		break;
	}
	
}

void clearBoard(){
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			game_board.board[i][j] = EMPTY;
		}
	}

	game_board.numOfBlackBishops = 0;
	game_board.numOfBlackKings = 0;
	game_board.numOfBlackKnights = 0;
	game_board.numOfBlackPawns = 0;
	game_board.numOfBlackQueens = 0;
	game_board.numOfBlackRooks = 0;

	game_board.numOfWhiteBishops = 0;
	game_board.numOfWhiteKings = 0;
	game_board.numOfWhiteKnights = 0;
	game_board.numOfWhitePawns = 0;
	game_board.numOfWhiteQueens = 0;
	game_board.numOfWhiteRooks = 0;
}

void moveUser(moveList userMove){
	char type = game_board.board[userMove.origin.row][userMove.origin.column];
	removeUser(userMove.origin);
	// Add promotion
	addUserByValue(userMove.destination, type);
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

moveList* createMoveListNode(locationNode origin, locationNode dedtination, char soldierToPromoteTo){
	moveList* toReturn = myCalloc(1, sizeof(moveList));
	if (toReturn == NULL){
		return NULL;
	}
	
	toReturn->destination = dedtination;
	toReturn->origin = origin;
	toReturn->soldierToPromoteTo = soldierToPromoteTo; //TODO- check that OK
	toReturn->next = NULL;

	return toReturn;
}

locationNode createLocationNode(int colom, int row){
	locationNode toReturn;
	toReturn.column = colom;
	toReturn.row = row;
	return toReturn;
}

locationNode cloneLocationNode(locationNode loc){
	locationNode toReturn;
	toReturn.column = loc.column;
	toReturn.row = loc.row;
	return toReturn;
}

void addMoveToMoveList(moveList* head, moveList* toAdd){
	moveList* temp = head->next;
	toAdd->next = temp;
	head->next = toAdd;
}