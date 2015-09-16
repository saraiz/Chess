all: chessprog

clean:
	-rm utils.o settingsStateManager.o minimax.o IO.o ChessLogic.o Chess.o gameStateManager.o guiUtils.o guiBoard.o GuiSettingsStateManager.o chessprog


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

ChessLogic.o: ChessLogic.c ChessLogic.c utils.c gameStateManager.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm ChessLogic.c `sdl-config --cflags`

guiBoard.o: guiBoard.c ChessLogic.c utils.c gameStateManager.c guiUtils.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm guiBoard.c `sdl-config --cflags`

guiUtils.o: guiUtils.c utils.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm guiUtils.c `sdl-config --cflags`

GuiSettingsStateManager.o: GuiSettingsStateManager.c utils.c ChessLogic.c IO.c ChessLogic.c minimax.o
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm GuiSettingsStateManager.c `sdl-config --cflags`



chessprog : utils.o settingsStateManager.o minimax.o IO.o ChessLogic.o Chess.o gameStateManager.o guiUtils.o guiBoard.o GuiSettingsStateManager.o
	gcc  -o chessprog -lm -std=c99 -pedantic-errors -g `sdl-config --libs` utils.o settingsStateManager.o minimax.o IO.o ChessLogic.o Chess.o gameStateManager.o guiUtils.o guiBoard.o GuiSettingsStateManager.o
