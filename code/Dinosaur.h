#pragma once

#include "Enemy.h"
#include <cstdlib>
#include <ctime>

class Dinosaur : public Enemy   // Dinosaur inherits from Enemy
{
public:
    // Constructor
    Dinosaur();
    // Dinosaur spawn function
    void spawn(float startX, float startY, int seed) override;
    // Hit function
    bool hit() override;
    // Dinosaur update function
    void update(float elapsedTime, Vector2f playerLocation) override;
    // Overriden shoot function
    bool isReadyToShoot(void) const override;
    // Reset shoot timer function
    void resetShootTimer(void) override;
    // Get center of dinosaur
    Vector2<float> getCenter(void) const override;

private:
    // Dinosaur-specific properties
    void applySpeedVariation(int seed);
    void applySizeVariation(int seed);

    float m_SizeModifier;

};
