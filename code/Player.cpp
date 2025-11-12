#include "player.h"
#include "TextureHolder.h"

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	// !!Watch this space!!
	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/player.png"));

	m_Sprite.setTextureRect(sf::IntRect{ 45,65,50,60 });

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(25,25);
}

void Player::setArena(sf::FloatRect arena, float tileSize)
{
	// holds level total size
	m_Arena = arena;

	// holds the size of one tile.
	m_TileSize = tileSize;
}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

void Player::spawn(Vector2f startPosition)
{
	// Place the player in the middle of the arena
	m_Position.x = startPosition.x;
	m_Position.y = startPosition.y;

	m_Sprite.setPosition(m_Position);

}

Time Player::getLastHitTime()
{
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
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

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

int Player::getHealth()
{
	return m_Health;
}

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
	FloatRect r = getPosition();

	// Bottom
	m_Bottom.left = r.left + 3;
	m_Bottom.top = r.top + r.height - 1;
	m_Bottom.width = r.width - 6;
	m_Bottom.height = 1;

	// Top
	m_Top.left = r.left + 3;
	m_Top.top = r.top;
	m_Top.width = r.width - 6;
	m_Top.height = 1;

	// Right
	m_Right.left = r.left + r.width - 1;
	m_Right.top = r.top + r.height * .35;
	m_Right.width = 1;
	m_Right.height = r.height * .3;

	// Left
	m_Left.left = r.left;
	m_Left.top = r.top + r.height * .35;
	m_Left.width = 1;
	m_Left.height = r.height * .3;
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

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);

}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

// Get and set methods for dodge
float Player::getSpeed() {

	return m_Speed;
}

void Player::setSpeed(float speedIn) {

	m_Speed = speedIn;
}






