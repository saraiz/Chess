all: Chess.exe

clean:
	-rm utils settingsStateManager minimax IO gameStateManager ChessLogic Chess
	-rm utils.o settingsStateManager.o minimax.o IO.o gameStateManager.o ChessLogic.o Chess.o


Chess.o: Chess.c utils.c settingsStateManager.c gameStateManager.c ChessLogic.c
	gcc  -c Chess.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

utils.o: utils.c Chess.c
	gcc  -c utils.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

settingsStateManager.o: settingsStateManager.c utils.c
	gcc  -c settingsStateManager.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

minimax.o: minimax.c utils.c gameStateManager.c
	gcc  -c minimax.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

IO.o: IO.c utils.c utils.c
	gcc  -c IO.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

gameStateManager.o: gameStateManager.c utils.c ChessLogic.c
	gcc  -c gameStateManager.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

ChessLogic.o: ChessLogic.c utils.c
	gcc  -c ChessLogic.c -lm -std=c99 -pedantic-errors -g `sdl-config --libs`


Chess.exe : utils.o settingsStateManager.o minimax.o IO.o gameStateManager.o ChessLogic.o Chess.o
	gcc  -std=c99 -pedantic-errors -o -Wall -g -lm Chess.exe `sdl-config --cflags`
