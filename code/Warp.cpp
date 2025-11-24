#include "Warp.h"
#include "TextureHolder.h"

Warp::Warp()
{
	// Associate texture with sprite
	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/warp.png"));
}


void Warp::spawn(Vector2f startPosition)
{
	// Place the player in the middle of the arena
	m_Position.x = startPosition.x;
	m_Position.y = startPosition.y;

	m_Sprite.setPosition(m_Position);
}

void Warp::setArena(sf::FloatRect arena, float tileSize)
{
	// Holds level total size
	m_Arena = arena;

	// Holds the size of one tile.
	m_TileSize = tileSize;
}

FloatRect Warp::getPosition()
{
	// Get warp position
	return m_Sprite.getGlobalBounds();
}

Sprite Warp::getSprite()
{
	// Get warp sprite
	return m_Sprite;
}

