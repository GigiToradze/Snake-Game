#include "..\lib\Button.h"

Button::Button(SDL_Renderer* renderer, const SDL_Rect& rect, const std::string& text, TTF_Font* font, const SDL_Color& color)
	: buttonRect(rect), buttonText(text), font(font), buttonColor(color), textTexture(nullptr) {}

Button::~Button() {}

bool Button::checkHover(int cursorX, int cursorY) const
{
	if (cursorX >= buttonRect.x && cursorX <= buttonRect.x + buttonRect.w && cursorY >= buttonRect.y && cursorY <= buttonRect.y + buttonRect.h) {
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

void Button::drawFilledTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
	if (y2 < y1) { std::swap(x1, x2); std::swap(y1, y2); }
	if (y3 < y1) { std::swap(x1, x3); std::swap(y1, y3); }
	if (y3 < y2) { std::swap(x2, x3); std::swap(y2, y3); }

	int totalHeight = y3 - y1;
	for (int i = 0; i < totalHeight; i++) {
		bool secondHalf = i > y2 - y1 || y2 == y1;
		int segmentHeight = secondHalf ? y3 - y2 : y2 - y1;
		float alpha = (float)i / totalHeight;
		float beta = (float)(i - (secondHalf ? y2 - y1 : 0)) / segmentHeight;

		int A = x1 + (x3 - x1) * alpha;
		int B = secondHalf ? x2 + (x3 - x2) * beta : x1 + (x2 - x1) * beta;

		if (A > B) std::swap(A, B);

		SDL_RenderDrawLine(renderer, A, y1 + i, B, y1 + i);
	}
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
