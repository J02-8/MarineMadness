#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	// Player health and speed
	const float START_SPEED = 200;
	const float START_HEALTH = 150;

	// Where is the player
	Vector2f m_Position;

	// Of course we will need a sprite
	Sprite m_Sprite;

	// And a texture
	Texture m_Texture;

	// What is the screen resolution
	Vector2f m_Resolution;

	// What size is the current arena
	FloatRect m_Arena;

	// Where are the player's various sides?
	FloatRect m_Bottom;
	FloatRect m_Top;
	FloatRect m_Right;
	FloatRect m_Left;

	// How big is each tile of the arena
	int m_TileSize;

	// Which directions is the player currently moving in?
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// Which directions is the player permitted to move in?
	bool m_AllowGoingUp;
	bool m_AllowGoingDown;
	bool m_AllowGoingLeft;
	bool m_AllowGoingRight;

	// How much health has the player got?
	int m_Health;

	// What is the maximum health the player can have
	int m_MaxHealth;

	// When was the player last hit
	Time m_LastHit;

	// Speed in pixels per second
	float m_Speed;

	// All our public functions will come next
public:

	// Construcor
	Player();

	// Call this at the end of every game
	void resetPlayerStats();

	void spawn(Vector2f startPosition);

	void setArena(FloatRect arena, float tileSize);

	// Handle the player getting hit by a zombie
	bool hit(Time timeHit);

	// How long ago was the player last hit
	Time getLastHitTime();

	// A rectangle representing the position for different parts of the sprite
	FloatRect getBottom();
	FloatRect getTop();
	FloatRect getRight();
	FloatRect getLeft();

	// Where is the player
	FloatRect getPosition();

	// Where is the center of the player
	Vector2f getCenter();

	// Which angle is the player facing
	float getRotation();

	// Send a copy of the sprite to main
	Sprite getSprite();

	// How much health has the player currently got?
	int getHealth();

	// The next four functions move the player
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	// Update player sides
	void updateLeftRightTopBottom();

	// Stop the player moving in a specific direction
	void stopLeftMovement();

	void stopRightMovement();

	void stopUpMovement();

	void stopDownMovement();

	// Stop the player moving when wall collision
	void stopLeft(float pos);

	void stopRight(float pos);

	void stopUp(float pos);

	void stopDown(float pos);

	// We will call this function once every frame
	void update(float elapsedTime, Vector2f mousePosition);

	// Get player speed
	float getSpeed();

	// Set player speed
	void setSpeed(float speedIn);

	// Reset player health
	void resetHealth();
};