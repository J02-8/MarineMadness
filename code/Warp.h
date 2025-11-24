#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Warp
{
private:
	// The sprite that represents this warp
	Sprite m_Sprite;

	// Where is the warp
	Vector2f m_Position;

	// What size is the current arena
	FloatRect m_Arena;

	// How big is each tile of the arena
	int m_TileSize;

	// Public prototypes go here
public:
	// Constructor
	Warp();

	// Spawn warp in given location
	void spawn(Vector2f spwn);

	// Allow warp to be spawned on level grid
	void setArena(sf::FloatRect arena, float tileSize);

	// Check the position of a warp
	FloatRect getPosition();

	// Get the sprite for drawing
	Sprite getSprite();

	// Let the war update itself each frame
	void update(float elapsedTime);

};



