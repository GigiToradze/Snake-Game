#include "..\lib\Menu.h"

Menu::Menu(SDL_Renderer* renderer) : cursorX(NULL), cursorY(NULL), hover(false), board(800, 800, 32, 10),
startButton(renderer, { 290, 320, 325, 55 }, "Start", nullptr, { 255, 255, 255 }),
quitButton(renderer, { 290, 420, 325, 55 }, "Quit", nullptr, { 255, 255, 255 }),
restartButton(renderer, { 290, 320, 325, 55 }, "Restart", nullptr, { 255, 255, 255 }),
resumeButton(renderer, { 290, 320, 325, 55 }, "Resume", nullptr, { 255, 255, 255 }),
pauseButton(renderer, { 750, 40, 50, 50 }, "", nullptr, { 255, 255, 255 }),
currentMenu(MAIN_MENU) {}

Menu::~Menu() {}

void Menu::renderGameOver(SDL_Renderer* renderer)
{
    currentMenu = GAME_OVER_MENU;
    board.drawMenu(renderer, "Game Over");
    restartButton.drawButton(renderer);
    quitButton.drawButton(renderer);

    if (hover == start)
    {
        restartButton.drawHover(renderer);
    }
    else if (hover == quit)
    {
        quitButton.drawHover(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Menu::renderPauseMenu(SDL_Renderer* renderer)
{
    currentMenu = PAUSE_MENU;
    board.drawMenu(renderer, " Paused");
    resumeButton.drawButton(renderer);
    quitButton.drawButton(renderer);

    if (hover == resume)
    {
        resumeButton.drawHover(renderer);
    }
    else if (hover == quit)
    {
        quitButton.drawHover(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Menu::renderMainMenu(SDL_Renderer* renderer)
{
    currentMenu = MAIN_MENU;
    board.drawMenu(renderer, "Main Menu");
    startButton.drawButton(renderer);
    quitButton.drawButton(renderer);

    if (hover == start) {
        startButton.drawHover(renderer);
    }
    else if (hover == quit) {
        quitButton.drawHover(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Menu::hoverButtonCheck()
{
    SDL_GetMouseState(&cursorX, &cursorY);

    switch (currentMenu)
    {
    case MAIN_MENU:
        if (startButton.checkHover(cursorX, cursorY)) {
            hover = start;
        }
        else if (quitButton.checkHover(cursorX, cursorY)) {
            hover = quit;
        }
        else {
            hover = none;
        }
        break;
    case PAUSE_MENU:
        if (resumeButton.checkHover(cursorX, cursorY)) {
            hover = resume;
        }
        else if (quitButton.checkHover(cursorX, cursorY)) {
            hover = quit;
        }
        else if (pauseButton.checkHover(cursorX, cursorY)) {
            hover = pause;
        }
        else {
            hover = none;
        }
        break;
    case GAME_OVER_MENU:
        if (restartButton.checkHover(cursorX, cursorY)) {
            hover = start;
        }
        else if (quitButton.checkHover(cursorX, cursorY)) {
            hover = quit;
        }
        else {
            hover = none;
        }
        break;
    }
}

int Menu::clickButtonCheck()
{
    SDL_GetMouseState(&cursorX, &cursorY);

    if (startButton.checkHover(cursorX, cursorY)) {
        return start;
    }
    else if (resumeButton.checkHover(cursorX, cursorY)) {
        return resume;
    }
    else if (quitButton.checkHover(cursorX, cursorY)) {
        return quit;
    }
    else if (pauseButton.checkHover(cursorX, cursorY)) {
        return pause;
    }
    else
    {
        return none;
    }
}
