#pragma once

#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include "Pathfinding.h"

class Android : public Enemy    // Android inherits from Enemy
{
public:
    // Constructor
    Android();
    // Android spawn function
    void spawn(float startX, float startY, int seed) override;
    // Hit function
    bool hit() override;
    // Android update function
    void update(float elapsedTime, Vector2f playerLocation) override;
    // Overriden shoot function
    bool isReadyToShoot() const override;
    // Reset function
    void resetShootTimer() override;
    // Get center of android
    Vector2f getCenter() const override;

private:
    // Private variables and functions
    // Android-specific properties
    void applySpeedVariation(int seed);

    Clock m_ShootTimer;
    float m_ShootCooldown;
    float m_TimeSinceLastShot;
    bool m_CanShoot;

    float m_SizeModifier;

};