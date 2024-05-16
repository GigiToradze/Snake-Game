#include "..\lib\Game.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), pause(false), gameState(NULL), snake(32),
fruit(32), score(), ableToSetNewDirection(false), board(800, 800, 32, 10), mainMenu(renderer), gameOverMenu(renderer), 
pauseMenu(renderer), button(renderer, { 750, 40, 50, 50 }, "", nullptr, { 255, 255, 255 }) {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, int flag) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0) {
		std::cout << "Initialised successfully" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
		if (window != nullptr) {
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != nullptr) { isRunning = true; }
			else { std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl; }
		}
		else { std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl; }
	}
	else { std::cout << "SDL could not be initialised! SDL_Error: " << SDL_GetError() << std::endl; }
}
void Game::handleGameOverEvents() {
	SDL_Event event;
	gameOverMenu.renderGameOver(renderer);
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			gameOverMenu.hoverButtonCheck();
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
		{
			if (gameOverMenu.clickButtonCheck() == Menu::start) {
				snake.setX(448);
				snake.setY(448);
				snake.reset();
				score = 0;
				fruit.spawn();
				gameState = IN_GAME;
			}
			else if (gameOverMenu.clickButtonCheck() == Menu::quit) {
				isRunning = false;
			}
		}
	}
}

void Game::handleMainMenuEvents() {
	SDL_Event menuEvent;

	mainMenu.renderMainMenu(renderer);

	while (SDL_PollEvent(&menuEvent) != 0)
	{
		if (menuEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}

		if (menuEvent.type == SDL_MOUSEMOTION)
		{
			mainMenu.hoverButtonCheck();
		}
		else if (menuEvent.type == SDL_MOUSEBUTTONDOWN && menuEvent.button.button == SDL_BUTTON_LEFT)
		{
			if (mainMenu.clickButtonCheck() == Menu::start) {
				gameState = IN_GAME;
			}
			else if (mainMenu.clickButtonCheck() == Menu::quit) {
				isRunning = false;
			}
		}
	}
}
void Game::handleEvents()
{
	SDL_Event event;
	bool pause = getPause();

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}

		if (event.type == SDL_KEYDOWN && ableToSetNewDirection)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				snake.setDirection(Snake::UP);
				ableToSetNewDirection = false;
				break;
			case SDLK_RIGHT:
				snake.setDirection(Snake::RIGHT);
				ableToSetNewDirection = false;
				break;
			case SDLK_DOWN:
				snake.setDirection(Snake::DOWN);
				ableToSetNewDirection = false;
				break;
			case SDLK_LEFT:
				snake.setDirection(Snake::LEFT);
				ableToSetNewDirection = false;
				break;
			case SDLK_p:
				if (gameState == IN_GAME)
				{
					gameState = PAUSE_MENU;
					setPause(true);
				}
				else if (gameState == PAUSE_MENU)
				{
					gameState = IN_GAME;
					setPause(false);
				}
				break;
			default:
				break;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
		{
			if (pauseMenu.clickButtonCheck() == Menu::pause) {
				gameState = PAUSE_MENU;
				setPause(true);
			}
		}
	}
}

void Game::handlePauseMenuEvents()
{
	SDL_Event pauseEvent;

	pauseMenu.renderPauseMenu(renderer);

	while (SDL_PollEvent(&pauseEvent) != 0)
	{
		if (pauseEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (pauseEvent.type == SDL_MOUSEMOTION)
		{
			pauseMenu.hoverButtonCheck();
		}
		else if (pauseEvent.type == SDL_MOUSEBUTTONDOWN && pauseEvent.button.button == SDL_BUTTON_LEFT)
		{
			if (pauseMenu.clickButtonCheck() == Menu::pause || pauseMenu.clickButtonCheck() == Menu::start)
			{
				gameState = IN_GAME;
				setPause(false);
			}
			else if (pauseMenu.clickButtonCheck() == Menu::quit)
			{
				isRunning = false;
			}
		}
		else if (pauseEvent.type == SDL_KEYDOWN && pauseEvent.key.keysym.sym == SDLK_p)
		{
			gameState = IN_GAME;
			setPause(false);
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::update()
{
	if (snake.checkCollision() || snake.checkSelfCollision(snake))
	{
		std::cout << "Game Over!" << std::endl;
		//isRunning = false;
		gameState = GAME_OVER;
	}
	else if (isRunning)
	{
		if (snake.checkFruitCollision(fruit))
		{
			setScore(score);
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
	if (gameState == IN_GAME) {
		board.drawPause(renderer);
	}
	else if (gameState == PAUSE_MENU) {
		button.drawFilledTriangle(renderer, 750, 40, 750, 70, 777, 55);
	}
	board.displayText(renderer, getScore());
	fruit.render(renderer);

	snake.render(renderer);
	ableToSetNewDirection = true;

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int Game::setScore(int& score)
{
	int addScore = 10;
	if (snake.checkFruitCollision(fruit))
	{
		return score += addScore;
	}
	return score;
}
