#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;


class LevelManager
{
private:
	Vector2i m_LevelSize;
	Vector2i m_LevelSpawningPointsSize;
	Vector2f m_PlayerStartPosition;
	Vector2f m_WarpStartPosition;
	int m_CurrentLevel = 0;
	const int NUM_LEVELS = 5;

public:

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;
	static const int TILE_WALL = 0;

	Vector2f getPlayerStartPosition();
	Vector2f getWarpStartPosition();

	int** nextLevel(VertexArray& rVaLevel);
	int** nextLevelSpawningPoints();

	Vector2i getLevelSize();
	Vector2i getLevelSpawningPointsSize();

	FloatRect getArenaBounds();
	float getTileSize();

	int getCurrentLevel();
	void setCurrentLevel(int levelNum);
};
