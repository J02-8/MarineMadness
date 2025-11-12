#pragma once
#include "LevelManager.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class HUD
{
private:
    Font m_Font;

    Text m_BulletsText;
    Text m_ScoreText;
    Text m_LevelText;
    Text m_HealthText;

    int m_BulletsInClip;
    int m_BulletsSpare;
    int m_Score;
    int m_Level;
    int m_Health;

    // Create objects for marine health and current level
    Player marine;
    LevelManager lm;

public:
    HUD();

    void update(int bulletsInClip, int bulletsSpare, int score, int level, int health);

    void draw(RenderWindow& window);
};