#include "Android.h"
#include "TextureHolder.h"
#include <cmath> 
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;

Android::Android()
{
    // Initialize all member variables
    m_Position = sf::Vector2f(0, 0);
    m_Speed = 0;
    m_Health = 0;
    m_Alive = false;
    m_SizeModifier = 1.0f;

    // Initialize shooting variables
    m_ShootCooldown = 0.0f;
    m_TimeSinceLastShot = 0.0f;
    m_CanShoot = true;
}

void Android::spawn(float startX, float startY, int seed)
{
    // Single android type 
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/android.png"));
    m_Speed = 45;
    m_Health = 6;

    // Apply variations
    applySpeedVariation(seed);

    // Initialize shooting with random cooldown (1-3 seconds)
    std::srand(static_cast<unsigned int>(std::time(0)) * seed);
    m_ShootCooldown = (std::rand() % 2000 + 1000) / 1000.0f; // 1-3 seconds
    m_TimeSinceLastShot = 0.0f;
    m_CanShoot = true;

    m_Position.x = startX;
    m_Position.y = startY;
    m_Alive = true;


}

bool Android::hit()
{
    m_Health--;

    if (m_Health <= 0)
    {
        // dead
        m_Alive = false;
        m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
        return true;
    }

    // injured but not dead yet
    return false;
}

void Android::update(float elapsedTime, sf::Vector2f playerLocation) {
    if (!m_Alive) return;

    //update the shoot timer
    m_TimeSinceLastShot += elapsedTime;


    // Calculate direction to player
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

    // Update the cowboy position
    if (playerX > m_Position.x)
    {
        m_Position.x = m_Position.x + m_Speed * elapsedTime;
    }

    if (playerY > m_Position.y)
    {
        m_Position.y = m_Position.y + m_Speed * elapsedTime;
    }

    if (playerX < m_Position.x)
    {
        m_Position.x = m_Position.x - m_Speed * elapsedTime;
    }

    if (playerY < m_Position.y)
    {
        m_Position.y = m_Position.y - m_Speed * elapsedTime;
    }




    // Move the sprite
    m_Sprite.setPosition(m_Position);

    // Face the sprite in the correct direction
    float angle = (atan2(playerY - m_Position.y,
        playerX - m_Position.x)
        * 180) / 3.141f;

    m_Sprite.setRotation(angle);
}

void Android::applySpeedVariation(int seed)
{
    // FIXED: Use a better random approach
    std::srand(static_cast<unsigned int>(std::time(0)) * seed);
    float modifier = (std::rand() % 20) + 90; // 90-110% variation
    modifier /= 100;
    m_Speed *= modifier;
}

//  method to get shooting state
bool Android::isReadyToShoot() const {
    return m_CanShoot && m_TimeSinceLastShot >= m_ShootCooldown;
}

//  method to reset shooting timer after shooting
void Android::resetShootTimer() {
    m_TimeSinceLastShot = 0.0f;
    m_ShootCooldown = (std::rand() % 2000 + 1000) / 1000.0f; // 1-3 seconds
}

//method to get cowboy position
sf::Vector2f Android::getCenter() const {
    return m_Position;
}



