#ifndef ChessLogic
#define ChessLogic
#include "utils.h"

moveList* getAllValidMoves(int isBlack);
moveList* getValidMovesForLocation(locationNode loc);

moveList* getPossibleMovesKing(locationNode loc, int isBlack);
moveList* getPossibleMovesQueen(locationNode loc, int isBlack);
moveList* getPossibleMovesRook(locationNode loc, int isBlack);
moveList* getPossibleMovesKnight(locationNode loc, int isBlack);
moveList* getPossibleMovesBishop(locationNode loc, int isBlack);
moveList* getPossibleMovesPawn(locationNode loc, int isBlack);


int isLocationOnBord(locationNode loc);
int isSameColorAsMe(locationNode loc, int isBlack);
char getPice(locationNode loc);
int CheackDeatenetionAndAdd(moveList* sentinal, locationNode origen, locationNode destenetion, int isBlack);


#endif
 