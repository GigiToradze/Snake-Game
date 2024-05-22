#include "..\lib\Board.h"
#include <iostream>
Board::Board(int width, int height, int cellSize, int score) : width(width), height(height),
border{}, pauseRect1{}, pauseRect2{}, cellSize(cellSize), font(), score(), menuRect{} {}

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

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, title.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);


	SDL_Rect textRect = { menuRect.x + (menuRect.w - textSurface->w) / 2, 220, textSurface->w, textSurface->h };
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

void Board::displayScore(SDL_Renderer* renderer, int score)
{
	font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 18);
	SDL_Color textColor = { 255, 255, 255 };

	std::string scoreText = "Score: " + std::to_string(score);

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect textRect = { 115, 48, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void Board::drawPause(SDL_Renderer* renderer)
{
	pauseRect1.x = 750;
	pauseRect1.y = 40;
	pauseRect1.w = 10;
	pauseRect1.h = 30;

	pauseRect2.x = 770;
	pauseRect2.y = 40;
	pauseRect2.w = 10;
	pauseRect2.h = 30;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &pauseRect1);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &pauseRect2);

}