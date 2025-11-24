#include "MarineMachine.h"
#include "EnemySpawner.h"
#include "SoundManager.h"

void MarineMachine::loadLevel()
{
	// Arena dimensions
	arena.width = 500;
	arena.height = 500;

	int level = lm.getCurrentLevel();
	setTileSheets(level);
	int count = 5; // Enemy count

	// Set specific enemies for each levels
	switch (level)
	{
	case 1:
		EnemySpawner<Dinosaur>(enemies, count, arena);
		break;
	case 2:
		EnemySpawner<Cowboy>(enemies, count, arena);
		break;
	case 3:
		EnemySpawner<Android>(enemies, count, arena);
		break;
	default:
		enemies.clear();
		break;
	}

	numEnemiesAlive = enemies.size();
	m_Playing = false;

	// Delete the previously allocated memory
	for (int i = 0; i < lm.getLevelSize().y; ++i)
	{
		delete[] m_ArrayLevel[i];

	}
	delete[] m_ArrayLevel;

	// Load the next 2d array with the map for the level
	// And repopulate the vertex array as well
	m_ArrayLevel = lm.nextLevel(vaLevel);

	// Initialize pathfinding with level data (add after m_ArrayLevel is loaded)
	m_Pathfinding->setLevelData(m_ArrayLevel, lm.getLevelSize(), lm.getTileSize());

	// Get level's pixel bounds from LevelManager. 
	FloatRect arenaBounds = lm.getArenaBounds();

	// Get tile size
	float tileSize = lm.getTileSize();

	// Pass the arena data to player
	marine.setArena(arenaBounds, tileSize);

	// Pass arena date to warp
	wp.setArena(arenaBounds, tileSize);

	// Delete previously allocated memory
	for (int i = 0; i < lm.getLevelSpawningPointsSize().y; ++i)
	{
		delete[] m_ArraySpawningPointsLevel[i];

	}
	delete[] m_ArraySpawningPointsLevel;

	// Load spawning points data
	m_ArraySpawningPointsLevel = lm.nextLevelSpawningPoints();

	// Spawn Player
	marine.spawn(lm.getPlayerStartPosition());

	// Spawn Warp
	wp.spawn(lm.getWarpStartPosition());

	// Make sure this code isn't run again
	m_NewLevelRequired = false;

	// Add observer
	m_ScoreSystem.addObserver(this);
}