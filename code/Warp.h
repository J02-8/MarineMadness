#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Warp
{
private:
	//Start value for health pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 6;
	const int START_SECONDS_TO_LIVE = 5;

	// The sprite that represents this pickup
	Sprite m_Sprite;

	// Where is the player
	Vector2f m_Position;

	// Handle spawning and disappearing
	bool m_Spawned;

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

	// Check the position of a pickup
	FloatRect getPosition();

	// Get the sprite for drawing
	Sprite getSprite();

	// Let the pickup update itself each frame
	void update(float elapsedTime);

	// Is this pickup currently spawned?
	bool isSpawned();

};



