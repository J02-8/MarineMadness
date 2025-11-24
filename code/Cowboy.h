#pragma once

#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include "Pathfinding.h"

using namespace std;

class Cowboy : public Enemy // Cowboy inherits from Enemy
{
public:
    // Constructor
    Cowboy();
    // Cowboy spawn function
    void spawn(float startX, float startY, int seed) override;
    // Hit function
    bool hit() override;
    // Cowboy update function
    void update(float elapsedTime, Vector2f playerLocation) override;
    // Overriden shoot function
    bool isReadyToShoot() const override;
    // Reset shoot timer function
    void resetShootTimer() override;
    // Get center of cowboy
    Vector2f getCenter() const override;

private:
    // Private variables and functions
    // Cowboy-specific properties
    void applySpeedVariation(int seed);

    // Shooting variables
    Clock m_ShootTimer;
    float m_ShootCooldown;
    float m_TimeSinceLastShot;
    bool m_CanShoot;

    float m_SizeModifier;
};
