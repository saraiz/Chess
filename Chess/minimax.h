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

int getBoardScore(int isBlack);

#endif
