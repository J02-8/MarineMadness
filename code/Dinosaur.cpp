#include "Dinosaur.h"
#include "TextureHolder.h"
#include <cmath> 
#include <cstdlib>
#include <ctime>

Dinosaur::Dinosaur()
{
    // Initialize all member variables
    m_Position = sf::Vector2f(0, 0);
    m_Speed = 0;
    m_Health = 0;
    m_Alive = false;
    m_SizeModifier = 1.0f;
}

void Dinosaur::spawn(float startX, float startY, int seed)
{
    m_SizeModifier = 1.0f;

    // Single dinosaur type 
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/dinosaur.png"));
    m_Speed = 45;
    m_Health = 3;

    // Apply variations
    applySpeedVariation(seed);
    applySizeVariation(seed);

    m_Position.x = startX;
    m_Position.y = startY;
    m_Alive = true;

    // Set origin to center and apply size variation
    m_Sprite.setOrigin(25, 25);
    m_Sprite.setScale(m_SizeModifier, m_SizeModifier);
    m_Sprite.setPosition(m_Position);
}

bool Dinosaur::hit()
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

void Dinosaur::update(float elapsedTime, sf::Vector2f playerLocation) {
    if (!m_Alive) return;

    // Calculate direction to player
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

    // Update the dinosaur position
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

void Dinosaur::applySpeedVariation(int seed)
{
    // FIXED: Use a better random approach
    std::srand(static_cast<unsigned int>(std::time(0)) * seed);
    float modifier = (std::rand() % 20) + 90; // 90-110% variation
    modifier /= 100;
    m_Speed *= modifier;
}

void Dinosaur::applySizeVariation(int seed)
{
    // FIXED: Use a better random approach
    std::srand(static_cast<unsigned int>(std::time(0)) * seed + 123);
    m_SizeModifier = (std::rand() % 40 + 80) / 100.0f; // 80-120% size variation
}

bool Dinosaur::isReadyToShoot() const
{
    // Dinosaurs don't shoot, so return false
    return false;
}

void Dinosaur::resetShootTimer()
{
    // Dinosaurs don't shoot, so this does nothing

}

sf::Vector2f Dinosaur::getCenter() const
{
    // Return the center position of the dinosaur
    return m_Position;

}