#pragma once
#include <string>
#include <fstream>
#include <sstream>
class User {
public:
    User();
    ~User();

    void setUsername(const std::string& username);
    std::string getUsername() const;

    void setHighScore(int score);
    int getHighScore() const;
    void saveHighScore() const;

private:
    std::string username;
    std::string password;
    int highScore;
};


