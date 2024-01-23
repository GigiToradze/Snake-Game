#include "Header.h"

int main(int argc, char* args[])
{
	Game game;
	game.init("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

	while (game.running())
	{
		game.handleEvents();
		game.update();
		game.render();
	}
	game.clean();

	return 0;
}