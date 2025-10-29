#include "MarineMachine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "SoundManager.h"

using namespace sf;
//using namespace std;

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

	// Loop through and update dinos
	if (dinosaurs != nullptr)
	{
		for (int i = 0; i < numDinosaurs; i++)
		{
			if (dinosaurs[i]->isAlive())
			{
				dinosaurs[i]->update(dtAsSeconds, playerPosition);
			}
		}
	}

	
	// Loop through and update cowboys
	if (cowboys != nullptr)
	{
		for (int i = 0; i < numCowboys; i++)
		{
			if (cowboys[i]->isAlive())
			{
				cowboys[i]->update(dtAsSeconds, playerPosition);
			
				// Handle cowboy shooting
				if (cowboys[i]->isReadyToShoot()) {
					// Find an available bullet
					for (int j = 0; j < 100; j++) {
						if (!enemyBullets[j].isInFlight()) {

							soundManager2.playShoot();
							// Shoot from cowboy's center towards player position
							Vector2f cowboyCenter = cowboys[i]->getCenter();

							// Shoot towards player position
							Vector2f playerPos = marine.getCenter();
							enemyBullets[j].shoot(
								cowboyCenter.x,
								cowboyCenter.y,
								playerPos.x,
								playerPos.y
							);


							cout << "Bullet " << j << " fired from cowboy " << i << endl;

							// Reset the cowboy's shoot timer
							cowboys[i]->resetShootTimer();
							break;
						}
					}
				}
			
			
			}
		}
	}


	// Loop through and update androids
	if (androids != nullptr)
	{
		for (int i = 0; i < numAndroids; i++)
		{
			if (androids[i]->isAlive())
			{
				androids[i]->update(dtAsSeconds, playerPosition);

				// Handle android shooting
				if (androids[i]->isReadyToShoot()) {
					// Find an available bullet
					for (int j = 0; j < 100; j++) {
						if (!enemyBullets[j].isInFlight()) {

							soundManager2.playShoot();
							// Shoot from cowboy's center towards player position
							Vector2f androidCenter = androids[i]->getCenter();

							// Shoot towards player position
							Vector2f playerPos = marine.getCenter();
							enemyBullets[j].shoot(
								androidCenter.x,
								androidCenter.y,
								playerPos.x,
								playerPos.y
							);


							cout << "Bullet " << j << " fired from android " << i << endl;

							// Reset the cowboy's shoot timer
							androids[i]->resetShootTimer();
							break;
						}
					}
				}

			}
		}
	}
	
	// COLLISION DETECTION - Bullets hitting dinosaurs
	if (dinosaurs != nullptr)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < numDinosaurs; j++)
			{
				if (bullets[i].isInFlight() && dinosaurs[j]->isAlive())
				{
					if (bullets[i].getPosition().intersects(dinosaurs[j]->getPosition()))
					{
						// Stop the bullet
						bullets[i].stop();

						// Register the hit
						if (dinosaurs[j]->hit())
						{
							// Dinosaur killed
							score += 15;
							numDinosaursAlive--;

							 std::cout << "Dinosaur killed! Remaining: " << numDinosaursAlive << std::endl;

							
						}
						// Play splat sound
						 soundManager2.playSplat();
					}
				}
			}
		}
	}

	// COLLISION DETECTION - Bullets hitting cowboys
	if (cowboys != nullptr)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < numCowboys; j++)
			{
				if (bullets[i].isInFlight() && cowboys[j]->isAlive())
				{
					if (bullets[i].getPosition().intersects(cowboys[j]->getPosition()))
					{
						// Stop the bullet
						bullets[i].stop();

						// Register the hit
						if (cowboys[j]->hit())
						{
							// cowboy killed
							score += 15;
							numCowboysAlive--;

							std::cout << "cowboy killed! Remaining: " << numCowboysAlive << std::endl;

							
						}
						// Play splat sound
						 soundManager2.playSplat();
					}
				}
			}
		}
	}


	// COLLISION DETECTION - Bullets hitting androids
	if (androids != nullptr)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < numAndroids; j++)
			{
				if (bullets[i].isInFlight() && androids[j]->isAlive())
				{
					if (bullets[i].getPosition().intersects(androids[j]->getPosition()))
					{
						// Stop the bullet
						bullets[i].stop();

						// Register the hit
						if (androids[j]->hit())
						{
							// cowboy killed
							score += 15;
							numCowboysAlive--;

							std::cout << "cowboy killed! Remaining: " << numCowboysAlive << std::endl;


						}
						// Play splat sound
						soundManager2.playSplat();
					}
				}
			}
		}
	}



	// COLLISION DETECTION - cowboy bullets hitting player 
	if (cowboys != nullptr)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < numCowboys; j++)
			{
				if (enemyBullets[i].isInFlight() && marine.getHealth()>0)
				{
					if (enemyBullets[i].getPosition().intersects(marine.getPosition()))
					{
						// Stop the bullet
						bullets[i].stop();

						if (marine.hit(gameTimeTotal))
						{
							std::cout << "Player hit by cowboy bullet!" << std::endl;
							// Play hit sound
							 soundManager2.playHit();
						}

						if (marine.getHealth() <= 0)
						{
							std::cout << "Player dead! Game Over." << std::endl;
							state = State::GAME_OVER;
						}
						
					}
						
				}
				
			}
		}
	}


	// COLLISION DETECTION - android bullets hittting player 
	if (androids != nullptr)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < numAndroids; j++)
			{
				if (enemyBullets[i].isInFlight() && marine.getHealth() > 0)
				{
					if (enemyBullets[i].getPosition().intersects(marine.getPosition()))
					{
						// Stop the bullet
						bullets[i].stop();

						if (marine.hit(gameTimeTotal))
						{
							std::cout << "Player hit by android bullet!" << std::endl;
							// Play hit sound
							soundManager2.playHit();
						}

						if (marine.getHealth() <= 0)
						{
							std::cout << "Player dead! Game Over." << std::endl;
							state = State::GAME_OVER;
						}

					}

				}

			}
		}
	}

	// COLLISION DETECTION - Dinosaurs touching player
	if (dinosaurs != nullptr)
	{
		for (int i = 0; i < numDinosaurs; i++)
		{
			if (marine.getPosition().intersects(dinosaurs[i]->getPosition()) &&
				dinosaurs[i]->isAlive())
			{
				if (marine.hit(gameTimeTotal))
				{
					std::cout << "Player hit by dinosaur!" << std::endl;
					// Play hit sound
					 soundManager2.playHit();
				}

				if (marine.getHealth() <= 0)
				{
					std::cout << "Player dead! Game Over." << std::endl;
					state = State::GAME_OVER;
				}
			}
		}
	}

	// COLLISION DETECTION - cowboys touching player
	if (cowboys != nullptr)
	{
		for (int i = 0; i < numCowboys; i++)
		{
			if (marine.getPosition().intersects(cowboys[i]->getPosition()) &&
				cowboys[i]->isAlive())
			{
				if (marine.hit(gameTimeTotal))
				{
					std::cout << "Player hit by cowboy!" << std::endl;
					// Play hit sound
					 soundManager2.playHit();
				}

				if (marine.getHealth() <= 0)
				{
					std::cout << "Player dead! Game Over." << std::endl;
					state = State::GAME_OVER;
				}
			}
		}
	}

	// COLLISION DETECTION - androids touching player
	if (androids != nullptr)
	{
		for (int i = 0; i < numAndroids; i++)
		{
			if (marine.getPosition().intersects(androids[i]->getPosition()) &&
				androids[i]->isAlive())
			{
				if (marine.hit(gameTimeTotal))
				{
					std::cout << "Player hit by android!" << std::endl;
					// Play hit sound
					soundManager2.playHit();
				}

				if (marine.getHealth() <= 0)
				{
					std::cout << "Player dead! Game Over." << std::endl;
					state = State::GAME_OVER;
				}
			}
		}
	}



	// Has the marine player touched time warp?
	if (marine.getPosition().intersects(wp.getPosition()))
	{
		// Reload the level
		m_NewLevelRequired = true;
		
		// Go to next level
		levelNum++;

		if (levelNum > 4)
		{
			// Avoid non-existent levels
			levelNum = 0;
		}

		// Alter tiles for next level
		MarineMachine::setTileSheets(levelNum);
		
		// Load the subsequent level
		lm.setCurrentLevel(levelNum);
		std::cout << "CURRENT LEVEL: " << levelNum;
	}
	
}