#include "IO.h"         

fileData loadGame(char* path){ //if error everithing is -1
	fileData toReturn = { -1, -1, -1, -1 };
	FILE* f = fopen(path, "r");
	if (f == NULL){
		return toReturn;
	}
	fscanf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>");
	char StrParser[100];
	fscanf(f, "\t<next_turn>%s", StrParser);
	toReturn.isNextBlack = strstr(StrParser, "black") != NULL ? 1 : 0;

	int intParser = 0;
	fscanf(f, "\t<game_mode>%s", StrParser);
	toReturn.gameMode = StrParser[0]-48;

	fscanf(f, "\t<difficulty>%s", StrParser);
	if (strstr(StrParser, "best") != NULL){
		toReturn.difficulty = -1;
	}
	else{
		toReturn.difficulty = StrParser[0] - 48;
	}

	fscanf(f, "\t<user_color>%s", StrParser);
	toReturn.isUserColorBlack = strstr(StrParser, "black") != NULL ? 1 : 0;
	fscanf(f,"\t<board>");

	int row;
	for (row = BOARD_SIZE - 1; row > -1; row--){
		fscanf(f, "\t\t<row_%d>%s",&intParser, StrParser);
		int coloumn;
		for (coloumn = 0; coloumn < BOARD_SIZE; coloumn++){
			locationNode loc = createLocationNode(coloumn, row);
			char content = StrParser[coloumn] == '_' ? EMPTY : StrParser[coloumn];
			addUserByValue(loc, content);
		}
	}
	
	fclose(f);
	return toReturn;
}

int saveGame(fileData toSave, char* path){ //ret 1 if failure 
	FILE* f = fopen(path, "w");
	if (f == NULL){
		return 1;
	}
	int ispVc = toSave.gameMode == 2;

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