#pragma once
#include "ScoreSubject.h"
#include "ScoreManager.h"

class ScoreSystem : public ScoreSubject
{
private:
    ScoreManager m_ScoreManager;

public:
    // Add points and notify the observers
    void addPoints(int points)
    {
        m_ScoreManager.addPoints(points);
        notifyScoreChange(m_ScoreManager.getScore());
    }

    int getScore() const { return m_ScoreManager.getScore(); }
    int getHighScore() const { return m_ScoreManager.getHighScore(); }

    void resetScore() { m_ScoreManager.resetScore(); }

    void saveScores()
    {
        m_ScoreManager.saveHighScore();
        m_ScoreManager.logScore();
    }
};