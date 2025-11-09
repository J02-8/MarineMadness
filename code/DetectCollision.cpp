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
				// Respawn the player
				lm.setCurrentLevel(lm.getCurrentLevel());
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
				// Respawn the player
				lm.setCurrentLevel(lm.getCurrentLevel());
			}
		}
	}

	// Has the marine player touched time warp?
	if (marine.getPosition().intersects(wp.getPosition()))
	{
		// Reload the level
		m_NewLevelRequired = true;

		// Alter tiles for next level
		MarineMachine::setTileSheets(lm.getCurrentLevel());

		// Load the subsequent level
		lm.setCurrentLevel(lm.getCurrentLevel());
		cout << "CURRENT LEVEL: " << lm.getCurrentLevel();
	}
}