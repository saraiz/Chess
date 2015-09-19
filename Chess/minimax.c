#include "minimax.h"

int getBoardScoreOld(int isBlack){
	if (isMate(isBlack, 0) == 1){
		return LOSSING_SCORE;
	}
	else if (isMate(1-isBlack, 0) == 1){
		// The opponent is lossing, therfore I'm winning
		return WINNING_SCORE;
	}
	else if (isTie(1-isBlack, 0)==1){
		return TIE_SCORE;
	}
	else{
		int currentUserScore = PAWN_SCORE * game_board.numOfBlackPawns +
			KNIGHT_SCORE * game_board.numOfBlackKnights +
			BISHOP_SCORE * game_board.numOfBlackBishops +
			ROOK_SCORE * game_board.numOfBlackRooks +
			QUEEN_SCORE * game_board.numOfBlackQueens +
			KING_SCORE * game_board.numOfBlackKings;

		int opponentUserScore = PAWN_SCORE * game_board.numOfWhitePawns +
			KNIGHT_SCORE * game_board.numOfWhiteKnights +
			BISHOP_SCORE * game_board.numOfWhiteBishops +
			ROOK_SCORE * game_board.numOfWhiteRooks +
			QUEEN_SCORE * game_board.numOfWhiteQueens +
			KING_SCORE * game_board.numOfWhiteKings;

		if (!isBlack){
			// The current User is white
			int temp = currentUserScore;
			currentUserScore = opponentUserScore;
			opponentUserScore = temp;
		}

		int finalScore = currentUserScore - opponentUserScore;

		return finalScore;
	}
}

int getBoardScore(int isCurrentPlayerBlack, int isMinmaxForBlack){
	if ((isMinmaxForBlack != isCurrentPlayerBlack) && isMate(isMinmaxForBlack, 0) == 1){
		// if it's the turn of the user whom the minmax is for, there is no need to check for loose. 
		return LOSSING_SCORE;
	}
	else if ((isMinmaxForBlack == isCurrentPlayerBlack) && isMate(1 - isMinmaxForBlack, 0) == 1){
		// The opponent is lossing, therfore I'm winning
		// if it's the opponent turn there is no need to check for winning. 
		return WINNING_SCORE;
	}
	else if (isTie(1 - isCurrentPlayerBlack, 0) == 1){
		return TIE_SCORE;
	}
	else{
		int currentUserScore = PAWN_SCORE * game_board.numOfBlackPawns +
			KNIGHT_SCORE * game_board.numOfBlackKnights +
			BISHOP_SCORE * game_board.numOfBlackBishops +
			ROOK_SCORE * game_board.numOfBlackRooks +
			QUEEN_SCORE * game_board.numOfBlackQueens +
			KING_SCORE * game_board.numOfBlackKings;

		int opponentUserScore = PAWN_SCORE * game_board.numOfWhitePawns +
			KNIGHT_SCORE * game_board.numOfWhiteKnights +
			BISHOP_SCORE * game_board.numOfWhiteBishops +
			ROOK_SCORE * game_board.numOfWhiteRooks +
			QUEEN_SCORE * game_board.numOfWhiteQueens +
			KING_SCORE * game_board.numOfWhiteKings;

		if (!isMinmaxForBlack){
			// The current User is white
			int temp = currentUserScore;
			currentUserScore = opponentUserScore;
			opponentUserScore = temp;
		}

		int finalScore = currentUserScore - opponentUserScore;

		return finalScore;
	}
}

