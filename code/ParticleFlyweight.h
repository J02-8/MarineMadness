#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class ParticleFlyweight 
{
public:
    ParticleFlyweight(float radius);

    CircleShape& getShape();
    float getRadius() const;

private:
    CircleShape shape;
};