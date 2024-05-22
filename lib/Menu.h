#pragma once

#include "Board.h"
#include "Button.h"
#include <fstream>
#include <sstream>
#include <iomanip>
class Menu {
public:
	Menu(SDL_Renderer* renderer);
	~Menu();

	void renderMainMenu(SDL_Renderer* renderer);
	void renderGameOver(SDL_Renderer* renderer);
	void renderPauseMenu(SDL_Renderer* renderer);
	void renderAuthMenu(SDL_Renderer* renderer);
	void renderLeaderboard(SDL_Renderer* renderer);

	void renderRegistration(SDL_Renderer* renderer);
	void renderLogin(SDL_Renderer* renderer);

	void hoverButtonCheck();
	int clickButtonCheck();
	void drawTextField(SDL_Renderer* renderer, SDL_Rect *fieldRect);
	void writeText(SDL_Renderer* renderer, std::string text, int x, int y);
	enum button { none, start, quit, resume, pause, registration, login, leaderboard, back };
	enum MenuType { AUTH_MENU, MAIN_MENU, PAUSE_MENU, GAME_OVER_MENU, REGISTRATION, LOGIN, LEADERBOARD };
	
private:
	Board board;
	Button restartButton;
	Button startButton;
	Button quitButton;
	Button resumeButton;
	Button pauseButton;
	Button regButton;
	Button loginButton;
	Button qButton;
	Button leaderboardButton;
	Button backButton;

	SDL_Rect fieldRect;
	int hover;
	int cursorX, cursorY;
	MenuType currentMenu;
};

