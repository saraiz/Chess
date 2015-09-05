#ifndef ChessLogic
#define ChessLogic
#include "utils.h"
#include "gameStateManager.h"


typedef struct  {
	locationNode origen;
	char origenPice;
	locationNode destenetion;
	char destenetionPice;
}whatMoved;

moveList* getAllValidMoves(int isBlack, int depth);
moveList* getValidMovesForLocation(locationNode loc, int depth);

moveList* getPossibleMovesKing(locationNode loc, int isBlack, int depth);
moveList* getPossibleMovesQueen(locationNode loc, int isBlack, int depth);
moveList* getPossibleMovesRook(locationNode loc, int isBlack, int depth);
moveList* getPossibleMovesKnight(locationNode loc, int isBlack, int depth);
moveList* getPossibleMovesBishop(locationNode loc, int isBlack, int depth);
moveList* getPossibleMovesPawn(locationNode loc, int isBlack, int depth);


int isSameColorAsMe(locationNode loc, int isBlack);
char getPice(locationNode loc);
int CheackDeatenetionAndAdd(moveList* sentinal, locationNode origen, locationNode destenetion, int isBlack, int depth);
int CheackDeatenetionAndAdd_Pawn(moveList* sentinal, locationNode origen, locationNode destenation, int isEat, int isblack, int depth);
moveList* removeAndFreeSentinalIfNececery(moveList* sentinal);
int isLocInDestenetion(locationNode loc, moveList* head);
int amIThreatened( int isMyColorBlack);
whatMoved moveUserByLocationNode(locationNode origen, locationNode destenetion, char piceToChangeTo);
void undoWhatMoved(whatMoved toUndo);



#endif
 