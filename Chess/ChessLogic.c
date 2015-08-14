#include "ChessLogic.h"

moveList* getAllValidMoves(int isBlack){
	moveList* toReturn = EMPTYMOVELIST;
	if (toReturn == NULL){
		return NULL;
	}
	int i;
	for (i = 0; i < BOARD_SIZE; i++){
		int j;
		for (j = 0; j < BOARD_SIZE; j++){
			locationNode curLoc =  createLocationNode(i, j);
			if (getPice(curLoc)!=EMPTY && isSameColorAsMe(curLoc, isBlack)){
				moveList* move = getValidMovesForLocation(curLoc);
				toReturn = moveConcat(toReturn, move);
			}
		}
	}
	return toReturn;
}

moveList* getValidMovesForLocation(locationNode loc){ //TODO casteling
	char pice = tolower( getPice(loc));
	int isBlack = isSameColorAsMe(loc, 1);
	switch (pice)
	{
	case 'm':
		return getPossibleMovesPawn(loc, isBlack);
	case 'b':
		return getPossibleMovesBishop(loc, isBlack);
	case 'r':
		return getPossibleMovesRook(loc, isBlack);
	case 'n':
		return getPossibleMovesKnight(loc, isBlack);
	case 'q':
		return getPossibleMovesQueen(loc, isBlack);
	case 'k':
		return getPossibleMovesKing(loc, isBlack);
	default: //EMPTY
		return EMPTYMOVELIST;
		}
}

moveList* getPossibleMovesKing(locationNode loc, int isBlack){
	moveList* sentinal = EMPTYMOVELIST;
	if (sentinal == NULL){
		return NULL;
	}
	int horisontalDiff;
	for (horisontalDiff = -1; horisontalDiff < 2; horisontalDiff++){
		int verticalDiff;
		for (verticalDiff = -1; verticalDiff < 2; verticalDiff++){
			if (verticalDiff == 0 && horisontalDiff == 0){
				continue;
			}
			int destRow = loc.row + verticalDiff;
			int destColumn = loc.column + horisontalDiff;
			locationNode destenetion = createLocationNode(destColumn, destRow);

			if (2 == CheackDeatenetionAndAdd(sentinal, loc, destenetion, isBlack)){
				return NULL;
			}

		}
	}
	moveList* toReturn = removeAndFreeSentinalIfNececery(sentinal);
	return toReturn;
}

moveList* getPossibleMovesQueen(locationNode loc, int isBlack){
	moveList* bishop = getPossibleMovesBishop(loc, isBlack);
	moveList* rook = getPossibleMovesRook(loc, isBlack);
	return moveConcat(bishop, rook);
}

moveList* getPossibleMovesRook(locationNode loc, int isBlack){
	moveList* sentinal = EMPTYMOVELIST;
	if (sentinal == NULL){
		return NULL;
	}
	int Direction; //-1 go left/ down, +1 go right/ up
	for (Direction = -1; Direction < 2; Direction += 2){
		int isHorisontal; // 0 for vertical, 1 for horisontal
		for (isHorisontal = 0; isHorisontal < 2; isHorisontal += 1){  
			int numOfSteps = 1;
			int isAdded;
			locationNode destenetion;
			do
			{
				int Diff = Direction*numOfSteps;
				int destRow = loc.row + Diff*(1-isHorisontal);
				int destColumn = loc.column + Diff*(isHorisontal);
				destenetion = createLocationNode(destColumn, destRow);
				isAdded = CheackDeatenetionAndAdd(sentinal, loc, destenetion, isBlack);
				if (2 == isAdded){
					return NULL;
				}

				numOfSteps++;
			} while (isAdded == 1 && getPice(destenetion) == EMPTY);
		}
	}
	moveList* toReturn = removeAndFreeSentinalIfNececery(sentinal);
	return toReturn;
}

moveList* getPossibleMovesKnight(locationNode curLoc, int isBlack){
	moveList* sentinal = EMPTYMOVELIST;
	if (sentinal == NULL){
		return NULL;
	}
	int HorisontalDirection; //-1 go left, +1 go right
	for (HorisontalDirection = -1; HorisontalDirection < 2; HorisontalDirection += 2){
		int VerticalDiraction; //-1 down, 1 up
		for (VerticalDiraction = -1; VerticalDiraction < 2; VerticalDiraction+=2){
			int numOfHorisontalSteps; //num of steps we go right or left
			for (numOfHorisontalSteps = 1; numOfHorisontalSteps < 3; numOfHorisontalSteps+=1){
				int numOfVerticalStaps = 3 - numOfHorisontalSteps; //num of steps we go up or down

				int horisontalShift = HorisontalDirection*numOfHorisontalSteps;
				int VerticalShift = VerticalDiraction*numOfVerticalStaps;

				//printf("VS:%d, HS:%d\n", VerticalShift, horisontalShift);
				
				locationNode destenetion = createLocationNode(curLoc.column + horisontalShift, curLoc.row + VerticalShift);

				if (2 == CheackDeatenetionAndAdd(sentinal, curLoc, destenetion, isBlack)){
					return NULL;
				}
			}
		}
	}
	moveList* toReturn = removeAndFreeSentinalIfNececery(sentinal);
	return toReturn;
}

