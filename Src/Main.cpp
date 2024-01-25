#include "Header.h"

int main(int argc, char* args[])
{
	const int window_width = 896;
	const int window_height = 896;

	srand(static_cast<unsigned>(time(NULL)));

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