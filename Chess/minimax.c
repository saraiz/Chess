#include "minimax.h"

int getBoardScore(int isBlack){
	if (isMate(isBlack, 0) == 1){
		return LOSSING_SCORE;
	}
	else if (isMate(1-isBlack, 0) == 1){
		// The opponent is lossing, therfore I'm winning
		return WINNING_SCORE;
	}
	else if (isTie(isBlack, 0)==1){
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
					int isGetBest, 
					int isFirstIteration){
	// if isMaximizingPlayer == 1- its the computer turn. the color is the oposit than the user's color
	int isBlack = isMinMaxForBlack;
	if (isMaximizingPlayer == 0){
		isBlack = 1 - isMinMaxForBlack;
	}

	moveList* allPossibleMoves;
	if (isGetScore == 1){
		// this is the get_score function 
		isGetScore = 0;
		allPossibleMoves = createMoveListNode(createLocationNode(move.origin.column, move.origin.row), createLocationNode(move.destination.column, move.destination.row), move.soldierToPromoteTo);
	}
	else{
		allPossibleMoves = getAllValidMoves(isBlack, 0);
	}
	
	int isListEmpty = isEmptyMoveList(allPossibleMoves);
	int bestValue;
	moveList bestMove;
	moveList *bestMovesList;
	int isBestMovesListEmpty = 1;

	if (depth == 0 || isListEmpty){
		minmaxValue value;
		value.score = getBoardScore(isMinMaxForBlack);
		value.bestMove.origin.row = value.bestMove.origin.column = value.bestMove.destination.row = value.bestMove.destination.column = -1;
		freeAllMoveList(allPossibleMoves);
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

				if (isGetBest == 1 && isFirstIteration == 1){
					// This is the get_best_moves function
					// need to init the best moves list with the current move 
					if (!isBestMovesListEmpty){
						freeAllMoveList(bestMovesList);
					}

					bestMovesList = createMoveListNode(createLocationNode(current->origin.column, current->origin.row), createLocationNode(current->destination.column, current->destination.row), current->soldierToPromoteTo);
					isBestMovesListEmpty = 0;
				}
			}
			else if (result.score == bestValue && isGetBest == 1 && isFirstIteration == 1){
				// This is the get_best_moves function
				// need to add the current move to the best moves list
				bestMovesList->next = createMoveListNode(createLocationNode(current->origin.column, current->origin.row), createLocationNode(current->destination.column, current->destination.row), current->soldierToPromoteTo);
			}

			//alpha = max(alpha, bestValue);
			if (bestValue > alpha){
				alpha = bestValue;
			}

			if (betha <= alpha){
				// PRUNING
				break;
			}

			// delete last changes
			restorBoardData(backup);

			current = current->next;
		}


		finalResult.bestMove = bestMove;
		if (isGetBest == 1&& isFirstIteration == 1){
			finalResult.bestMovesList = bestMovesList;
		}
		
		finalResult.score = bestValue;
		freeAllMoveList(allPossibleMoves);
		//freeAllMoveList(bestMovesList);

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

			if (betha <= alpha){
				// PRUNING
				break;
			}

			// delete last changes
			restorBoardData(backup);

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


