#include "ScoreManager.h"
#include <fstream>

// Constructor: initialize score to 0 and load high score from file
ScoreManager::ScoreManager(const string& hsFile, const string& logFilePath)
    : score(0), highScoreFile(hsFile), logFile(logFilePath) 
{
    loadHighScore();
}

void ScoreManager::loadHighScore() 
{
    ifstream input(highScoreFile);
    if (input.is_open()) 
    {
        input >> highScore;
        input.close();
    }
    else 
    {
        highScore = 0; 
    }
}

void ScoreManager::addPoints(int points) 
{
    score += points;

    if (score > highScore) 
    {
        highScore = score;
    }
}

int ScoreManager::getScore() const 
{
    return score;
}

int ScoreManager::getHighScore() const 
{
    return highScore;
}

void ScoreManager::resetScore() 
{
    score = 0;
}

void ScoreManager::saveHighScore() 
{
    ofstream output(highScoreFile);

    if (output.is_open()) 
    {
        output << highScore;
        output.close();
    }
}

void ScoreManager::logScore() 
{
    ofstream logFileStream(logFile, ios::app);
    if (logFileStream.is_open()) 
    {
        logFileStream << score << "\n"; // Add each score on a new line
    }
}