/* for get_score the minmax function will change the allPossibleMoves to be a list of one move (the wanted move)
for get_best_move the minmax function is returning a list of steps and not 1 step
*/
minmaxValue minmax(gameBoard backup, 
					int depth, 
					int isMaximizingPlayer, 
					int alpha, 
					int betha, 
					int isMinMaxForBlack, 
					int isGetScore, 
					moveList move, 
					int isGetBest, int isFirstIteration){
	// if isMaximizingPlayer == 1- its the computer turn. the color is the oposit than the user's color
	int isBlack = isMinMaxForBlack;
	if (isMaximizingPlayer == 0){
		isBlack = 1 - isMinMaxForBlack;
	}

	moveList* allPossibleMoves;
	int isListEmpty = 0;
	if (depth != 0){
		// Not asking for possible moves if the depth is 0 because the recursion will stop now
		if (isGetScore == 1){
			// this is the get_score function 
			isGetScore = 0;
			allPossibleMoves = createMoveListNode(createLocationNode(move.origin.column, move.origin.row), createLocationNode(move.destination.column, move.destination.row), move.soldierToPromoteTo);
		}
		else{
			allPossibleMoves = getAllValidMoves(isBlack, 0);
		}
		 isListEmpty = isEmptyMoveList(allPossibleMoves);
	}
	
	int bestValue;
	moveList bestMove;
	moveList *bestMovesHeadList;
	moveList *bestMovesTailList;
	int isMovesListEmpty = 1;

	if (depth == 0 || isListEmpty){
		minmaxValue value;
		// For the isCurrPlayerBlack I send 1-isBlack because the player that did the last step is the not the current one.
		value.score = getBoardScore(1-isBlack, isMinMaxForBlack); 
		value.bestMove.origin.row = value.bestMove.origin.column = value.bestMove.destination.row = value.bestMove.destination.column = -1;
		if (depth != 0){
			freeAllMoveList(allPossibleMoves);
		}
		return value;
	}
	moveList* current = allPossibleMoves;
	minmaxValue finalResult;
	if (isMaximizingPlayer){
		bestValue = -99999;

		while (current != NULL){
			moveUser(*current, isMinMaxForBlack);
			minmaxValue result = minmax(getCurrentBoardData(), depth - 1, 0, alpha, betha, isMinMaxForBlack, isGetScore, move, isGetBest, 0);
			if (result.score > bestValue){
				bestValue = result.score;
				bestMove = *current;

				if (isGetBest && isFirstIteration){
					// need to free the best moves arr
					if (!isMovesListEmpty){
						freeAllMoveList(bestMovesHeadList);
					}

					bestMovesHeadList = createMoveListNode(createLocationNode(current->origin.column, current->origin.row), createLocationNode(current->destination.column, current->destination.row), current->soldierToPromoteTo);
					isMovesListEmpty = 0;
					if (bestMovesHeadList == NULL){
						finalResult.bestMovesList = NULL;
						return finalResult;
					}

					bestMovesTailList = bestMovesHeadList;
				}
			}
			else if (isGetBest && isFirstIteration &&  result.score == bestValue){
				// need to add the move to the best moves list
				bestMovesTailList->next = createMoveListNode(createLocationNode(current->origin.column, current->origin.row), createLocationNode(current->destination.column, current->destination.row), current->soldierToPromoteTo);
				if (bestMovesTailList->next == NULL){
					finalResult.bestMovesList = NULL;
					return finalResult;
				}
				
				bestMovesTailList = bestMovesTailList->next;
				bestMovesTailList->next = NULL;
			}

			//alpha = max(alpha, bestValue);
			if (bestValue > alpha){
				alpha = bestValue;
			}

			// delete last changes
			restorBoardData(backup);

			if (betha < alpha){
				// PRUNING
				break;
			}

			current = current->next;
		}


		finalResult.bestMove = bestMove;
		if (isGetBest && isFirstIteration){
			finalResult.bestMovesList = bestMovesHeadList;
		}

		finalResult.score = bestValue;
		freeAllMoveList(allPossibleMoves);

		return finalResult;
	}
	else{
		bestValue = 99999;

		while (current != NULL){
			moveUser(*current, 1-isMinMaxForBlack);
			minmaxValue result = minmax(getCurrentBoardData(), depth - 1, 1, alpha, betha, isMinMaxForBlack, isGetScore, move, isGetBest, 0);
			if (result.score < bestValue){
				bestValue = result.score;
				bestMove = *current;
			}
			//betha = min(betha, bestValue);
			if (bestValue < betha){
				betha = bestValue;
			}

			// delete last changes
			restorBoardData(backup);

			if (betha < alpha){
				// PRUNING
				break;
			}

			current = current->next;
		}


		finalResult.bestMove = bestMove;
		finalResult.score = bestValue;
		freeAllMoveList(allPossibleMoves);
		return finalResult;
	}
}

