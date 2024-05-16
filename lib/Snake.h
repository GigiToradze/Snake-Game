#pragma once
#include <SDL.h>
#include <deque>

class Fruit;
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
