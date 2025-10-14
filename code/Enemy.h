#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Enemy
{
public:
    Enemy();
    virtual ~Enemy() = default;

    virtual void spawn(float startX, float startY, int seed) = 0;
    virtual bool hit() = 0;
    virtual bool isAlive();
    virtual FloatRect getPosition();
    virtual Sprite getSprite();
    virtual void update(float elapsedTime, Vector2f playerLocation) = 0;

protected:
    Sprite m_Sprite;
    Vector2f m_Position;
    float m_Speed;
    int m_Health;
    bool m_Alive;

    // Constants for speed variation
    static const int MAX_VARRIANCE = 30;
    static const int OFFSET = 85;
};