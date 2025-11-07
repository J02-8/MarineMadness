#pragma once
#include <vector>
#include "ScoreObserver.h"

using namespace std;

class ScoreSubject
{
private:
    vector<ScoreObserver*> m_Observers;

public:
    // Destructor
    virtual ~ScoreSubject() = default;

    // Adds ovservers to the list so it can recieve score updates
    void addObserver(ScoreObserver* observer)
    {
        m_Observers.push_back(observer);
    }

protected:
    // Notifies ovservers that the score has changed
    void notifyScoreChange(int score)
    {
        for (auto* obs : m_Observers)
        {
            obs->onScoreChange(score);
        }
    }
};