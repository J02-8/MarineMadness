#pragma once

class ScoreObserver
{
public:
    // Destructor
    virtual ~ScoreObserver() = default;
    
    // Pure virtual function to handle socre changes
    virtual void onScoreChange(int score) = 0;
};