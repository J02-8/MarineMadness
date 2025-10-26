#include "Warp.h"
#include "TextureHolder.h"

Warp::Warp()
{

	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/warp.png"));

	m_Sprite.setOrigin(25, 25);
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
	// holds level total size
	m_Arena = arena;

	// holds the size of one tile.
	m_TileSize = tileSize;
}

FloatRect Warp::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Warp::getSprite()
{
	return m_Sprite;
}

bool Warp::isSpawned()
{
	return m_Spawned;
}
