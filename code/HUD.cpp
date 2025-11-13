#include "HUD.h"
#include <sstream>

HUD::HUD()
{
    // Load font
    m_Font.loadFromFile("fonts/ByteBounce.ttf");

    // Setup bullets text
    m_BulletsText.setFont(m_Font);
    m_BulletsText.setCharacterSize(60);
    m_BulletsText.setFillColor(Color::White);
    m_BulletsText.setPosition(30, 1000);

    // Setup score text
    m_ScoreText.setFont(m_Font);
    m_ScoreText.setCharacterSize(60);
    m_ScoreText.setFillColor(Color::Yellow);
    m_ScoreText.setPosition(30, 30);

    // Setup level text
    m_LevelText.setFont(m_Font);
    m_LevelText.setCharacterSize(60);
    m_LevelText.setFillColor(Color::Green);
    m_LevelText.setPosition(1720, 30);
    
    // Setup health text
    m_HealthText.setFont(m_Font);
    m_HealthText.setCharacterSize(60);
    m_HealthText.setFillColor(Color::Magenta);
    m_HealthText.setPosition(1750, 1000);

    // Initialize values
    m_BulletsInClip = 0;
    m_BulletsSpare = 0;
    m_Score = 0;
    m_Level = 0;
}

void HUD::update(int bulletsInClip, int bulletsSpare, int score, int level, int health)
{
    m_BulletsInClip = bulletsInClip;
    m_BulletsSpare = bulletsSpare;
    m_Score = score;
    m_Level = level;
    m_Health = health;

    stringstream ammoStream;
    ammoStream << "Ammo: " << m_BulletsInClip << "/" << m_BulletsSpare;
    m_BulletsText.setString(ammoStream.str());

    stringstream scoreStream;
    scoreStream << "Score: " << m_Score;
    m_ScoreText.setString(scoreStream.str());

    stringstream levelStream;
    levelStream << "Level: " << m_Level - 1;
    m_LevelText.setString(levelStream.str());

    stringstream healthStream;
    healthStream << "HP: " << m_Health;
    m_HealthText.setString(healthStream.str());
}

void HUD::draw(RenderWindow& window)
{
    window.draw(m_BulletsText);
    window.draw(m_ScoreText);
    window.draw(m_LevelText);
    window.draw(m_HealthText);
}
