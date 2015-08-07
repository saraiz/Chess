#include "ChessLogic.h"

moveList* getPossibleMovesKing(locationNode loc, int isBlack){

}
moveList* getPossibleMovesQueen(locationNode loc, int isBlack){

}
moveList* getPossibleMovesRook(locationNode loc, int isBlack){

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

				locationNode destanation = createLocationNode(curLoc.column + horisontalShift, curLoc.row + VerticalShift);
				if (!isLocationOnBord(destanation)){
					continue;
				}
				if (EMPTY == getPice(destanation) || !isSameColorAsMe(destanation, isBlack)){
					locationNode curLocClone = cloneLocationNode(curLoc);

					moveList* toAdd = createMoveListNode(curLocClone, destanation, EMPTY);
					if (toAdd == NULL){
						freeAllMoveList(sentinal);
						return NULL;
					}
					addMoveToMoveList(sentinal, toAdd);
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
	moveList* sentinal = createMoveListNode(createLocationNode(9, 9), createLocationNode(9, 9), EMPTY);
	if (sentinal == NULL){
		return NULL;
	}
	int isOriginalLoc = isBlack ? loc.row == 6 : loc.row == 1;
	if (isOriginalLoc){

	}
	else{

	}

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
