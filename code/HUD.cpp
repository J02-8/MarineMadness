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

    // Initialize values
    m_BulletsInClip = 0;
    m_BulletsSpare = 0;
    m_Score = 0;
}

void HUD::update(int bulletsInClip, int bulletsSpare, int score)
{
    m_BulletsInClip = bulletsInClip;
    m_BulletsSpare = bulletsSpare;
    m_Score = score;

    stringstream ammoStream;
    ammoStream << "Ammo: " << m_BulletsInClip << "/" << m_BulletsSpare;
    m_BulletsText.setString(ammoStream.str());

    stringstream scoreStream;
    scoreStream << "Score: " << m_Score;
    m_ScoreText.setString(scoreStream.str());
}

void HUD::draw(RenderWindow& window)
{
    window.draw(m_BulletsText);
    window.draw(m_ScoreText);
}
