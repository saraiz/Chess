#ifndef IO
#define IO
#include "utils.h"
#include "ChessLogic.h"

#define NUMOFSLOTS 7

int saveGame(fileData toSave, char* path);
fileData loadGame(char* path);


#endif
