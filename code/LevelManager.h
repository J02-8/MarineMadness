#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;


class LevelManager
{
private:
	// Properties for each level
	Vector2i m_LevelSize;
	Vector2i m_LevelSpawningPointsSize;
	Vector2f m_PlayerStartPosition;
	Vector2f m_WarpStartPosition;
	int m_CurrentLevel = 0;
	const int NUM_LEVELS = 5;

public:
	// Level dimensions
	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;
	static const int TILE_WALL = 0;

	// Position of marine and warp
	Vector2f getPlayerStartPosition();
	Vector2f getWarpStartPosition();

	// A pointer to a pointer
	int** nextLevel(VertexArray& rVaLevel);
	int** nextLevelSpawningPoints();

	// Level size 
	Vector2i getLevelSize();
	Vector2i getLevelSpawningPointsSize();

	// Level arena functions
	FloatRect getArenaBounds();
	float getTileSize();

	// Functions for specific level
	int getCurrentLevel();
	void setCurrentLevel(int levelNum);
};
