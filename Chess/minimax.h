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
#define WINNING_SCORE 600
#define TIE_SCORE 500

#define MAX_BOARDS 1000000

typedef struct minmaxValue{
	int score;
	moveList bestMove;
}minmaxValue;

int getBoardScoreOld(int isBlack);
int getBoardScore(int isCurrentPlayerBlack, int isMinmaxForBlack);
minmaxValue minmax(gameBoard backup,
	int depth,
	int isMaximizingPlayer,
	int alpha,
	int betha,
	int isMinMaxForBlack,
	int isGetScore,
	moveList move);
gameBoard getCurrentBoardData();
void restorBoardData(gameBoard backUp);
int getBestDepth();


#endif
