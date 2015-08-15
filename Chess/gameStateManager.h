#ifndef GAME_ACTIONS
#define GAME_ACTIONS

#include "utils.h"
#include "ChessLogic.h"
#include "minimax.h"

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
int printAllPossibleMoves(moveList* moves);
int printOneMove(moveList move);
int isMate(int isBlack, int isShowMessage);
int isTie(int isBlack, int isShowMessage);
locationNode getKingLocation(int isBlack);
char* convertSoldierTypeToSoldierName(char type);
char convertSoldierNameToSoldierType(char *name, int isBlack);
void computerTurn();
char getSoldierTypeByColor(char type, int isBlack);
void moveUser(moveList userMove, int isBlack);

#endif
