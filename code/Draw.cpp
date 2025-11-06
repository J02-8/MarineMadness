#pragma once
#include "MarineMachine.h"
using namespace sf;

void MarineMachine::draw()
{
	m_Window.clear();

	// Game Menu
	if (state == State::MAIN_MENU)
	{
		m_Window.setView(m_Window.getDefaultView()); // Reset View
		mainMenu.drawParticles(m_Window); // Draw the particle background first
		mainMenu.draw(m_Window);
	}

	// Show some story
	if (state == State::STORY_MENU)
	{
		m_Window.draw(storySprite);
		m_Window.draw(storyText);
	}
	
	// Draw player and stuff
	if (state == State::PLAYING)
	{
		m_Window.clear();

		// Set the mainView to be displayed in the window
		m_Window.setView(m_MainView);

		// Draw the Level
		m_Window.draw(vaLevel, &m_TextureTiles);

		//draw the dinosaurs
		if (dinosaurs != nullptr) {
			for (int i = 0; i < numDinosaurs; i++)
			{
				if (dinosaurs[i]->isAlive())
				{
					m_Window.draw(dinosaurs[i]->getSprite());
				}
			}
		}

		//draw the cowboys
		if (cowboys!= nullptr) {
			for (int i = 0; i < numCowboys; i++)
			{
				if (cowboys[i]->isAlive())
				{
					m_Window.draw(cowboys[i]->getSprite());
				}
			}
		}

		//draw the androids
		if (androids != nullptr) {
			for (int i = 0; i < numAndroids; i++)
			{
				if (androids[i]->isAlive())
				{
					m_Window.draw(androids[i]->getSprite());
				}
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

		// Draw the warp
		m_Window.draw(wp.getSprite());

		// Draw the crosshair
		m_Window.draw(spriteCrosshair);
		

		/**
		// Draw the pickups is currently spawned
		if (ammoPickup.isSpawned())
		{
			window.draw(ammoPickup.getSprite());
		}
		if (healthPickup.isSpawned())
		{
			window.draw(healthPickup.getSprite());
		}

		// draw the weapon pick ups 
		if (smgPickup.isSpawned()) {
			window.draw(smgPickup.getSprite());
		}
		if (shotgunPickup.isSpawned()) {
			window.draw(shotgunPickup.getSprite());
		}

		//draw the melee attack
		if (isMeleeAttacking) {

			window.draw(meleeAttackRect);

		}
		*/



		/*
		// Switch to the HUD view
		window.setView(hudView);

		// Draw all the HUD elements
		window.draw(spriteAmmoIcon);
		window.draw(ammoText);
		window.draw(scoreText);
		window.draw(hiScoreText);
		window.draw(healthBar);
		window.draw(waveNumberText);
		window.draw(zombiesRemainingText);
		*/
	}

	/*
	if (state == State::LEVELING_UP)
	{
		window.draw(spriteGameOver);
		window.draw(levelUpText);
	}

	if (state == State::PAUSED)
	{
		window.draw(pausedText);
	}

	if (state == State::GAME_OVER)
	{
		window.draw(spriteGameOver);
		window.draw(gameOverText);
		window.draw(scoreText);
		window.draw(hiScoreText);
	}
	*/



	m_Window.display();

}

