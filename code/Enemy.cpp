#include "Enemy.h"

Enemy::Enemy() : m_Speed(0), m_Health(0), m_Alive(false)
{
}


bool Enemy::isAlive()
{
    return m_Alive;
}

FloatRect Enemy::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

Sprite Enemy::getSprite()
{
    return m_Sprite;
}
