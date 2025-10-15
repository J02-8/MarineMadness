#pragma once

#include "Enemy.h"
#include <cstdlib>
#include <ctime>

class Dinosaur : public Enemy
{
public:
    Dinosaur();
    void spawn(float startX, float startY, int seed) override;
    bool hit() override;
    void update(float elapsedTime, Vector2f playerLocation) override;


private:
    void applySpeedVariation(int seed);
    void applySizeVariation(int seed);

    // Dinosaur-specific properties
    float m_SizeModifier;

};
