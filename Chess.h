//#ifndef CHESS_
#define CHESS_

#include<stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'


#define EMPTY ' '

#define BOARD_SIZE 8

// board initialization
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
 
#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
 
#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));

#define BEST -1
#define TWO_PLAYERS 1
#define PLAYER_VS_AI 2
#define COLOR_WHITE "white"
#define COLOR_BLACK "black"

typedef struct {
	unsigned int minmax_depth;
	int userColor;
	int isUserBlack;
	int isGUI;
	int gameMode;
}gameSettings;



typedef struct {
	int numOfBlackKings;
	int numOfWhiteKings;

	int numOfBlackQueens;
	int numOfWhiteQueens;

	int numOfBlackRooks;
	int numOfWhiteRooks;

	int numOfBlackKnights;
	int numOfWhiteKnights;

	int numOfBlackBishops;
	int numOfWhiteBishops;

	int numOfBlackPawns;
	int numOfWhitePawns;

	int isBlackTurn;
	char board[BOARD_SIZE][BOARD_SIZE];
}gameBoard;

struct list {
	int column; //numeric postion in the board (not letters) i
	int row; //numeric postion in the board (not letters) j
	struct list* next;
};

typedef struct list locationNode;

typedef struct {
	char column;
	int row;
}locationInLetters;

//struct move {
//int numOfManCupture;
//int numOfKingCupture;
//locationNode* path;
//struct move* next;
//};

//typedef struct move move;

typedef struct {
	int isFound;
	int isExecuted;
	int isError;
	char failedFunc[50];
}actionSummery;

gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
actionSummery readSettings();
void getInput(char** inputPointer);
actionSummery executeSettings(char* input);
locationNode convertBoardLocationToNumericLocation(char column, int row);
locationInLetters convertNumericLocationTpBoardLocation(int column, int row);
char* getNextChar(char* str);
char* getSubString(const char* input, const  char* Loc);
int isLocationValid(locationNode node, int isPrintMessage);
int isBoardValidAfterSet(char *type, char *color, int isShowMessage);
int isBoardValidToStartGame(int isShowMessage);
actionSummery checkForGameMode(char *input);
actionSummery checkForDifficulty(char *input);
actionSummery checkForUserColor(char* input);
actionSummery checkForLoad(char *input);
actionSummery checkForClear(char *input);
actionSummery checkForNextPlayer(char *input);
actionSummery checkForRemove(char *input);
actionSummery checkForSet(char* input);
actionSummery checkForPrint(char* input);
actionSummery checkForStart(char* input);

/*#endif CHESS_*/