#pragma once

#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include "Pathfinding.h"

using namespace std;

class Cowboy : public Enemy
{
public:
    Cowboy();
    void spawn(float startX, float startY, int seed) override;
    bool hit() override;
    void update(float elapsedTime, Vector2f playerLocation) override;
    bool isReadyToShoot() const override;
    void resetShootTimer() override;
    sf::Vector2f getCenter() const override;

    


private:
    void applySpeedVariation(int seed);

    //shooting variables
    sf::Clock m_ShootTimer;
    float m_ShootCooldown;
    float m_TimeSinceLastShot;
    bool m_CanShoot;

    // cowboy-specific properties
    float m_SizeModifier;

    

};
