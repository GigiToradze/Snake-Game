#pragma once

#include <SDL.h>
#include <iostream>

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

