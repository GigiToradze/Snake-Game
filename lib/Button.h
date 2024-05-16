#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Button {
public:
	Button(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, TTF_Font* font, const SDL_Color& color);
	~Button();

	void drawButton(SDL_Renderer* renderer);
	void drawHover(SDL_Renderer* renderer);
	void drawFilledTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3);
	bool checkHover(int cursorX, int cursorY) const;

private:
	SDL_Rect buttonRect;
	SDL_Color buttonColor;
	TTF_Font* font;
	std::string buttonText;
	SDL_Texture* textTexture;
};

