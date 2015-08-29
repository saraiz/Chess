#ifndef MINIMAX
#define MINIMAX
#include "utils.h"
#include "gameStateManager.h"

#define PAWN_SCORE 1
#define KNIGHT_SCORE 3
#define BISHOP_SCORE 3
#define ROOK_SCORE 5
#define QUEEN_SCORE 9
#define KING_SCORE 400
#define LOSSING_SCORE -500
#define WINNING_SCORE 500
#define TIE_SCORE 600

typedef struct minmaxValue{
	int score;
	moveList bestMove;
	moveList *bestMovesList;
	//struct minmaxValue *next;
}minmaxValue;

int getBoardScore(int isBlack);
minmaxValue minmax(gameBoard backup,
	int depth,
	int isMaximizingPlayer,
	int alpha,
	int betha,
	int isMinMaxForBlack,
	int isGetScore,
	moveList move,
	int isGetBest,
	int isFirstIteration);
gameBoard getCurrentBoardData();
void restorBoardData(gameBoard backUp);

#endif
