#include "ScoreManager.h"
#include <fstream>

// Constructor: initialize score to 0 and load high score from file
ScoreManager::ScoreManager(const string& hsFile, const string& logFilePath)
    : score(0), highScoreFile(hsFile), logFile(logFilePath) 
{
    loadHighScore();
}

// Loads the high score from the file
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

// adds points and checks the high score
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

// Saves the high score to a file
void ScoreManager::saveHighScore() 
{
    ofstream output(highScoreFile);

    if (output.is_open()) 
    {
        output << highScore;
        output.close();
    }
}

// Appends the score to a log file
void ScoreManager::logScore() 
{
    ofstream logFileStream(logFile, ios::app); // ios::app is used to append the file

    if (logFileStream.is_open()) 
    {
        logFileStream << score << "\n"; // Add each score on a new line
    }
}