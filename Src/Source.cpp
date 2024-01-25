#include "Header.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), snake(418, 418, 32), board(800, 800) {}
Game::~Game() {}

Snake::Snake(int initialX, int initialY, int cellSize) : cellSize(cellSize), direction(UP)
{
	body.push_front({ initialX, initialY });
}
Snake::~Snake() {}

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
		else if (SDL_KEYDOWN)
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
	snake.move();
	
}
void Game::render()
{
	board.drawBorder(renderer);
	snake.render(renderer);
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

Board::Board(int width, int height) : width(width), height(height), border{} {}
Board::~Board() {}

void Board::drawBorder(SDL_Renderer* renderer)
{
	border.x = 100;
	border.y = 100;
	border.w = 700;
	border.h = 700;

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