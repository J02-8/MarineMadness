#pragma once
#include "ScoreSubject.h"
#include "ScoreManager.h"

class ScoreSystem : public ScoreSubject
{
private:
    ScoreManager m_ScoreManager;
	Font font;

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

    // Draw the scoreboard
    void draw(RenderWindow& window)
    {
		font.loadFromFile("fonts/ByteBounce.ttf");

		// Title
		Text title("Scoreboard", font, 80);
		title.setFillColor(Color::Yellow);
		title.setPosition(200.f, 100.f);
		window.draw(title);

		// High score display
		stringstream ss;
		ss << "High Score: " << getHighScore();
		Text hsText(ss.str(), font, 60);
		hsText.setFillColor(Color::White);
		hsText.setPosition(200.f, 220.f);
		window.draw(hsText);

		// Instruction
		Text instr("Press Escape to return", font, 32);
		instr.setFillColor(Color::Green);
		instr.setPosition(200.f, 900.f);
		window.draw(instr);
    }
};