gameBoard getCurrentBoardData(){
	gameBoard backup;
	for (int col = 0; col < BOARD_SIZE; col++){
		for (int row = 0; row < BOARD_SIZE; row++){
			backup.board[row][col] = game_board.board[row][col];
		}
	}

	backup.numOfBlackBishops = game_board.numOfBlackBishops;
	backup.numOfBlackKings = game_board.numOfBlackKings;
	backup.numOfBlackKnights = game_board.numOfBlackKnights;
	backup.numOfBlackPawns = game_board.numOfBlackPawns;
	backup.numOfBlackQueens = game_board.numOfBlackQueens;
	backup.numOfBlackRooks = game_board.numOfBlackRooks;

	backup.numOfWhiteBishops = game_board.numOfWhiteBishops;
	backup.numOfWhiteKings = game_board.numOfWhiteKings;
	backup.numOfWhiteKnights = game_board.numOfWhiteKnights;
	backup.numOfWhitePawns = game_board.numOfWhitePawns;
	backup.numOfWhiteQueens = game_board.numOfWhiteQueens;
	backup.numOfWhiteRooks = game_board.numOfWhiteRooks;


	return backup;
}

void restorBoardData(gameBoard backUp){

	for (int col = 0; col < BOARD_SIZE; col++){
		for (int row = 0; row < BOARD_SIZE; row++){
			game_board.board[row][col] = backUp.board[row][col];
		}
	}

	game_board.numOfBlackBishops = backUp.numOfBlackBishops;
	game_board.numOfBlackKings = backUp.numOfBlackKings;
	game_board.numOfBlackKnights = backUp.numOfBlackKnights;
	game_board.numOfBlackPawns = backUp.numOfBlackPawns;
	game_board.numOfBlackQueens = backUp.numOfBlackQueens;
	game_board.numOfBlackRooks = backUp.numOfBlackRooks;

	game_board.numOfWhiteBishops = backUp.numOfWhiteBishops;
	game_board.numOfWhiteKings = backUp.numOfWhiteKings;
	game_board.numOfWhiteKnights = backUp.numOfWhiteKnights;
	game_board.numOfWhitePawns = backUp.numOfWhitePawns;
	game_board.numOfWhiteQueens = backUp.numOfWhiteQueens;
	game_board.numOfWhiteRooks = backUp.numOfWhiteRooks;

}

int getBestDepth(){
	int maxPawnSteps = 12;
	int maxBishopSteps = 13;
	int maxRookSteps = 14;
	int maxKnightSteps = 8;
	int maxQueenSteps = 27;
	int maxKingSteps = 8;

	int maxChildrenInFirstLevel_white = game_board.numOfWhitePawns * maxPawnSteps +
										game_board.numOfWhiteBishops * maxBishopSteps +
										game_board.numOfWhiteRooks * maxRookSteps +
										game_board.numOfWhiteKnights * maxKnightSteps +
										game_board.numOfWhiteQueens * maxQueenSteps +
										game_board.numOfWhiteKings * maxKingSteps;

	int maxChildrenInFirstLevel_black = game_board.numOfBlackPawns * maxPawnSteps +
										game_board.numOfBlackBishops * maxBishopSteps +
										game_board.numOfBlackRooks * maxRookSteps +
										game_board.numOfBlackKnights * maxKnightSteps +
										game_board.numOfBlackQueens * maxQueenSteps +
										game_board.numOfBlackKings * maxKingSteps;

	int maxChildren = maxChildrenInFirstLevel_white > maxChildrenInFirstLevel_black ? maxChildrenInFirstLevel_white : maxChildrenInFirstLevel_black;
	// in height = 1 the maximum amount of children (possible steps) is maxChildren. 
	// for height > 1 the number of soldiers can only decrease therefore the maxChildren can only decrease. 
	// So we can use this as an upper bound.
	int numOfBoards = 1;
	int childrenInLastHeight = 1;
	int depth = 0;
	int isNeedToContinue = 1;

	while (isNeedToContinue){
		if (numOfBoards + (childrenInLastHeight * maxChildren) < MAX_BOARDS){
			++depth;	
			numOfBoards += childrenInLastHeight * maxChildren;
			childrenInLastHeight = childrenInLastHeight * maxChildren;
		}
		else{
			isNeedToContinue = 0;
		}
	}


	return depth;
}



