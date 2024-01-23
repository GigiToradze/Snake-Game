#include "Header.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) {}
Game::~Game() {}

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
	}
}
void Game::update()
{
	
}
void Game::render()
{
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}