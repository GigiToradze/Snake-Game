#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Board {
public:
	Board(int width, int height, int cellSize, int score);
	~Board();

	void drawMenu(SDL_Renderer* renderer, std::string title);
	void drawBorder(SDL_Renderer* renderer);
	void displayText(SDL_Renderer* renderer, int score);
	void setScore(int newScore) { score = newScore; }
	void drawPause(SDL_Renderer* renderer);

private:
	std::string title;
	TTF_Font* font;
	SDL_Rect border;
	SDL_Rect menuRect;
	SDL_Rect pauseRect1;
	SDL_Rect pauseRect2;
	int width;
	int height;
	int cellSize;
	int score;
};


