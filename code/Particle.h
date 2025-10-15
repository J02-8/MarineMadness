#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "ParticleFlyweight.h"
#include <iostream>

using namespace std;
using namespace sf;

class Particle 
{
public:
    Particle(shared_ptr<ParticleFlyweight> flyweight, const Vector2f& position, const Vector2f& velocity);

    void update(float dt);
    void checkCollision(Particle& other);
    void draw(sf::RenderWindow& window);

    Vector2f position;
    Vector2f velocity;

private:
    shared_ptr<ParticleFlyweight> flyweight;
};