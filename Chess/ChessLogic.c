#include "ChessLogic.h"

moveList* getPossibleMovesKing(locationNode loc, int isBlack){

}
moveList* getPossibleMovesQueen(locationNode loc, int isBlack){

}
moveList* getPossibleMovesRook(locationNode loc, int isBlack){
	moveList* sentinal = createMoveListNode(createLocationNode(9, 9), createLocationNode(9, 9), EMPTY);
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
	printMoveList(toReturn); //TODO delete
	return toReturn;
}

moveList* getPossibleMovesKnight(locationNode curLoc, int isBlack){
	moveList* sentinal = createMoveListNode(createLocationNode(9, 9), createLocationNode(9, 9), EMPTY);
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
	printMoveList(toReturn); //TODO delete
	return toReturn;
}

moveList* getPossibleMovesBishop(locationNode loc, int isBlack){

}

moveList* getPossibleMovesPawn(locationNode loc, int isBlack){


}

int isLocationOnBord(locationNode loc){ //TODO the same sa isLocationValid??????????????
	return -1 < loc.row && loc.row < 8 && -1 < loc.column && loc.column < 8;
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
	if (!isLocationOnBord(destenetion)){
		return 0;
	}
	if (EMPTY == getPice(destenetion) || !isSameColorAsMe(destenetion, isBlack)){
		locationNode curLocClone = cloneLocationNode(origen);

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

