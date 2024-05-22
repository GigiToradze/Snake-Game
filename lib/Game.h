#pragma once

#include "Menu.h"
#include "Board.h"
#include "Snake.h"
#include "Fruit.h"
#include "User.h"

#include <fstream>

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, int flag);

	void handleMainMenuEvents();

	void handleEvents();
	void update();
	void render();
	void clean();

	void handleGameOverEvents();

	enum GameState { AUTH_MENU, MAIN_MENU, IN_GAME, GAME_OVER, PAUSE_MENU, REGISTRATION, LOGIN, LEADERBOARD};

	void setGameState(int initialState) { gameState = initialState; }
	int getGameState() const { return gameState; }

	int setScore(int& score);
	int getScore() const { return score; }

	bool running() const { return isRunning; }
	SDL_Renderer* getRenderer() { return renderer; }

	bool getPause() const { return pause; }
	void setPause(const bool& currentPause) { pause = currentPause; }
	bool ableToSetNewDirection;
	void handlePauseMenuEvents();
	void handleAuthMenuEvents();

	void handleRegistrationEvents();
	void handleLoginEvents();
	void handleLeaderboardEvents();
private:
	int gameState;
	int previousMenu;
	bool isRunning;
	int score;
	bool pause;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Menu gameOverMenu;
	Menu mainMenu;
	Menu pauseMenu;
	Menu authMenu;
	Menu regMenu;
	Menu loginMenu;
	Menu leaderboardMenu;

	Button button;
	Button submit;
	Board board;
	Fruit fruit;
	Snake snake;

	User currentUser;
};
