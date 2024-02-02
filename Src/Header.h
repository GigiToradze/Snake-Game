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

	void drawMenu(SDL_Renderer* renderer);
	void drawBorder(SDL_Renderer* renderer);
	void displayText(SDL_Renderer* renderer, int score);
	void setScore(int newScore) { score = newScore; }
	
private:
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
	void drawButton(SDL_Renderer* renderer);

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
	int getX() { return x; }
	int getY() { return y; }

private:
	int x;
	int y;
	int size;
	int cellSize;
};
class Snake {
public:
	Snake(int initialX, int initialY, int cellSize);
	~Snake();

	enum Direction { UP, RIGHT, DOWN, LEFT };

	void move();
	void grow();
	bool checkCollision();
	bool checkFruitCollision(Fruit& fruit);
	bool checkSelfCollision(Snake& snake);
	bool checkFruitLocation(Fruit& fruit);
	void render(SDL_Renderer* renderer);

	SDL_Point getHead() { return body.front(); }
	void setDirection(Direction newDirection);

private:
	int cellSize;
	Direction direction;
	std::deque<SDL_Point> body;
};

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, int flag);
	
	void handleMainMenuEvents();
	void renderMainMenu();

	void handleEvents();
	void update();
	void render();
	void clean();

	void handleGameOverEvents();
	void renderGameOver();

	enum option { none, start, quit };
	enum GameState { MAIN_MENU, IN_GAME, GAME_OVER };

	void setGameState(int initialState) { gameState = initialState; }
	int getGameState() { return gameState; }

	int setScore(int& score);
	int getScore() { return score; }

	int getCursorX() { return cursorX; }
	int getCursorY() { return cursorY; }

	int buttonChoice(int cursorX, int cursorY);

	bool running() { return isRunning; }
	SDL_Renderer* getRenderer() { return renderer; }

	void drawHover();
	bool hover;

private:
	int gameState;
	int cursorX, cursorY;
	bool isRunning;
	int score;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Button startButton;
	Button quitButton;
	Board board;
	Snake snake;
	Fruit fruit;
};