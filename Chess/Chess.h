#ifndef CHESS
#define CHESS
#include "utils.h"
#include "settingsStateManager.h"
#include "gameStateManager.h"


int isLocationValid(locationNode node, int isPrintMessage);
int isBoardValidAfterSet(char type, char *color, int isShowMessage);
int isBoardValidToStartGame(int isShowMessage);


#endif

