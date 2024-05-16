#pragma once

#include "Board.h"
#include "Button.h"

class Menu {
public:
	Menu(SDL_Renderer* renderer);
	~Menu();

	void renderMainMenu(SDL_Renderer* renderer);
	void renderGameOver(SDL_Renderer* renderer);
	void renderPauseMenu(SDL_Renderer* renderer);
	void hoverButtonCheck();
	int clickButtonCheck();
	enum button { none, start, quit, resume, pause };
	enum MenuType { MAIN_MENU, PAUSE_MENU, GAME_OVER_MENU };

private:
	Board board;
	Button restartButton;
	Button startButton;
	Button quitButton;
	Button resumeButton;
	Button pauseButton;
	int hover;
	int cursorX, cursorY;
	MenuType currentMenu;
};

