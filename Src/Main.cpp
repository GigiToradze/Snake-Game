#include "Header.h"

int main(int argc, char* args[])
{
	const int window_width = 896;
	const int window_height = 896;
	srand(static_cast<unsigned>(time(NULL)));
	Game game;
    
	game.init("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    game.setGameState(game.MAIN_MENU);

    while (game.running())
    {
        switch (game.getGameState())
        {
        case Game::MAIN_MENU:
            game.handleMainMenuEvents();
            break;
        
        case Game::IN_GAME:
            SDL_Delay(250);
            game.handleEvents();
            game.update();
            game.render();
            break;
            
        case Game::GAME_OVER:
            game.handleGameOverEvents();
            break;
        }
    }
	game.clean();

	return 0;
}