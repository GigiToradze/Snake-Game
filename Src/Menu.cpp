#include "..\lib\Menu.h"

Menu::Menu(SDL_Renderer* renderer) : cursorX(NULL), cursorY(NULL), hover(false), board(800, 800, 32, 10),
startButton(renderer, { 290, 320, 325, 55 }, "Start", nullptr, { 255, 255, 255 }),
quitButton(renderer, { 290, 420, 325, 55 }, "Quit", nullptr, { 255, 255, 255 }),
restartButton(renderer, { 290, 320, 325, 55 }, "Restart", nullptr, { 255, 255, 255 }),
resumeButton(renderer, { 290, 320, 325, 55 }, "Resume", nullptr, { 255, 255, 255 }),
pauseButton(renderer, { 750, 40, 50, 50 }, "", nullptr, { 255, 255, 255 }),
regButton(renderer, { 290, 320, 325, 55 }, "Register", nullptr, { 255, 255, 255 }),
loginButton(renderer, { 290, 420, 325, 55 }, "Login", nullptr, { 255, 255, 255 }),
qButton(renderer, { 290, 520, 325, 55 }, "Quit", nullptr, { 255, 255, 255 }),
leaderboardButton(renderer, {290, 420, 325, 55}, "Leaderboard", nullptr, {255, 255, 255}),
backButton(renderer, { 290, 500, 325, 55 }, "Go back", nullptr, { 255, 255, 255 }),
currentMenu(MAIN_MENU), fieldRect() {}

Menu::~Menu() {}

struct UserScore {
    std::string username;
    int score = 0;
};

