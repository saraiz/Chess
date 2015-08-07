#ifndef SETTINGS
#define SETTINGS
#include "utils.h"

void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
actionSummery readSettings();
actionSummery executeSettings(char* input);
actionSummery checkForGameMode(char *input);
actionSummery checkForDifficulty(char *input);
actionSummery checkForUserColor(char* input);
actionSummery checkForLoad(char *input);
actionSummery checkForClear(char *input);
actionSummery checkForNextPlayer(char *input);
actionSummery checkForRemove(char *input);
actionSummery checkForSet(char* input);
actionSummery checkForPrint(char* input);
actionSummery checkForStart(char* input);
void setSettings(char* input);
char* getNextLine(char* input);

#endif
