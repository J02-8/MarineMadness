#include "MarineMachine.h"
#include "SoundManager.h"
#include <iostream>
#include <typeinfo>

using namespace std;

extern SoundManager soundManager2;
extern Time gameTimeTotal;

void MarineMachine::detectCollision()
{
	// Bullet hitting an enemy
	for (int i = 0; i < 100; ++i)
	{
		if (!bullets[i].isInFlight()) continue;

		for (auto& enemy : enemies)
		{
			if (!enemy->isAlive()) continue;

			if (bullets[i].getPosition().intersects(enemy->getPosition()))
			{
				bullets[i].stop();

				if (enemy->hit())
				{
					m_ScoreSystem.addPoints(10);
					cout << "Enemy killed!" << endl;
				}

				// Play appropriate sound
				string typeName = typeid(*enemy).name();
				if (typeName.find("Dinosaur") != string::npos)
				{
					soundManager2.playSplat();
				}
				else if (typeName.find("Cowboy") != string::npos)
				{
					soundManager2.playSplat();
				}
				else if (typeName.find("Android") != string::npos)
				{
					soundManager2.playBoom();
				}
			}

			
		}
	}

	// Enemy is attacked by melee weapon
	for (auto& enemy : enemies)
	{
		if (meleeAttackRect.getGlobalBounds().intersects(enemy->getPosition()))
		{
			if (enemy->hit())
			{
				// Remove collision for weapon
				meleeAttackRect.setPosition(2000, 60);
				
				// Update score
				m_ScoreSystem.addPoints(15);
				
				// Print
				cout << "Enemy killed by melee attack!" << endl;
			}
		}
	}
	
	// Enemy bullets hitting the player
	for (int i = 0; i < 100; ++i)
	{
		if (!enemyBullets[i].isInFlight()) continue;

		if (enemyBullets[i].getPosition().intersects(marine.getPosition()) && marine.getHealth() > 0)
		{
			enemyBullets[i].stop();

			if (marine.hit(gameTimeTotal))
			{
				soundManager2.playHit();
				cout << "Player hit by enemy bullet!" << endl;
			}

			if (marine.getHealth() <= 0)
			{
				m_ScoreSystem.saveScores();
				resetGame();
				state = State::MAIN_MENU;
				return;
			}
		}
	}

	// Enemies intersect the player
	for (auto& enemy : enemies)
	{
		if (enemy->isAlive() && marine.getPosition().intersects(enemy->getPosition()))
		{
			if (marine.hit(gameTimeTotal))
			{
				soundManager2.playHit();
				cout << "Player hit by enemy!" << endl;
			}

			if (marine.getHealth() <= 0)
			{
				m_ScoreSystem.saveScores();
				resetGame();
				state = State::MAIN_MENU;
				return;
			}
		}
	}

	// Has the marine player touched time warp?
	if (marine.getPosition().intersects(wp.getPosition()))
	{
		// Play warping sound
		soundManager2.playWarp();

		// Reload the level
		m_NewLevelRequired = true;

		// Alter tiles for next level
		setTileSheets(lm.getCurrentLevel());

		// Load the subsequent level
		lm.setCurrentLevel(lm.getCurrentLevel());
		cout << "CURRENT LEVEL: " << lm.getCurrentLevel();

		if (lm.getCurrentLevel() > 3)
		{
			// The game is prepared for finishing
			m_HasReturned = true;
		}

		// DO NOT REMOVE THIS BOOL AS IT ALLOWS STORY TEXT TO UPDATE PROPERLY!
		testBool = true;
		state = State::STORY_MENU;
	}
}