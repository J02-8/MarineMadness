#include "ParticleFlyweight.h"
#include <iostream>

using namespace std;
using namespace sf;

ParticleFlyweight::ParticleFlyweight(float radius) : shape(radius) 
{
    shape.setFillColor(sf::Color::White);
}

CircleShape& ParticleFlyweight::getShape() 
{
    return shape;
}

float ParticleFlyweight::getRadius() const 
{
    return shape.getRadius();
}