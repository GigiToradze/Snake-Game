#include "..\lib\Snake.h"
#include "..\lib\Fruit.h"

Snake::Snake(int cellSize) : cellSize(cellSize), direction(UP), initialX(448), initialY(448) {
	body.push_front({ initialX, initialY });
}
Snake::~Snake() {}

void Snake::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (const SDL_Point& bodyPart : body)
	{
		SDL_Rect rect = { bodyPart.x, bodyPart.y, cellSize, cellSize };
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Snake::move()
{
	// Current head position
	SDL_Point head = body.front();

	switch (direction)
	{
	case UP:
		head.y -= cellSize;
		break;
	case RIGHT:
		head.x += cellSize;
		break;
	case DOWN:
		head.y += cellSize;
		break;
	case LEFT:
		head.x -= cellSize;
		break;
	}
	body.push_front(head);
	body.pop_back();
}

void Snake::setDirection(Direction newDirection)
{
	if (direction == UP && newDirection == DOWN || direction == DOWN && newDirection == UP ||
		direction == LEFT && newDirection == RIGHT || direction == RIGHT && newDirection == LEFT) {
	}
	else
	{
		direction = newDirection;
	}
}

bool Snake::checkCollision()
{
	SDL_Point head = body.front();

	if ((head.x == 96 && direction == LEFT) || (head.x == 768 && direction == RIGHT) ||
		(head.y == 96 && direction == UP) || (head.y == 768 && direction == DOWN))
	{
		return true;
	}
	return false;
}

bool Snake::checkFruitCollision(Fruit& fruit)
{
	SDL_Point head = body.front();

	if (head.x == fruit.getX() && head.y == fruit.getY())
	{
		return true;
	}
	return false;
}

bool Snake::checkSelfCollision(Snake& snake)
{
	SDL_Point head = body.front();

	for (auto i = body.begin() + 1; i != body.end(); ++i)
	{
		if (head.x == i->x && head.y == i->y)
		{
			return true;
		}
	}
	return false;
}

bool Snake::checkFruitLocation(Fruit& fruit)
{
	for (auto i = body.begin() + 1; i != body.end(); ++i)
	{
		if (fruit.getX() == i->x && fruit.getY() == i->y)
		{
			return true;
		}
	}
	return false;
}

void Snake::grow()
{
	body.push_back(body.back());
}

void Snake::reset()
{
	body.clear();
	body.push_front({ initialX, initialY });
	direction = UP;
}