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
