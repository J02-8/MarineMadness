#include "MarineMachine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "SoundManager.h"

using namespace std;

int score = 0;

float hitDelay = 0.0f;

SoundManager soundManager2;
Time gameTimeTotal;

void MarineMachine::update(float dtAsSeconds)
{
	// Make a note of the players new position
	Vector2f playerPosition(marine.getCenter());

	// Update mouse positions
	mouseScreenPosition = Mouse::getPosition();
	mouseWorldPosition = m_Window.mapPixelToCoords(Mouse::getPosition(), m_MainView);

	// Set crosshair position
	spriteCrosshair.setPosition(mouseWorldPosition);

	// Update the player
	marine.update(dtAsSeconds, mouseWorldPosition);

	// Make a rect for all his parts
	FloatRect detectionZone = marine.getPosition();

	// Make a FloatRect to test each block
	FloatRect block;

	// Allign block dimensions with dimensions of map tiles
	block.width = TILE_SIZE;
	block.height = TILE_SIZE;

	// Build a zone around player to detect collisions
	int startX = (int)(round(detectionZone.left) / TILE_SIZE) - 1;
	int startY = (int)(round(detectionZone.top) / TILE_SIZE) - 1;
	int endX = (int)(round(detectionZone.left) / TILE_SIZE) + 2;
	int endY = (int)round((detectionZone.top) / TILE_SIZE) + 2;
	
	// Make sure we don't test positions lower than zero
	// Or higher than the end of the array
	if (startX < 0)startX = 0;
	if (startY < 0)startY = 0;
	if (endX >= lm.getLevelSize().x)
		endX = lm.getLevelSize().x;
	if (endY >= lm.getLevelSize().y)
		endY = lm.getLevelSize().y;

	// Collision detection
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			block.left = x * TILE_SIZE;
			block.top = y * TILE_SIZE;

			// Is player colliding with a wall?
			if (m_ArrayLevel[y][x] == 0)
			{
				// Check if player collides with wall on left or right sides
				if (marine.getRight().intersects(block))
				{
					marine.stopRight(block.left);
				}
				else if (marine.getLeft().intersects(block))
				{
					marine.stopLeft(block.left);
				}

				// Check if player collides with wall on top or bottom sides
				if (marine.getBottom().intersects(block))
				{
					marine.stopDown(block.top);
				}
				else if (marine.getTop().intersects(block))
				{
					marine.stopUp((block.top));
				}
			}

		}

	}
	
	// Start of a new game we will need a new level
	if (m_NewLevelRequired)
	{
		loadLevel();
	}

	m_MainView.setCenter(marine.getCenter());
	m_Window.setView(m_MainView); // Apply the centered view immediately


	// Update bullets
	for (int i = 0; i < 100; i++)
	{
		if (bullets[i].isInFlight())
		{
			bullets[i].update(dtAsSeconds);
		}
	}

	// Update enemy bullets
	for (int i = 0; i < 100; i++)
	{
		if (enemyBullets[i].isInFlight())
		{
			enemyBullets[i].update(dtAsSeconds);
		}
	}

	// Update enemies
	for (auto& enemy : enemies)
	{
		if (enemy->isAlive())
		{
			enemy->update(dtAsSeconds, playerPosition);

			// Handle enemy shooting only if the enemy type shoots
			if (enemy->isReadyToShoot())
			{
				for (int j = 0; j < 100; ++j)
				{
					if (!enemyBullets[j].isInFlight())
					{
						Vector2f center = enemy->getCenter();
						Vector2f target = marine.getCenter();

						enemyBullets[j].shoot(center.x, center.y, target.x, target.y);

						// Different sound based on type
						string typeName = typeid(*enemy).name();
						if (typeName.find("Cowboy") != string::npos)
						{
							soundManager2.playShoot();
						}
						else if (typeName.find("Android") != string::npos)
						{
							soundManager2.playLazer();
						}
							
						enemy->resetShootTimer();
						break;
					}
				}
			}
		}
	}

	//update the dodge variables and reset the player
	if (isDodging && (gameTimeTotal - lastDodgeTime >= dodgeDuration)) {
		isDodging = false;
		marine.setSpeed(originalSpeed);
	}
	
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
					score += 15;
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