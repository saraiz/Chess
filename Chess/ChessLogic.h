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


int isSameColorAsMe(locationNode loc, int isBlack);
char getPice(locationNode loc);
int CheackDeatenetionAndAdd(moveList* sentinal, locationNode origen, locationNode destenetion, int isBlack);
int CheackDeatenetionAndAdd_Pawn(moveList* sentinal, locationNode origen, locationNode destenation, int isEat, int isblack);
moveList* removeAndFreeSentinalIfNececery(moveList* sentinal);
int amITretrnd(locationNode loc, int myColor);
int amIThreatened(locationNode loc, int isMyColorBlack);



#endif
 