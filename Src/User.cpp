#include "..\lib\User.h"

User::User() : highScore(0) {}

User::~User() {}

void User::setUsername(const std::string& username) {
    this->username = username;
}

std::string User::getUsername() const {
    return username;
}

void User::setHighScore(int score) {
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }
}

int User::getHighScore() const {
    return highScore;
}

void User::saveHighScore() const {
    std::ifstream file("user_score.txt");
    std::string data, line;
    bool found = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string user;
        int score;

        // Parse the line with colon separator
        if (std::getline(iss, user, ':') && (iss >> score)) {
            if (user == username) {
                // Update the line with the new high score
                line = username + ":" + std::to_string(highScore);
                found = true;
            }
        }
        // Append the line (modified or unmodified) to data
        data += line + "\n";
    }
    file.close();

    if (!found) {
        // Add new user and high score if not found
        data += username + ":" + std::to_string(highScore) + "\n";
    }

    std::ofstream outFile("user_score.txt");
    outFile << data;
}