// Simple bubble sort to sort scores in descending order
void bubbleSort(UserScore scores[], int count) {
    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = 0; j < count - i - 1; ++j)
        {
            if (scores[j].score < scores[j + 1].score)
            {
                UserScore temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

void Menu::renderLeaderboard(SDL_Renderer* renderer)
{
    currentMenu = LEADERBOARD;
    board.drawMenu(renderer, "Leaderboard");
    backButton.drawButton(renderer);
    UserScore scores[100];
    int count = 0;
    if (hover == back)
    {
        backButton.drawHover(renderer);
    }
    
    std::ifstream file("user_score.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string scoreStr;

        if (std::getline(iss, scores[count].username, ':') && std::getline(iss, scoreStr)) {
            std::istringstream scoreStream(scoreStr);
            scoreStream >> scores[count].score;
            count++;
        }
    }
    file.close();

    bubbleSort(scores, count);

    // Get the top 5 scores
    int displayCount = count < 5 ? count : 5;

    // display top scores
    const int lineWidth = 25;

    for (int i = 0; i < displayCount; ++i) {
        std::ostringstream oss;
        std::string username = scores[i].username;
        std::string score = std::to_string(scores[i].score);

        // number of dots needed
        int dotsCount = lineWidth - (username.length() + score.length() + 5);
        std::string dots(dotsCount, '.');

        oss << std::setw(0) << (i + 1) << ". " << username << dots << score;
        std::string text = oss.str();

        int y = 320 + i * 25;
        if (!text.empty())
        {
            writeText(renderer, text, 270, y);
        }
    }

    SDL_RenderPresent(renderer);
}

void Menu::renderGameOver(SDL_Renderer* renderer)
{
    currentMenu = GAME_OVER_MENU;
    board.drawMenu(renderer, "Game Over");
    restartButton.drawButton(renderer);
    leaderboardButton.drawButton(renderer);
    qButton.drawButton(renderer);

    if (hover == start)
    {
        restartButton.drawHover(renderer);
    }
    else if (hover == quit)
    {
        qButton.drawHover(renderer);
    }
    else if (hover == leaderboard)
    {
        leaderboardButton.drawHover(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Menu::renderPauseMenu(SDL_Renderer* renderer)
{
    currentMenu = PAUSE_MENU;
    board.drawMenu(renderer, "Game Paused");
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
    leaderboardButton.drawButton(renderer);
    qButton.drawButton(renderer);

    if (hover == start) {
        startButton.drawHover(renderer);
    }
    else if (hover == leaderboard)
    {
        leaderboardButton.drawHover(renderer);
    }
    else if (hover == quit) {
        qButton.drawHover(renderer);
    }

    SDL_RenderPresent(renderer);
}
void Menu::renderAuthMenu(SDL_Renderer* renderer)
{
    currentMenu = AUTH_MENU;
    board.drawMenu(renderer, "Welcome");
    regButton.drawButton(renderer);
    loginButton.drawButton(renderer);
    qButton.drawButton(renderer);

    if (hover == registration) {
        regButton.drawHover(renderer);
    }
    else if (hover == login) {
        loginButton.drawHover(renderer);
    }
    else if (hover == quit) {
        qButton.drawHover(renderer);
    }
    SDL_RenderPresent(renderer);
}
void Menu::hoverButtonCheck()
{
    SDL_GetMouseState(&cursorX, &cursorY);

    switch (currentMenu)
    {
    case AUTH_MENU:
        if (regButton.checkHover(cursorX, cursorY)) {
            hover = registration;
        }
        else if (loginButton.checkHover(cursorX, cursorY)) {
            hover = login;
        }
        else if (qButton.checkHover(cursorX, cursorY)) {
            hover = quit;
        }
        else {
            hover = none;
        }
        break;
    case MAIN_MENU:
        if (startButton.checkHover(cursorX, cursorY)) {
            hover = start;
        }
        else if (leaderboardButton.checkHover(cursorX, cursorY))
        {
            hover = leaderboard;
        }
        else if (qButton.checkHover(cursorX, cursorY)) {
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
        else if (qButton.checkHover(cursorX, cursorY)) {
            hover = quit;
        }
        else if (leaderboardButton.checkHover(cursorX, cursorY)) {
            hover = leaderboard;
        }
        else {
            hover = none;
        }
        break;
    case LEADERBOARD:
        if (backButton.checkHover(cursorX, cursorY)) {
            hover = back;
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
    if (currentMenu == AUTH_MENU && regButton.checkHover(cursorX, cursorY)) {
        return registration;
    }
    else if (currentMenu == AUTH_MENU && loginButton.checkHover(cursorX, cursorY)) {
        return login;
    }
    else if (startButton.checkHover(cursorX, cursorY)) {
        return start;
    }
    else if (resumeButton.checkHover(cursorX, cursorY)) {
        return resume;
    }
    else if (currentMenu == LEADERBOARD && backButton.checkHover(cursorX, cursorY)) {
        return back;
    }
    else if ((currentMenu == AUTH_MENU && quitButton.checkHover(cursorX, cursorY)) || qButton.checkHover(cursorX, cursorY)) {
        return quit;
    }
    else if (leaderboardButton.checkHover(cursorX, cursorY)) {
        return leaderboard;
    }
    else if (pauseButton.checkHover(cursorX, cursorY)) {
        return pause;
    }
    else {
        return none;
    }
}

void Menu::drawTextField(SDL_Renderer* renderer, SDL_Rect *fieldRect)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, fieldRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, fieldRect);

}

void Menu::writeText(SDL_Renderer* renderer, std::string text, int x, int y)
{
    TTF_Font* font;
    font = TTF_OpenFont("Assets/PressStart2P-Regular.ttf", 16);
    SDL_Color textColor = { 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (text.empty()) {
        // Do not attempt to render if the text is empty
        return;
    }
    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void Menu::renderRegistration(SDL_Renderer* renderer)
{
    SDL_Rect usernameField = { 290, 340, 325, 55 };
    SDL_Rect passwordField = { 290, 450, 325, 55 };

    board.drawMenu(renderer, "Registration");
    writeText(renderer, "Enter Username:", 290, 310);
    drawTextField(renderer, &usernameField);
    writeText(renderer, "Enter Password:", 290, 420);
    drawTextField(renderer, &passwordField);
   
    SDL_RenderPresent(renderer);
}

void Menu::renderLogin(SDL_Renderer* renderer)
{
    SDL_Rect usernameField = { 290, 340, 325, 55 };
    SDL_Rect passwordField = { 290, 450, 325, 55 };

    board.drawMenu(renderer, "Login");
    writeText(renderer, "Enter Username:", 290, 310);
    drawTextField(renderer, &usernameField);
    writeText(renderer, "Enter Password:", 290, 420);
    drawTextField(renderer, &passwordField);

    SDL_RenderPresent(renderer);
}

