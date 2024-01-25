#include "Header.h"

int main(int argc, char* args[])
{
	const int window_width = 900;
	const int window_height = 900;

	Game game;

	game.init("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);

	while (game.running())
	{
		game.handleEvents();
		game.update();
		game.render();

		SDL_Delay(250);
	}
	game.clean();

	return 0;
}