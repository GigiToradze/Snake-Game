#pragma once
#include <iostream>
#include <SDL.h>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <string>

class Board {
public:
	Board(int width, int height, int cellSize, int score);
	~Board();

	void drawMenu(SDL_Renderer* renderer, std::string title);
	void drawBorder(SDL_Renderer* renderer);
	void displayText(SDL_Renderer* renderer, int score);
	void setScore(int newScore) { score = newScore; }
	
private:
	std::string title;
	TTF_Font* font;
	SDL_Rect border;
	SDL_Rect menuRect;
	int width;
	int height;
	int cellSize;
	int score;
};

class Button {
public:
	Button(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, TTF_Font* font, const SDL_Color& color);
	~Button();

	enum button { none, start, quit };

	void drawButton(SDL_Renderer* renderer);
	void drawHover(SDL_Renderer* renderer);
	bool checkHover(int cursorX, int cursorY) const;
	
private:
	SDL_Rect buttonRect;
	SDL_Color buttonColor;
	TTF_Font* font;
	std::string buttonText;
	SDL_Texture* textTexture;
};

class Fruit {
public:
	Fruit(int cellSize);
	~Fruit();

	void spawn();
	void render(SDL_Renderer* renderer);

	SDL_Point getPosition() { return { x, y }; }
	int getX() const { return x; }
	int getY() const { return y; }

private:
	int x;
	int y;
	int size;
	int cellSize;
};

class Snake {
public:
	Snake(int cellSize);
	~Snake();

	enum Direction { UP, RIGHT, DOWN, LEFT };

	void move();
	void grow();
	bool checkCollision();
	bool checkFruitCollision(Fruit& fruit);
	bool checkSelfCollision(Snake& snake);
	bool checkFruitLocation(Fruit& fruit);
	void render(SDL_Renderer* renderer);
	void setX(int newX) { initialX = newX; }
	void setY(int newY) { initialY = newY; }
	void reset();

	SDL_Point getHead() { return body.front(); }
	void setDirection(Direction newDirection);

private:
	int initialX, initialY;
	int cellSize;
	Direction direction;
	std::deque<SDL_Point> body;
};

class Menu {
public:
	Menu(SDL_Renderer* renderer);
	~Menu();

	void renderMainMenu(SDL_Renderer* renderer);
	void renderGameOver(SDL_Renderer* renderer);

	void hoverButtonCheck();
	int clickButtonCheck();
	enum button { none, start, quit };

private:
	Board board;
	Button restartButton;
	Button startButton;
	Button quitButton;
	int hover;
	int cursorX, cursorY;
};

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

	enum GameState { MAIN_MENU, IN_GAME, GAME_OVER };

	void setGameState(int initialState) { gameState = initialState; }
	int getGameState() const { return gameState; }

	int setScore(int& score);
	int getScore() const { return score; }

	bool running() const { return isRunning; }
	SDL_Renderer* getRenderer() { return renderer; }

	bool ableToSetNewDirection;

private:
	int gameState;
	bool isRunning;
	int score;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Menu gameOverMenu;
	Menu mainMenu;
	Board board;
	Snake snake;
	Fruit fruit;
};