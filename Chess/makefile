all: chessprog

clean:
	-rm utils settingsStateManager minimax IO gameStateManager ChessLogic Chess
	-rm utils.o settingsStateManager.o minimax.o IO.o gameStateManager.o ChessLogic.o Chess.o


Chess.o: Chess.c utils.c settingsStateManager.c gameStateManager.c ChessLogic.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Chess.c `sdl-config --cflags`

utils.o: utils.c Chess.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm utils.c `sdl-config --cflags`

settingsStateManager.o: settingsStateManager.c utils.c IO.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm settingsStateManager.c `sdl-config --cflags`

minimax.o: minimax.c utils.c gameStateManager.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm minimax.c `sdl-config --cflags`

IO.o: IO.c utils.c utils.c  ChessLogic.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm IO.c `sdl-config --cflags`

gameStateManager.o: gameStateManager.c utils.c ChessLogic.c minimax.c IO.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm gameStateManager.c `sdl-config --cflags`

ChessLogic.o: ChessLogic.c utils.c gameStateManager.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm ChessLogic.c `sdl-config --cflags`


chessprog : utils.o settingsStateManager.o minimax.o IO.o ChessLogic.o Chess.o gameStateManager.o
	gcc  -o chessprog -lm -std=c99 -pedantic-errors -g `sdl-config --libs` utils.o settingsStateManager.o minimax.o IO.o ChessLogic.o Chess.o gameStateManager.o
