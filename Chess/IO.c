#include "IO.h"         

fileData loadGame(int slot){

}

int saveGame(fileData toSave, int slot){ //ret 1 if failure
	assert(slot > 0 && slot <= NUMOFSLOTS);
	char fileName[] = "slot no  .xml";
	fileName[8] = slot + 48; // convet to ascii
	int ispVc = toSave.gameMode == 2;
	FILE* f = fopen(fileName, "w");
	if (f == NULL){
		return 1;
	}
	char* line = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n";
	fputs(line, f);

	line = "\t<next_turn>%s</next_turn>\n";
	char* nextPlyer = toSave.isNextBlack ? "black" : "white";
	fprintf(f, line, nextPlyer);

	line = "\t<game_mode>%d</game_mode>\n";
	fprintf(f, line, toSave.gameMode);

	if (ispVc){
		if (toSave.difficulty != -1){
			line = "\t<difficulty>%d</difficulty>\n";
			fprintf(f, line, toSave.difficulty);
		}
		else{
			line = "\t<difficulty>best</difficulty>\n";
			fputs(line,f);
		}


		line = "\t<user_color>%s</user_color>\n";
		char* userColor = toSave.isUserColorBlack ? "black" : "white";
		fprintf(f, line, userColor);

	}
	else{
		line = "\t<difficulty></difficulty>\n\t<user_color></user_color>\n";
		fputs(line, f);
	}

	fputs("\t<board>\n",f);
	line = "\t\t<row_%d>%s</row_%d>\n";
	int row;
	for (row = BOARD_SIZE-1; row > -1; row--){
		char rowCont[BOARD_SIZE + 1];
		rowCont[BOARD_SIZE] = 0;
		int coloumn;
		for (coloumn = 0; coloumn < BOARD_SIZE; coloumn++){
			locationNode loc = createLocationNode(coloumn, row);
			rowCont[coloumn] = getPice(loc) == EMPTY ? '_' : getPice(loc);

		}
	fprintf(f, line, row + 1, rowCont, row + 1);
	}


	fputs("\t</board>\n</game>",f);

	fclose(f);
	return 0;
}