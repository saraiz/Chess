#ifndef GAME_ACTIONS
#define GAME_ACTIONS

#include "utils.h"

actionSummery readGameActions();
actionSummery executeGameActions(char* input);
actionSummery checkForMove(char *input);
actionSummery checkForGetMoves(char *input);
actionSummery checkForBestMoves(char *input);
actionSummery checkForGetScore(char *input);
actionSummery checkForSave(char *input);
locationNode getNextLocation(char** loc);
int isValidMove(moveList soldierMove, int isBlack, int isShowMessage);

#endif
