#include "..\lib\Game.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), pause(false), gameState(NULL), snake(32),
fruit(32), score(), ableToSetNewDirection(false), board(800, 800, 32, 10), mainMenu(renderer), gameOverMenu(renderer), 
pauseMenu(renderer), authMenu(renderer), button(renderer, { 750, 40, 50, 50 }, "", nullptr, { 255, 255, 255 }), regMenu(renderer), 
loginMenu(renderer), submit(renderer, { 290, 530, 325, 55 }, "Submit", nullptr, { 255, 255, 255 }), leaderboardMenu(renderer), previousMenu(NULL) {}

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
				//currentUser.setHighScore(score);
				isRunning = false;
			}
			else if (gameOverMenu.clickButtonCheck() == Menu::leaderboard) {
				// Variable to check from which menu is leaderboard menu selected
				previousMenu = GAME_OVER;
				gameState = LEADERBOARD;
			}
		}
		currentUser.setHighScore(score);
	}
}
void Game::handleLeaderboardEvents()
{
	SDL_Event leadEvent;
	leaderboardMenu.renderLeaderboard(renderer);
	while (SDL_PollEvent(&leadEvent) != 0) {
		if (leadEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (leadEvent.type == SDL_MOUSEMOTION)
		{
			leaderboardMenu.hoverButtonCheck();
		}
		else if (leadEvent.type == SDL_MOUSEBUTTONDOWN && leadEvent.button.button == SDL_BUTTON_LEFT)
		{
			if (leaderboardMenu.clickButtonCheck() == Menu::back)
			{
				if (previousMenu == GAME_OVER) {
					gameState = GAME_OVER;
				}
				else if (previousMenu == MAIN_MENU)
				{
					gameState = MAIN_MENU;
				}
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
			else if (mainMenu.clickButtonCheck() == Menu::leaderboard)
			{
				previousMenu = MAIN_MENU;
				gameState = LEADERBOARD;
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
void Game::handleAuthMenuEvents()
{
	SDL_Event authEvent;

	authMenu.renderAuthMenu(renderer);

	while (SDL_PollEvent(&authEvent) != 0)
	{
		if (authEvent.type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (authEvent.type == SDL_MOUSEMOTION)
		{
			authMenu.hoverButtonCheck();
		}
		else if (authEvent.type == SDL_MOUSEBUTTONDOWN && authEvent.button.button == SDL_BUTTON_LEFT)
		{
			if (authMenu.clickButtonCheck() == Menu::registration)
			{
				gameState = REGISTRATION;
			}
			else if (authMenu.clickButtonCheck() == Menu::login)
			{
				gameState = LOGIN;
			}
			else if (authMenu.clickButtonCheck() == Menu::quit)
			{
				isRunning = false;
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::handleRegistrationEvents()
{
	SDL_Event regEvent;
	int cursorX, cursorY;
	SDL_Rect usernameField = { 290, 340, 325, 55 };
	SDL_Rect passwordField = { 290, 450, 325, 55 };
	bool usernameActive = false;
	bool passwordActive = false;
	std::string usernameInput = "";
	std::string passwordInput = "";
	SDL_Cursor* cursor = nullptr;
	bool needsRedraw = true;
	enum input { none, valid, invalid };
	int usernameState = none;
	int passwordState = none;
	bool keyPressed = false;
	bool submitHover = false;

	SDL_StartTextInput();

	// Create a target texture for double buffering
	SDL_Texture* targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 896, 896);

	while (isRunning && gameState == REGISTRATION)
	{
		while (SDL_PollEvent(&regEvent) != 0)
		{
			switch (regEvent.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&cursorX, &cursorY);
				if ((cursorX >= usernameField.x && cursorX <= usernameField.x + usernameField.w &&
					cursorY >= usernameField.y && cursorY <= usernameField.y + usernameField.h) ||
					(cursorX >= passwordField.x && cursorX <= passwordField.x + passwordField.w &&
						cursorY >= passwordField.y && cursorY <= passwordField.y + passwordField.h))
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
					SDL_SetCursor(cursor);
				}
				else
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
					SDL_SetCursor(cursor);
				}
				needsRedraw = true;
				// Check if the mouse is hovering over the submit button
				if (usernameState == valid && passwordState == valid)
				{
					submitHover = submit.checkHover(cursorX, cursorY);
				}
				else
				{
					submitHover = false;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (regEvent.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&cursorX, &cursorY);
					if (cursorX >= usernameField.x && cursorX <= usernameField.x + usernameField.w &&
						cursorY >= usernameField.y && cursorY <= usernameField.y + usernameField.h)
					{
						usernameActive = true;
						passwordActive = false;
					}
					else if (cursorX >= passwordField.x && cursorX <= passwordField.x + passwordField.w &&
						cursorY >= passwordField.y && cursorY <= passwordField.y + passwordField.h)
					{
						passwordActive = true;
						usernameActive = false;
					}
					else if (submit.checkHover(cursorX, cursorY))
					{
						std::ofstream file("user_data.txt", std::ios::app);
						if (usernameState == valid && passwordState == valid)
						{
							if (file.is_open())
							{
								file << usernameInput << ":" << passwordInput << "\n";
							}
							file.close();
							gameState = AUTH_MENU;
							break;
						}
						gameState = AUTH_MENU;
					}
					else
					{
						usernameActive = false;
						passwordActive = false;
					}
				}
				break;

			case SDL_TEXTINPUT:
				if (usernameActive && usernameInput.length() < 18)
				{
					usernameInput += regEvent.text.text;
				}
				else if (passwordActive && passwordInput.length() < 18)
				{
					passwordInput += regEvent.text.text;
				}
				needsRedraw = true;
				break;

			case SDL_KEYDOWN:
				std::ofstream file("user_data.txt", std::ios::app);
				if (regEvent.key.keysym.sym == SDLK_BACKSPACE)
				{
					if (usernameActive && !usernameInput.empty())
					{
						usernameInput.pop_back();
					}
					else if (passwordActive && !passwordInput.empty())
					{
						passwordInput.pop_back();
					}
					needsRedraw = true;
				}
				else if (regEvent.key.keysym.sym == SDLK_RETURN)
				{
					keyPressed = true;
					if (usernameState == valid && passwordState == valid)
					{
						if (file.is_open())
						{
							file << usernameInput << ":" << passwordInput << "\n";
						}
						file.close();
						gameState = AUTH_MENU;
						break;
					}
				}
				break;
			}
			if (usernameInput.empty() && passwordInput.empty())
			{
				usernameState = none;
				passwordState = none;
			}
			// Password and Username validation
			if (!usernameInput.empty() && usernameInput.length() <= 15)
			{
				usernameState = valid;
			}
			else if (usernameInput.empty())
			{
				usernameState = invalid;
			}
			else
			{
				usernameState = invalid;
			}

			if (passwordInput.length() >= 8 && passwordInput.length() <= 15)
			{
				passwordState = valid;
			}
			else if (passwordInput.empty())
			{
				passwordState = invalid;
			}
			else
			{
				passwordState = invalid;
			}
		}

		// Re-render if necessary
		if (needsRedraw)
		{
			SDL_SetRenderTarget(renderer, targetTexture); // Set the render target to the texture
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// Render the registration menu
			regMenu.renderRegistration(renderer);

			// Render the user input
			regMenu.writeText(renderer, usernameInput, usernameField.x + 15, usernameField.y + 20);
			regMenu.writeText(renderer, std::string(passwordInput.size(), '*'), passwordField.x + 15, passwordField.y + 20);

			// Render the submit button only if both username and password are valid
			if (usernameState == valid && passwordState == valid)
			{
				submit.drawButton(renderer);
				if (submitHover)
				{
					submit.drawHover(renderer);
				}
			}

			SDL_SetRenderTarget(renderer, nullptr); 
			SDL_RenderCopy(renderer, targetTexture, nullptr, nullptr);
			SDL_RenderPresent(renderer);

			needsRedraw = false;
		}

		// Render error messages if any
		if (keyPressed)
		{
			if (usernameState == invalid && passwordState == invalid)
			{
				regMenu.writeText(renderer, "Username and Password", 290, 550);
				regMenu.writeText(renderer, "is not Valid!", 290, 590);
				regMenu.writeText(renderer, "Please try again", 290, 630);
				SDL_RenderPresent(renderer);
			}
			else if (usernameState == invalid)
			{
				regMenu.writeText(renderer, "Username is not Valid!", 290, 550);
				SDL_RenderPresent(renderer);
			}
			else if (passwordState == invalid)
			{
				regMenu.writeText(renderer, "Password is not valid", 290, 550);
				SDL_RenderPresent(renderer);
			}
			keyPressed = false;
		}
	}

	SDL_StopTextInput();
	if (cursor) SDL_FreeCursor(cursor);
	SDL_DestroyTexture(targetTexture);
}

void Game::handleLoginEvents()
{
	SDL_Event loginEvent;
	int cursorX, cursorY;
	SDL_Rect usernameField = { 290, 340, 325, 55 };
	SDL_Rect passwordField = { 290, 450, 325, 55 };
	bool usernameActive = false;
	bool passwordActive = false;
	std::string usernameInput = "";
	std::string passwordInput = "";
	SDL_Cursor* cursor = nullptr;
	bool needsRedraw = true;
	enum input { none, valid, invalid };
	int usernameState = none;
	int passwordState = none;
	bool keyPressed = false;
	bool submitHover = false;
	std::string storedUsername, storedPassword;
	SDL_StartTextInput();

	// Create a target texture for double buffering
	SDL_Texture* targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 896, 896);

	while (isRunning && gameState == LOGIN)
	{
		while (SDL_PollEvent(&loginEvent) != 0)
		{
			switch (loginEvent.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEMOTION:

				SDL_GetMouseState(&cursorX, &cursorY);
				if ((cursorX >= usernameField.x && cursorX <= usernameField.x + usernameField.w &&
					cursorY >= usernameField.y && cursorY <= usernameField.y + usernameField.h) ||
					(cursorX >= passwordField.x && cursorX <= passwordField.x + passwordField.w &&
						cursorY >= passwordField.y && cursorY <= passwordField.y + passwordField.h))
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
					SDL_SetCursor(cursor);
				}
				else
				{
					cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
					SDL_SetCursor(cursor);
				}
				needsRedraw = true;
				submitHover = submit.checkHover(cursorX, cursorY);


			case SDL_MOUSEBUTTONDOWN:
				if (loginEvent.button.button == SDL_BUTTON_LEFT)
				{
					SDL_GetMouseState(&cursorX, &cursorY);
					if (cursorX >= usernameField.x && cursorX <= usernameField.x + usernameField.w &&
						cursorY >= usernameField.y && cursorY <= usernameField.y + usernameField.h)
					{
						usernameActive = true;
						passwordActive = false;
					}
					else if (cursorX >= passwordField.x && cursorX <= passwordField.x + passwordField.w &&
						cursorY >= passwordField.y && cursorY <= passwordField.y + passwordField.h)
					{
						passwordActive = true;
						usernameActive = false;
					}
					else if (submit.checkHover(cursorX, cursorY))
					{
						keyPressed = true;
						std::ifstream file("user_data.txt");
						if (file.is_open()) {
							std::string storedUsername, storedPassword;
							bool credentialsMatch = false;

							std::string line;
							while (std::getline(file, line)) {
								std::istringstream iss(line);
								if (std::getline(iss, storedUsername, ':') && std::getline(iss, storedPassword)) {

									// Compare the stored credentials with user input
									if (usernameInput == storedUsername && passwordInput == storedPassword) {
										currentUser.setUsername(usernameInput);
										gameState = MAIN_MENU;
										credentialsMatch = true;
										break;
									}
								}
							}
							file.close();

							if (!credentialsMatch) {
								usernameState = invalid;
								passwordState = invalid;
							}
						}
					}
				}
				break;

			case SDL_TEXTINPUT:
				if (usernameActive && usernameInput.length() < 18)
				{
					usernameInput += loginEvent.text.text;
				}
				else if (passwordActive && passwordInput.length() < 18)
				{
					passwordInput += loginEvent.text.text;
				}
				needsRedraw = true;
				break;

			case SDL_KEYDOWN:

				if (loginEvent.key.keysym.sym == SDLK_BACKSPACE)
				{
					if (usernameActive && !usernameInput.empty())
					{
						usernameInput.pop_back();
					}
					else if (passwordActive && !passwordInput.empty())
					{
						passwordInput.pop_back();
					}
					needsRedraw = true;
				}
				else if (loginEvent.key.keysym.sym == SDLK_RETURN)
				{
					keyPressed = true;
					std::ifstream file("user_data.txt");
					if (file.is_open()) {
						std::string storedUsername, storedPassword;
						bool credentialsMatch = false;

						std::string line;
						while (std::getline(file, line)) {
							std::istringstream iss(line);
							if (std::getline(iss, storedUsername, ':') && std::getline(iss, storedPassword)) {

								// Compare the stored credentials with user input
								if (usernameInput == storedUsername && passwordInput == storedPassword) {
									currentUser.setUsername(usernameInput);
									gameState = MAIN_MENU;
									credentialsMatch = true;
									break;
								}
							}
						}
						file.close();

						if (!credentialsMatch) {
							usernameState = invalid;
							passwordState = invalid;
						}
					}
				}

				break;
			}

		}
		// Re-render if necessary
		if (needsRedraw)
		{
			SDL_SetRenderTarget(renderer, targetTexture); // Set the render target to the texture
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// Render the registration menu
			loginMenu.renderLogin(renderer);

			// Render the user input
			loginMenu.writeText(renderer, usernameInput, usernameField.x + 15, usernameField.y + 20);
			loginMenu.writeText(renderer, std::string(passwordInput.size(), '*'), passwordField.x + 15, passwordField.y + 20);

			submit.drawButton(renderer);
			if (submitHover)
			{
				submit.drawHover(renderer);
			}

			SDL_SetRenderTarget(renderer, nullptr);
			SDL_RenderCopy(renderer, targetTexture, nullptr, nullptr);
			SDL_RenderPresent(renderer);

			needsRedraw = false;
		}

		// Render error messages if any
		if (keyPressed)
		{
			if (usernameState == invalid || passwordState == invalid)
			{
				loginMenu.writeText(renderer, "Username or Password", 290, 600);
				loginMenu.writeText(renderer, "is not Valid! ", 290, 625);
				loginMenu.writeText(renderer, "Please try again", 290, 650);
				SDL_RenderPresent(renderer);
			}
			keyPressed = false;
		}
	}

	SDL_StopTextInput();
	if (cursor) SDL_FreeCursor(cursor);
	SDL_DestroyTexture(targetTexture);
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
	board.displayScore(renderer, getScore());
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
