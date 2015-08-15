#ifndef IO
#define IO
#include "utils.h"
#include "ChessLogic.h"

#define NUMOFSLOTS 7

int saveGame(fileData toSave, int slot);
fileData loadGame(int slot);

#endif
