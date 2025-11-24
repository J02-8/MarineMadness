#include "player.h"
#include "TextureHolder.h"

Player::Player()
{
	// Health and speed definition
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/player.png"));
	// Load specific sprite from spritesheet and attach it to player 
	m_Sprite.setTextureRect(sf::IntRect{ 45,65,50,60 });

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(25,25);
}

void Player::setArena(sf::FloatRect arena, float tileSize)
{
	// Holds level total size
	m_Arena = arena;

	// Holds the size of one tile.
	m_TileSize = tileSize;
}

void Player::resetPlayerStats()
{
	// Health and speed reset
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

void Player::spawn(Vector2f startPosition)
{
	// Place the player in the arena
	m_Position.x = startPosition.x;
	m_Position.y = startPosition.y;

	m_Sprite.setPosition(m_Position);
}

Time Player::getLastHitTime()
{
	// When was player last hit?
	return m_LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)// 2 tenths of second
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		if (m_Health < 0)
		{
			m_Health = 0;
		}
		return true;
	}
	else
	{
		return false;
	}
}

FloatRect Player::getPosition()
{
	// Get player position
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	// Get player center
	return m_Position;
}

float Player::getRotation()
{
	// Get player rotation
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	// Get sprite
	return m_Sprite;
}

// Get player box sides
FloatRect Player::getBottom()
{
	return m_Bottom;
}

FloatRect Player::getTop()
{
	return m_Top;
}

FloatRect Player::getLeft()
{
	return m_Left;
}

FloatRect Player::getRight()
{
	return m_Right;
}

// Get health
int Player::getHealth()
{
	return m_Health;
}

// Movement
void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

// Stop movement when key released
void Player::stopLeftMovement()
{
	m_LeftPressed = false;
}

void Player::stopRightMovement()
{
	m_RightPressed = false;
}

void Player::stopUpMovement()
{
	m_UpPressed = false;
}

void Player::stopDownMovement()
{
	m_DownPressed = false;
}

// Stop player on collision with wall tile
void Player::stopLeft(float pos)
{
	m_AllowGoingLeft = false;
	m_Position.x = pos + getPosition().width;
	m_Sprite.setPosition(m_Position);
}

void Player::stopRight(float pos)
{
	m_AllowGoingRight = false;
	m_Position.x = pos - getPosition().width;
	m_Sprite.setPosition(m_Position);
}

void Player::stopUp(float pos)
{
	m_AllowGoingUp = false;
	m_Position.y = pos + getPosition().height;
	m_Sprite.setPosition(m_Position);
}

void Player::stopDown(float pos)
{
	m_AllowGoingDown = false;
	m_Position.y = pos - getPosition().height;
	m_Sprite.setPosition(m_Position);
}

// Update parts
void Player::updateLeftRightTopBottom()
{
	// Collision box
	FloatRect r = getPosition();

	// Bottom dimensions
	m_Bottom.left = r.left + 3 + 10;
	m_Bottom.top = r.top + r.height - 1 - 10;
	m_Bottom.width = r.width - 6 - 20;
	m_Bottom.height = 1;

	// Top dimensions
	m_Top.left = r.left + 3 + 10;
	m_Top.top = r.top + 10;
	m_Top.width = r.width - 6 - 20;
	m_Top.height = 1;

	// Right dimensions
	m_Right.left = r.left + r.width - 1 - 10;
	m_Right.top = r.top + 10 + (r.height - 20) * .35;
	m_Right.width = 1;
	m_Right.height = (r.height - 20) * .3;

	// Left dimensions
	m_Left.left = r.left + 10;
	m_Left.top = r.top + 10 + (r.height - 20) * .35;
	m_Left.width = 1;
	m_Left.height = (r.height - 20) * .3;
}

void Player::update(float elapsedTime, Vector2f mousePosition)
{
	// Allow movement
	m_AllowGoingUp = true;
	m_AllowGoingDown = true;
	m_AllowGoingLeft = true;
	m_AllowGoingRight = true;

	// Move player up
	if (m_UpPressed && m_AllowGoingUp)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}

	// Move player down
	if (m_DownPressed && m_AllowGoingDown)
	{
		m_Position.y += m_Speed * elapsedTime;
	}

	// Move player right
	if (m_RightPressed && m_AllowGoingRight)
	{
		m_Position.x += m_Speed * elapsedTime;
	}

	// Move player left
	if (m_LeftPressed && m_AllowGoingLeft)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}

	// Set sprite position
	m_Sprite.setPosition(m_Position);

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Position.y ,
		mousePosition.x - m_Position.x )
		* 180) / 3.141;

	m_Sprite.setRotation(angle);

	// Update the sprite position
	m_Sprite.setPosition(m_Position);

	// Update parts
	updateLeftRightTopBottom();
}

// Get and set methods for dodge
float Player::getSpeed() {

	return m_Speed;
}

void Player::setSpeed(float speedIn) {

	m_Speed = speedIn;
}

// Reset player health
void Player::resetHealth()
{
	m_Health = START_HEALTH;
}