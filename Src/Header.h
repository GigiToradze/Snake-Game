#pragma once
#include <iostream>
#include <SDL.h>
#include <deque>

class Board
{
public:
	Board(int width, int height);
	~Board();

	void drawBorder(SDL_Renderer* renderer);

private:
	SDL_Rect border;
	int width;
	int height;
};
class Snake
{
public:
	Snake(int initialX, int initialY, int cellSize);
	~Snake();

	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	void move();
	bool checkCollision();
	void grow();
	void render(SDL_Renderer* renderer);

	SDL_Point getHead() { return body.front(); }
	void setDirection(Direction newDirection)
	{
		direction = newDirection;
	}

private:
	std::deque<SDL_Point> body;
	int cellSize;
	Direction direction;
};

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, int flag);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	
	bool running()
	{ 
		return isRunning; 
	}
	SDL_Renderer* getRenderer()
	{
		return renderer;
	}

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake snake;
	Board board;
};

