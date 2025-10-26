#include "MarineMachine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

using namespace sf;

int score = 0;

float hitDelay = 0.0f;

int levelNum = 0;

void MarineMachine::update(float dtAsSeconds)
{
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
	marine.update(dtAsSeconds, Mouse::getPosition());

	m_MainView.setCenter(marine.getCenter());
	m_Window.setView(m_MainView); // Apply the centered view immediately

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
		cout << "CURRENT LEVEL: " << levelNum;
	}

}