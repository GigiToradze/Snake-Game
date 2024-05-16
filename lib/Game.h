#pragma once

#include "Menu.h"
#include "Board.h"
#include "Snake.h"
#include "Fruit.h"

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

	enum GameState { MAIN_MENU, IN_GAME, GAME_OVER, PAUSE_MENU};

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
private:
	int gameState;
	bool isRunning;
	int score;
	bool pause;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Menu gameOverMenu;
	Menu mainMenu;
	Menu pauseMenu;
	Button button;
	Board board;
	Fruit fruit;
	Snake snake;
};
