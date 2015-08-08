#ifndef GAME_ACTIONS
#define GAME_ACTIONS

#include "utils.h"
#include "ChessLogic.h"

actionSummery readGameActions();
actionSummery executeGameActions(char* input);
actionSummery checkForMove(char *input);
actionSummery checkForGetMoves(char *input);
actionSummery checkForBestMoves(char *input);
actionSummery checkForGetScore(char *input);
actionSummery checkForSave(char *input);
locationNode getNextLocation(char** loc);
int isValidMove(moveList soldierMove, int isBlack, int isShowMessage);
int isPositionContainUserPiece(int isBlack, locationNode position, int isShowMessage);
void printAllPossibleMoves(moveList* moves);
void printOneMove(moveList move);
int isCheck(int isBlack, int isShowMessage);
int isMate(int isBlack, int isShowMessage);
int isTie(int isBlack, int isShowMessage);

#endif
