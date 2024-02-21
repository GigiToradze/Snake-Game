#include "Header.h"

Menu::Menu(SDL_Renderer* renderer) : cursorX(NULL), cursorY(NULL), hover(false), board(800, 800, 32, 10),
				startButton(renderer, { 290, 320, 325, 55 }, "Start", nullptr, { 255, 255, 255 }),
				quitButton(renderer, { 290, 420, 325, 55 }, "Quit", nullptr, { 255, 255, 255 }),
				restartButton(renderer, { 290, 320, 325, 55 }, "Restart", nullptr, {255, 255, 255 }) {}

Menu::~Menu() { std::cout << "Destructor executed!" << std::endl; }
void Menu::renderGameOver(SDL_Renderer* renderer) {

	board.drawMenu(renderer, "Game Over");
	restartButton.drawButton(renderer);
	quitButton.drawButton(renderer);

	if (hover == start) {
		restartButton.drawHover(renderer);
	}
	else if (hover == quit) {
		quitButton.drawHover(renderer);
	};

	SDL_RenderPresent(renderer);
}
void Menu::hoverButtonCheck()
{
	SDL_GetMouseState(&cursorX, &cursorY);

	if (startButton.checkHover(cursorX, cursorY)) {
		hover = start;
	}
	else if (quitButton.checkHover(cursorX, cursorY)) {
		hover = quit;
	}
	else {
		hover = none;
	}
}

int Menu::clickButtonCheck()
{
	SDL_GetMouseState(&cursorX, &cursorY);

	if (startButton.checkHover(cursorX, cursorY)) {
		//gameState = IN_GAME;
		return start;
	}
	else if (quitButton.checkHover(cursorX, cursorY)) {
		//isRunning = false;
		return quit;
	}
	else
	{
		return none;
	}
}
void Menu::renderMainMenu(SDL_Renderer* renderer)
{
	board.drawMenu(renderer, "Main Menu");
	startButton.drawButton(renderer);
	quitButton.drawButton(renderer);

	if (hover == start) {
		startButton.drawHover(renderer);
	}
	else if (hover == quit) {
		quitButton.drawHover(renderer);
	};

	SDL_RenderPresent(renderer);
}

Button::Button(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, TTF_Font* font, const SDL_Color& color)
	: buttonRect(rect), buttonText(text), font(font), buttonColor(color), textTexture(nullptr) {}

Button::~Button() {}
bool Button::checkHover(int cursorX, int cursorY) const
{
	if (cursorX >= buttonRect.x && cursorX <= buttonRect.x + buttonRect.w && cursorY >= buttonRect.y && cursorY <= buttonRect.y + buttonRect.h)
	{
		std::cout << "X: " << cursorX << "Y: " << cursorY << std::endl;
		return true;
	}

	return false;
}

void Button::drawHover(SDL_Renderer* renderer)
{
	SDL_Rect hoverRect = { buttonRect.x + 8, buttonRect.y + buttonRect.h, buttonRect.w, 8 };
	SDL_Rect hoverRectTwo = { buttonRect.x + buttonRect.w, buttonRect.y + 8, 8, 47 };

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &hoverRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &hoverRectTwo);
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
						textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

}
Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), gameState(NULL), snake(32),
fruit(32), score(), ableToSetNewDirection(false), board(800, 800, 32, 10), mainMenu(renderer), gameOverMenu(renderer) {}

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
			if (gameOverMenu.clickButtonCheck() == 1) {
				snake.setX(448);
				snake.setY(448);
				snake.reset();
				score = 0;
				fruit.spawn();
				gameState = IN_GAME;
			}
			else if (gameOverMenu.clickButtonCheck() == 2) {
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
			if (mainMenu.clickButtonCheck() == 1) {
				gameState = IN_GAME;
			}
			else if (mainMenu.clickButtonCheck() == 2) {
				isRunning = false;
			}
		}
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
Board::Board(int width, int height, int cellSize, int score) : width(width), height(height),
border{}, cellSize(cellSize), font(), score(), menuRect{} {}
Board::~Board() {}
void Board::drawMenu(SDL_Renderer* renderer, std::string title)
{
	menuRect = { 200, 150, 500, 600 };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &menuRect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &menuRect);

	font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 34);
	SDL_Color textColor = { 255, 255, 255 };

	//std::string title = "Main Menu" ;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, title.c_str(), textColor);
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
		direction == LEFT && newDirection == RIGHT || direction == RIGHT && newDirection == LEFT) {}
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