moveList* getPossibleMovesBishop(locationNode loc, int isBlack){
	moveList* sentinal = EMPTYMOVELIST;
	if (sentinal == NULL){
		return NULL;
	}
	int HorisontalDirection; //-1 go left, +1 go right
	for (HorisontalDirection = -1; HorisontalDirection < 2; HorisontalDirection += 2){
		int VerticalDiraction; //-1 down, 1 up
		for (VerticalDiraction = -1; VerticalDiraction < 2; VerticalDiraction += 2){
			int numOfSteps = 1;
			int isAdded;
			locationNode destenetion;
			do
			{
				int destRow = loc.row + numOfSteps*VerticalDiraction;
				int destColumn = loc.column + numOfSteps*HorisontalDirection;
				destenetion = createLocationNode(destColumn, destRow);
				isAdded = CheackDeatenetionAndAdd(sentinal, loc, destenetion, isBlack);
				if (2 == isAdded){
					return NULL;
				}

				numOfSteps++;
			} while (isAdded == 1 && getPice(destenetion) == EMPTY);
		}
	}
	moveList* toReturn = removeAndFreeSentinalIfNececery(sentinal);
	return toReturn;
}

moveList* getPossibleMovesPawn(locationNode loc, int isBlack){
	moveList* sentinal = EMPTYMOVELIST;
	if (sentinal == NULL){
		return NULL;
	}
	int horisontalShift;
	int VerticalShift = isBlack ? -1 : +1;
	int destRow = loc.row + VerticalShift;

	for (horisontalShift = -1; horisontalShift < 2; horisontalShift++){
		int destColumn = loc.column + horisontalShift;
		locationNode dest = createLocationNode(destColumn, destRow);

		if (2 == CheackDeatenetionAndAdd_Pawn(sentinal, loc, dest, abs(horisontalShift), isBlack)){
			return NULL;
		}
	}
	return removeAndFreeSentinalIfNececery(sentinal);
}


int isSameColorAsMe(locationNode loc, int amIBlack){
	char pice = getPice(loc);
	int isPiceBlack = isupper(pice);
	return isPiceBlack == amIBlack;
}

char getPice(locationNode loc){
	return game_board.board[loc.row][loc.column];
}

moveList* removeAndFreeSentinalIfNececery(moveList* sentinal){
	moveList* head = sentinal->next;
	if (head == NULL){
		return sentinal;
	}
	myFree(sentinal);
	return head;
}

int CheackDeatenetionAndAdd(moveList* sentinal,locationNode origen, locationNode destenetion, int isBlack){
	//0-didnt add, 1-add,2- error (frees sentinal's list)
	if (!isLocationValid(destenetion, 0)){
		return 0;
	}
	if (EMPTY == getPice(destenetion) || !isSameColorAsMe(destenetion, isBlack)){
		locationNode curLocClone = cloneLocationNode(origen); //TODO nedded??????????

		moveList* toAdd = createMoveListNode(curLocClone, destenetion, EMPTY);
		if (toAdd == NULL){
			freeAllMoveList(sentinal);
			return 2;
		}
		addMoveToMoveList(sentinal, toAdd);
		return 1;
	}
	return 0;
}

int CheackDeatenetionAndAdd_Pawn(moveList* sentinal,locationNode origen,locationNode destenation, int isEat, int isblack){
	int isPromoten = destenation.row == (isblack ? 0 : 7);
	int isToMove=0;
	if (!isLocationValid(destenation, 0)){
		return 0;
	}
	char destPice = getPice(destenation);
	if (isEat && destPice != EMPTY && !isSameColorAsMe(destenation, isblack)){
		isToMove = 1;
	}
	else if (!isEat && destPice == EMPTY){
		isToMove = 1;
	}

	if (isToMove){
		if (isPromoten){
			moveList* toAddQueen = createMoveListNode(cloneLocationNode(origen), destenation, 'q');
			if (toAddQueen == NULL){
				freeAllMoveList(sentinal);
				return 2;
			}
			addMoveToMoveList(sentinal, toAddQueen);

			moveList* toAddBishop = createMoveListNode(cloneLocationNode(origen), destenation, 'b');
			if (toAddBishop == NULL){
				freeAllMoveList(sentinal);
				return 2;
			}
			addMoveToMoveList(sentinal, toAddBishop);

			moveList* toAddKnight = createMoveListNode(cloneLocationNode(origen), destenation,'n');
			if (toAddKnight == NULL){
				freeAllMoveList(sentinal);
				return 2;
			}
			addMoveToMoveList(sentinal, toAddKnight);

			moveList* toAddRook = createMoveListNode(cloneLocationNode(origen), destenation, 'r');
			if (toAddRook == NULL){
				freeAllMoveList(sentinal);
				return 2;
			}
			addMoveToMoveList(sentinal, toAddRook);
		}

		else{
			moveList* toAdd = createMoveListNode(origen, destenation, EMPTY);
			if (toAdd == NULL){
				freeAllMoveList(sentinal);
				return 2;
			}
			addMoveToMoveList(sentinal, toAdd);
		}
		return 1;
	}
	return 0;
} 

int amITretrnd(locationNode loc, int myColor){
	//0 no, 1 yes, 2 ERROR
	moveList* validMoves = getAllValidMoves(!myColor);
	if (validMoves == NULL){
		return 2;
	}
	return isLocInDestenetion(loc, validMoves);
}

int isLocInDestenetion(locationNode loc, moveList* head){
	for (; head != NULL; head = head->next){
		if (head->destination.column == loc.column && head->destination.row == loc.row){
			return 1;
		}
	}
	return 0;
}