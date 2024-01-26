#include "Header.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), snake(448, 448, 32), board(800, 800, 32), fruit(32) {}
Game::~Game() {}

Snake::Snake(int initialX, int initialY, int cellSize) : cellSize(cellSize), direction(UP)
{
	body.push_front({ initialX, initialY });
}
Snake::~Snake() {}

Board::Board(int width, int height, int cellSize) : width(width), height(height), border{}, cellSize(cellSize) {}
Board::~Board() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, int flag)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Initialised successfully" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
		if (window != nullptr)
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != nullptr)
			{
				isRunning = true;
			}
			else
			{
				std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			}
		}
		else
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		}
	}
	else
	{
		std::cout << "SDL could not be initialised! SDL_Error: " << SDL_GetError() << std::endl;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				snake.setDirection(Snake::UP);
				break;
			case SDLK_RIGHT:
				snake.setDirection(Snake::RIGHT);
				break;
			case SDLK_DOWN:
				snake.setDirection(Snake::DOWN);
				break;
			case SDLK_LEFT:
				snake.setDirection(Snake::LEFT);
				break;
			default:
				break;
			}
		}
	}
}

void Game::update()
{
	if (snake.checkCollision() || snake.checkSelfCollision(snake))
	{
		std::cout << "Game Over!" << std::endl;
		isRunning = false;
	}
	else
	{
		if (snake.checkFruitCollision(fruit))
		{
			snake.grow();
			fruit.spawn();
			while (snake.checkFruitLocation(fruit))
			{
				fruit.spawn();
			}
		}

		snake.move();
	}
}
void Game::render()
{
	board.drawBorder(renderer);
	fruit.render(renderer);
	snake.render(renderer);

	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Board::drawBorder(SDL_Renderer* renderer)
{
	border.x = 96;
	border.y = 96;
	border.w = 704;
	border.h = 704;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &border);
}

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
		direction == LEFT && newDirection == RIGHT || direction == RIGHT && newDirection == LEFT) {}
	else
	{
		direction = newDirection;
	}
}

bool Snake::checkCollision()
{
	SDL_Point head = body.front();
	
	if (head.x < 96 || head.x > 770 || head.y < 96 || head.y > 770)	
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
			std::cout << "Fruit generated inside the snake body!" << std::endl;
			return true;
		}
	}
	return false;
}
void Snake::grow()
{
	body.push_back(body.back());
}

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