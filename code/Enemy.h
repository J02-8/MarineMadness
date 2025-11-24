#pragma once

#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;


class Pathfinding;

class Enemy
{
public:
    // Constructor
    Enemy();
    // Destructor
    virtual ~Enemy() = default;

    // Pure virtual methods
    virtual void spawn(float startX, float startY, int seed) = 0;
    virtual bool hit() = 0;
    virtual bool isAlive();
    virtual FloatRect getPosition();
    virtual Sprite getSprite();
    virtual void update(float elapsedTime, Vector2f playerLocation) = 0;
    virtual bool isReadyToShoot() const = 0;
    virtual void resetShootTimer() = 0;
    virtual Vector2f getCenter() const = 0;

    // Pathfinding methods
    virtual void setPathfinding(Pathfinding* pathfinding);
    virtual void updatePath(Vector2f targetPosition);

protected:
    // Variables for all inherited classes
    Sprite m_Sprite;
    Vector2f m_Position;
    float m_Speed;
    int m_Health;
    bool m_Alive;

    // Pathfinding variables - available to all enemies
    vector<Vector2f> m_Path;
    int m_CurrentPathIndex;
    float m_PathUpdateTimer;
    float m_PathUpdateInterval;
    Pathfinding* m_Pathfinding;

    // Constants for speed variation
    static const int MAX_VARRIANCE = 30;
    static const int OFFSET = 85;
};