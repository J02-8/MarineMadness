#pragma once
#include "MarineMachine.h"
using namespace sf;

void MarineMachine::draw()
{
	// Only clear when not paused
	if (state != State::PAUSED)
	{
		m_Window.clear();
	}

	// Game Menu
	if (state == State::MAIN_MENU)
	{
		m_Window.setView(m_Window.getDefaultView()); // Reset View
		mainMenu.draw(m_Window);
		m_Window.draw(m_TitleText);
	}

	// Scoreboard screen
	if (state == State::SCOREBOARD)
	{
		m_Window.setView(m_ScoreView);
		m_ScoreSystem.draw(m_Window);
	}

	// Option screen
	if (state == State::OPTION)
	{
		m_Window.setView(m_OptionView);
		optionMenu.draw(m_Window);
	}

	// Show some story
	if (state == State::STORY_MENU)
	{
		m_Window.setView(m_StoryView);
		m_Window.draw(m_StorySprite);
		m_Window.draw(m_StoryText);
	}
	
	// Draw player and stuff
	if (state == State::PLAYING || state == State::PAUSED)
	{
		// Set the mainView to be displayed in the window
		m_Window.setView(m_MainView);

		// Draw the Level
		m_Window.draw(vaLevel, &m_TextureTiles);

		// Draw control text only for tutorial level
		if (lm.getCurrentLevel() == 1)
		{
			m_Window.draw(m_ControlsTextA);
			m_Window.draw(m_ControlsTextB);
		}

		// Draw Enemies
		for (auto& enemy : enemies)
		{
			if (enemy->isAlive())
			{
				m_Window.draw(enemy->getSprite());
			}
		}

		// Draw enemy bullets
		for (int i = 0; i < 100; i++)
		{
			if (enemyBullets[i].isInFlight())
			{
				m_Window.draw(enemyBullets[i].getShape());
			}
		}

		// Draw bullets
		for (int i = 0; i < 100; i++)
		{
			if (bullets[i].isInFlight())
			{
				m_Window.draw(bullets[i].getShape());
			}
		}

		// Draw the player
		m_Window.draw(marine.getSprite());

		// Draw the melee attack
		if (isMeleeAttacking)
		{
			m_Window.draw(meleeAttackRect);
		}

		// Draw the warp
		m_Window.draw(wp.getSprite());

		// Draw the crosshair
		m_Window.draw(spriteCrosshair);

		// Draw HUD
		m_Window.setView(m_HudView);
		m_Hud.draw(m_Window);
	}

	// Pause menu
	if (state == State::PAUSED)
	{
		m_Window.setView(m_PauseView);
		pauseMenu.draw(m_Window);
	}

	// Show window
	m_Window.display();
}

