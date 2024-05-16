#include "..\lib\Fruit.h"

Fruit::Fruit(int cellSize) : size(cellSize), cellSize(cellSize)
{
	spawn();
}
Fruit::~Fruit() {}

void Fruit::spawn()
{
	x = rand() % (704 / cellSize) * cellSize + 96;
	y = rand() % (704 / cellSize) * cellSize + 96;
}

void Fruit::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	SDL_Rect rect = { x, y, size, size };
	SDL_RenderFillRect(renderer, &rect);
}