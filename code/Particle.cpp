#include "Particle.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

Particle::Particle(shared_ptr<ParticleFlyweight> flyweight, const Vector2f& position, const Vector2f& velocity)
    : flyweight(flyweight), position(position), velocity(velocity) {}

void Particle::update(float dt) 
{
    position += velocity * dt; // Update position based on velocity

    // Handle wall collisions (wrap around or bounce off)
    if (position.x < 0 || position.x > 1920) velocity.x = -velocity.x;
    if (position.y < 0 || position.y > 1080) velocity.y = -velocity.y;
}

void Particle::checkCollision(Particle& other) 
{
    Vector2f diff = other.position - position;
    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
    float minDist = flyweight->getRadius() + other.flyweight->getRadius();

    if (dist < minDist && dist > 0) 
    {
        Vector2f collisionNormal = diff / dist;
        Vector2f relativeVelocity = velocity - other.velocity;
        float velocityAlongNormal = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;

        if (velocityAlongNormal > 0) return;

        velocity -= collisionNormal * velocityAlongNormal;
        other.velocity += collisionNormal * velocityAlongNormal;
    }
}

void Particle::draw(RenderWindow& window) 
{
    CircleShape& shape = flyweight->getShape();
    shape.setPosition(position);
    window.draw(shape);
}