#pragma once
#include <string>

using namespace std;

class ScoreManager 
{
private:
    int score;
    int highScore;
    string highScoreFile;
    string logFile;

    void loadHighScore();  // Loads from file

public:
    ScoreManager(const string& hsFile = "gamedata/scores.txt", const string& logFile = "gamedata/scores_log.txt");

    void addPoints(int points);
    int getScore() const;
    int getHighScore() const;

    void resetScore();
    void saveHighScore();  // Writes the highest score to file
    void logScore(); // Writes all scores to file
};