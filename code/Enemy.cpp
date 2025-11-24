#include "Enemy.h"
#include "Pathfinding.h"
#include <cmath>

Enemy::Enemy() : m_Speed(0), m_Health(0), m_Alive(false),
m_CurrentPathIndex(0), m_PathUpdateTimer(0.0f),
m_PathUpdateInterval(0.5f), m_Pathfinding(nullptr)
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

void Enemy::setPathfinding(Pathfinding* pathfinding)
{
    m_Pathfinding = pathfinding;
}

void Enemy::updatePath(Vector2f targetPosition)
{
    if (m_Pathfinding == nullptr) return;

    m_Path = m_Pathfinding->findPath(m_Position, targetPosition);
    m_CurrentPathIndex = 0;

    // Skip the first waypoint if it's too close to current position
    if (!m_Path.empty()) {
        Vector2f firstWaypoint = m_Path[0];
        float dx = firstWaypoint.x - m_Position.x;
        float dy = firstWaypoint.y - m_Position.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 5.0f && m_Path.size() > 1) {
            m_CurrentPathIndex = 1;
        }
    }
}