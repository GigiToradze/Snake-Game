#include "Header.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), gameState(NULL), snake(448, 448, 32), board(800, 800, 32, 10), fruit(32), score(), cursorX(NULL), cursorY(NULL), hover(false),
				startButton(renderer, { 290, 320, 325, 55 }, "Start", nullptr, { 255, 255, 255 }), 
				quitButton(renderer, { 290, 420, 325, 55 }, "Quit", nullptr, { 255, 255, 255 }) {}

Game::~Game() {}

Snake::Snake(int initialX, int initialY, int cellSize) : cellSize(cellSize), direction(UP) {
	body.push_front({ initialX, initialY });
}
Snake::~Snake() {}

Board::Board(int width, int height, int cellSize, int score) : width(width), height(height), border{}, cellSize(cellSize), font(), score(), menuRect{} {}
Board::~Board() {}

Button::Button(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, TTF_Font* font, const SDL_Color& color)
	: buttonRect(rect), buttonText(text), font(font), buttonColor(color), textTexture(nullptr) {}

Button::~Button() {}

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

void Game::handleMainMenuEvents() {
	SDL_Event menuEvent;

	while (SDL_PollEvent(&menuEvent) != 0)
	{
		if (menuEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}

		if (menuEvent.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&cursorX, &cursorY);
			if (buttonChoice(cursorX, cursorY) == start)
			{
				hover = true;
				
			}
			else
			{
				hover = false;
			}
		}
		else if (menuEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&cursorX, &cursorY);

			if (menuEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (buttonChoice(cursorX, cursorY) == start)
				{
					gameState = IN_GAME;
				}
				else if (buttonChoice(cursorX, cursorY) == quit)
				{
					isRunning = false;
				}
			}
		}
	}
}
void Game::drawHover()
{
	SDL_Rect hoverRect = { 290 + 8, 360 + 15, 325, 8 };
	SDL_Rect hoverRectTwo = { 615, 320 + 8, 8, 47 };
	if (hover)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &hoverRect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &hoverRectTwo);
		SDL_RenderPresent(renderer);
	}
}
void Game::renderMainMenu()
{
	board.drawMenu(renderer);
	startButton.drawButton(renderer);
	quitButton.drawButton(renderer);
	drawHover();
	SDL_RenderPresent(renderer);
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
	board.displayText(renderer, getScore());
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

int Game::setScore(int& score)
{
	int addScore = 10;
	if (snake.checkFruitCollision(fruit))
	{
		return score += addScore;
	}
	return score;
}

int Game::buttonChoice(int cursorX, int cursorY)
{
	if (cursorX >= 290 && cursorX <= 290 + 325 && cursorY >= 320 && cursorY <= 320 + 55)
	{
		return start;
	}
	else if (cursorX >= 290 && cursorX <= 290 + 325 && cursorY >= 420 && cursorY <= 420 + 55)
	{
		return quit;
	}

	return none;
}

void Board::drawMenu(SDL_Renderer* renderer)
{
	menuRect = { 200, 150, 500, 600 };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &menuRect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &menuRect);

	font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 34);
	SDL_Color textColor = { 255, 255, 255 };

	std::string scoreText = "Main Menu" ;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { 305, 220, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

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

void Board::displayText(SDL_Renderer* renderer, int score)
{
	font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 18);
	SDL_Color textColor = { 255, 255, 255 };

	std::string scoreText = "Score: " + std::to_string(score);

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { 96, 48, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
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

void Button::drawButton(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &buttonRect);

	SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &buttonRect);

	font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 20);
	SDL_Color textColor = { 255, 255, 255 };

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttonText.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { buttonRect.x + (buttonRect.w - textSurface->w) / 2,
						buttonRect.y + (buttonRect.h - textSurface->h) / 2, 
						textSurface->w, textSurface->h};
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

}
