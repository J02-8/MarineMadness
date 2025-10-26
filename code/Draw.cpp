
#pragma once
#include "MarineMachine.h"
using namespace sf;

void MarineMachine::draw()
{
	
	// Draw stuff
	if (state == State::PLAYING)
	{
		m_Window.clear();

		// set the mainView to be displayed in the window
		m_Window.setView(m_MainView);
		
		// Draw the background
		//m_Window.draw(m_BackgroundSprite);

		// Draw the Level
		m_Window.draw(vaLevel, &m_TextureTiles);

		// Draw the player
		m_Window.draw(marine.getSprite());

		// Draw the warp
		m_Window.draw(wp.getSprite());

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


		//Draw the crosshair
		//m_Window.draw(spriteCrosshair);

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

