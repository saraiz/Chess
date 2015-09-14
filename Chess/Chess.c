#include "Chess.h"
#include <SDL.h>
#include <SDL_video.h>

#define WIN_W 800
#define WIN_H 600
#define IMG_W 240
#define IMG_H 296


gameSettings settings = { 1, COLOR_WHITE, 0, 0, TWO_PLAYERS };
gameBoard game_board = { 0, 0, 0, 0, 0 };

int main(int argc, char* argv[])
{

	//SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_Quit();

	// If you run your code in nova you should put this lines in comment
	argc = 2;
	argv[1] = "gui";
	////////////////////////////////////////////////////////////////////

	init_board(game_board.board);

	if (argc == 2){
		char* mode = argv[1];
		if (strcmp(mode, "console") == 0){
			handleConsole();
		}
		else if (strcmp(mode, "gui") == 0){
			handleGUI();
		}
	}
	return 1;
}

void handleConsole(){

	setSettings("clear");
	setSettings("game_mode 1");
	setSettings("next_player white");
	setSettings("set <b,8> black king");
	setSettings("set <e,7> white queen");
	setSettings("set <d,6> white knight");
	setSettings("set <b,6> white rook");
	setSettings("set <h,1> white king");

	// Don't delete me. We need to print the board in the begining og the game according to the instructions. 
	print_board(game_board.board);

	actionSummery summery = readSettings();
	if (summery.isError){
		print_error_message(summery.failedFunc);
	}
	else{
		if (summery.isExecuted == 1){
			// Need to start the game
			//printAllPossibleMoves( getAllValidMoves(1,0)); //TODO delete
			//print_board(game_board.board); //TODO delete
			summery = readGameActions();
			if (summery.isError){
				print_error_message(summery.failedFunc);
			}
			else{
				// game ended because: someone won/it's a tie/quit
				// Do we need to do something? 
			}

		}
	}
	int extern countAlloc;//TODO delete
	printf("%d", countAlloc);//TODO delete
	getchar(); //TODO delete
}

void handleGUI2(){
	//SDL_WM_SetCaption("title1", "title2");
	SDL_Event e;
	SDL_Rect rect = { 0, 0, 400, 400 };
	SDL_Rect imgrect = { 0, 0, 400, 400 };
	SDL_Surface *img = SDL_LoadBMP("test.bmp");
	SDL_Surface *w = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0;

	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	/* Create window surface*/

	if (w == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	/* Define the rects we need*/


	/* Load test spritesheet image*/

	if (img == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return 1;
	}

	/* Set colorkey to BLUE*/
	/*if (SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 0, 0, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		return 1;
	}*/


	while (!quit) {
		/* Clear window to BLACK*/
		if (SDL_FillRect(w, 0, 0xFFFFFF) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}

		/* Green rectangle button*/
		if (SDL_FillRect(w, 0, SDL_MapRGB(w->format, 0, 255, 0)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}

		SDL_Rect destRec = { 0, 200, 400, 400 };

		/* Draw image sprite*/
		if (SDL_BlitSurface(img, NULL, w, &destRec) != 0) {
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			break;
		}

		/* Advance to next sprite*/
		/*imgrect.x += imgrect.w;
		if (imgrect.x >= img->w) {
			imgrect.x = 0;
			imgrect.y += imgrect.h;
			if (imgrect.y >= img->h) imgrect.y = 0;
		}*/

		/* We finished drawing*/
		if (SDL_Flip(w) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONUP) :
				if ((e.button.x > destRec.x) && (e.button.x < destRec.x + destRec.w) && (e.button.y > destRec.y) && (e.button.y < destRec.y + destRec.h))
					quit = 1;
				break;
			default:
				break;
			}
		}

		/* Wait a little before redrawing*/
		SDL_Delay(1000);
	}

	SDL_FreeSurface(img);
}

void handleGUI(){
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	buildSettingsWindow();
	
	
}








