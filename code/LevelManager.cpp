#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TextureHolder.h"
#include <sstream>
#include <fstream>
#include "LevelManager.h"
#include <iostream>

using namespace sf;
using namespace std;

int** LevelManager::nextLevel(VertexArray& rVaLevel)
{
	// Level dimensions
	m_LevelSize.x = 0;
	m_LevelSize.y = 0;

	// Get the next level
	m_CurrentLevel++;

	// Prevent non-existent levels from loading
	if (m_CurrentLevel > NUM_LEVELS)
	{
		m_CurrentLevel = 1;
	}

	// Load the appropriate level from a text file
	string levelToLoad;
	switch (m_CurrentLevel)
	{
		// Set player and warp position for each level
		// Set text file to load for each level
	case 1:
		levelToLoad = "levels/level0-tutorial.txt";
		m_PlayerStartPosition.x = 100;
		m_PlayerStartPosition.y = 900;
		m_WarpStartPosition.x = 550;
		m_WarpStartPosition.y = 150;
		break;

	case 2:
		levelToLoad = "levels/level1-archaic-anarchy.txt";
		m_PlayerStartPosition.x = 400;
		m_PlayerStartPosition.y = 1200;
		m_WarpStartPosition.x = 440;
		m_WarpStartPosition.y = 80;
		break;

	case 3:
		levelToLoad = "levels/level2-wild-west.txt";
		m_PlayerStartPosition.x = 1150;
		m_PlayerStartPosition.y = 380;
		m_WarpStartPosition.x = 860;
		m_WarpStartPosition.y = 950;
		break;

	case 4:
		levelToLoad = "levels/level3-fracture-future.txt";
		m_PlayerStartPosition.x = 250;
		m_PlayerStartPosition.y = 650;
		m_WarpStartPosition.x = 910;
		m_WarpStartPosition.y = 110;
		break;
	}

	// File stream
	ifstream inputFile(levelToLoad);
	string s;

	// Count the number of rows in the file
	while (getline(inputFile, s))
	{
		++m_LevelSize.y;
	}

	// Store the length of the rows
	m_LevelSize.x = s.length();

	// Go back to the start of the file
	inputFile.clear();
	inputFile.seekg(0, ios::beg);

	// Prepare the 2d array to hold the int values from the file
	int** arrayLevel = new int* [m_LevelSize.y];
	for (int i = 0; i < m_LevelSize.y; ++i)
	{
		// Add a new array into each array element
		arrayLevel[i] = new int[m_LevelSize.x];
	}

	// Loop through the file and store all the values in the 2d array
	string row;
	int i = 0;
	while (inputFile >> row)
	{
		for (int j = 0; j < row.length(); j++) {

			const char val = row[j];

			arrayLevel[i][j] = atoi(&val);
		}

		i++;
	}

	// Close the file
	inputFile.close();

	// What type of primitive are we using?
	rVaLevel.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVaLevel.resize(m_LevelSize.x * m_LevelSize.y * VERTS_IN_QUAD);

	// Start at the beginning of the vertex array
	int currentVertex = 0;

	for (int x = 0; x < m_LevelSize.x; x++)
	{
		for (int y = 0; y < m_LevelSize.y; y++)
		{
			// Position each vertex in the current quad
			rVaLevel[currentVertex + 0].position =
				Vector2f(x * TILE_SIZE, y * TILE_SIZE);

			rVaLevel[currentVertex + 1].position =
				Vector2f((x * TILE_SIZE) + TILE_SIZE, y * TILE_SIZE);

			rVaLevel[currentVertex + 2].position =
				Vector2f((x * TILE_SIZE) + TILE_SIZE, (y * TILE_SIZE) + TILE_SIZE);

			rVaLevel[currentVertex + 3].position =
				Vector2f((x * TILE_SIZE), (y * TILE_SIZE) + TILE_SIZE);

			// Which tile from the sprite sheet should we use
			int verticalOffset = arrayLevel[y][x] * TILE_SIZE;

			rVaLevel[currentVertex + 0].texCoords =
				Vector2f(0, 0 + verticalOffset);

			rVaLevel[currentVertex + 1].texCoords =
				Vector2f(TILE_SIZE, 0 + verticalOffset);

			rVaLevel[currentVertex + 2].texCoords =
				Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);

			rVaLevel[currentVertex + 3].texCoords =
				Vector2f(0, TILE_SIZE + verticalOffset);

			// Position ready for the next four vertices
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}

	return arrayLevel;
}

// Get dimensions of level arena
FloatRect LevelManager::getArenaBounds()
{
	// TILE_SIZE is the constant used to build the map
	return sf::FloatRect(
		0,
		0,
		m_LevelSize.x * TILE_SIZE,
		m_LevelSize.y * TILE_SIZE
	);
}

// Find tile size
float LevelManager::getTileSize()
{
	return TILE_SIZE;
}

// Find level size
Vector2i LevelManager::getLevelSize()
{
	return m_LevelSize;
}

// Get level points
Vector2i LevelManager::getLevelSpawningPointsSize()
{
	return m_LevelSpawningPointsSize;
}

// Which level is it?
int LevelManager::getCurrentLevel()
{
	return m_CurrentLevel;
}

// Change level
void LevelManager::setCurrentLevel(int levelNum)
{
	m_CurrentLevel = levelNum;
}

// Get player spawn position
Vector2f LevelManager::getPlayerStartPosition()
{
	return m_PlayerStartPosition;
}

// Get player spawn position
Vector2f LevelManager::getWarpStartPosition()
{
	return m_WarpStartPosition;
}

// Define level spawning points
int** LevelManager::nextLevelSpawningPoints()
{
	m_LevelSpawningPointsSize.x = 0;
	m_LevelSpawningPointsSize.y = 0;

	// Load the appropriate level from a text file
	string levelToLoad;
	switch (m_CurrentLevel)
	{

	case 1:
		levelToLoad = "levels/level0-tutorial.txt";

		break;

	case 2:
		levelToLoad = "levels/level1-archaic-anarchy.txt";

		break;

	case 3:
		levelToLoad = "levels/level2-wild-west.txt";

		break;

	case 4:
		levelToLoad = "levels/level3-fracture-future.txt";

		break;
	}

	ifstream inputFile(levelToLoad);
	string s;

	// Count the number of rows in the file
	while (getline(inputFile, s))
	{
		++m_LevelSpawningPointsSize.y;
	}

	// Store the length of the rows
	m_LevelSpawningPointsSize.x = s.length();

	// Go back to the start of the file
	inputFile.clear();
	inputFile.seekg(0, ios::beg);

	// Prepare the 2d array to hold the int values from the file
	int** arrayLevel = new int* [m_LevelSpawningPointsSize.y];
	for (int i = 0; i < m_LevelSpawningPointsSize.y; ++i)
	{
		// Add a new array into each array element
		arrayLevel[i] = new int[m_LevelSpawningPointsSize.x];
	}

	// Loop through the file and store all the values in the 2d array
	string row;
	int i = 0;
	while (inputFile >> row)
	{
		for (int j = 0; j < row.length(); j++) {

			const char val = row[j];
			arrayLevel[i][j] = atoi(&val);
		}

		i++;
	}

	// close the file
	inputFile.close();

	return arrayLevel;
}
