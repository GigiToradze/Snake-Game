#pragma once
#include <iostream>
#include <SDL.h>
#include <deque>
#include <cstdlib>
#include <ctime>

class Board
{
public:
	Board(int width, int height, int cellSize);
	~Board();

	void drawBorder(SDL_Renderer* renderer);

private:
	SDL_Rect border;
	int width;
	int height;
	int cellSize;
};

class Fruit
{
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
	bool checkFruitCollision(Fruit& fruit);
	bool checkSelfCollision(Snake& snake);
	bool checkFruitLocation(Fruit& fruit);
	void grow();
	void render(SDL_Renderer* renderer);

	SDL_Point getHead() { return body.front(); }
	void setDirection(Direction newDirection);

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
	Fruit fruit;
};