#include "MarineMachine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "SoundManager.h"

using namespace std;

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

	// Make a rect for all his parts
	FloatRect detectionZone = marine.getPosition();

	// Make a FloatRect to test each block
	FloatRect block;

	block.width = TILE_SIZE;
	block.height = TILE_SIZE;

	// Build a zone around Enemy to detect collisions

// 21 11 2022 introducded rounding to allow for small differences
	int startX = (int)(round(detectionZone.left) / TILE_SIZE) - 1;
	int startY = (int)(round(detectionZone.top) / TILE_SIZE) - 1;
	//02/12/21 changed 2 to 1 for Endx
	int endX = (int)(round(detectionZone.left) / TILE_SIZE) + 2;
	int endY = (int)round((detectionZone.top) / TILE_SIZE) + 2;

	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			/*
			if (lm.hasHitWall(x,y))
			{

			}
			// Initialize the starting position of the current block

			block.left = x * TILE_SIZE;
			block.top = y * TILE_SIZE;

			// Is character colliding with a regular block i.e platform
			if (marine.getPosition().intersects(block))
			{
				marine.stopRight();
			}
			*/

		}

	}

	//start of a new game we will need a new level
	if (m_NewLevelRequired)
	{
		loadLevel();
	}

	// Update the player
	marine.update(dtAsSeconds, mouseWorldPosition);

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
	
	// Check for collision
	detectCollision();
	
}