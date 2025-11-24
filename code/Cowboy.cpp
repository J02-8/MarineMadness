#include "Cowboy.h"
#include "TextureHolder.h"
#include <cmath> 
#include <cstdlib>
#include <ctime>

#include <SFML/Audio.hpp>

#include <iostream>
using namespace std;

Cowboy::Cowboy()
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

void Cowboy::spawn(float startX, float startY, int seed)
{
    // Single cowboy type 
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/cowboy.png"));
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

bool Cowboy::hit()
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

void Cowboy::update(float elapsedTime, sf::Vector2f playerLocation) {
    if (!m_Alive) return;

    //update the shoot timer
    m_TimeSinceLastShot += elapsedTime;

    // Update path timer
    m_PathUpdateTimer += elapsedTime;

    // Recalculate path periodically
    if (m_PathUpdateTimer >= m_PathUpdateInterval && m_Pathfinding != nullptr) {
        updatePath(playerLocation);
        m_PathUpdateTimer = 0.0f;

        cout << "Cowboy at (" << m_Position.x << ", " << m_Position.y << ") - Path size: " << m_Path.size() << endl;
    }

    // Calculate direction to player
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

     // Follow the path if it exists
    if (!m_Path.empty() && m_CurrentPathIndex < m_Path.size()) {
        // Use every 3rd waypoint for smoother movement (or just the last one if close to end)
        int targetIndex = m_CurrentPathIndex;
        if (m_Path.size() - m_CurrentPathIndex > 3) {
            targetIndex = m_CurrentPathIndex + 2; // Skip ahead 2 waypoints
        } else {
            targetIndex = m_Path.size() - 1; // Just go to final destination
        }
        
        Vector2f targetWaypoint = m_Path[targetIndex];
        
        // Calculate direction to target waypoint
        float dx = targetWaypoint.x - m_Position.x;
        float dy = targetWaypoint.y - m_Position.y;
        float distance = sqrt(dx * dx + dy * dy);

        // Check if we've reached the current waypoint
        if (distance < 25.0f) {
            m_CurrentPathIndex = targetIndex + 1;
            cout << "Reached waypoint! Moving to index: " << m_CurrentPathIndex << endl;

            // If we've reached the end of the path, try to update it
            if (m_CurrentPathIndex >= m_Path.size()) {
                updatePath(playerLocation);
            }
        }
        else {
            // Move towards the waypoint
            dx /= distance;
            dy /= distance;

            m_Position.x += dx * m_Speed * elapsedTime;
            m_Position.y += dy * m_Speed * elapsedTime;

        // Face the sprite in the correct direction
        float angle = (atan2(dy, dx) * 180) / 3.141f;
        m_Sprite.setRotation(angle);
    }
    
        
    }
    
    /*
    else {

        cout << "No path found! Using direct movement." << endl;
        // Fallback to direct movement if no path exists
        float playerX = playerLocation.x;
        float playerY = playerLocation.y;
        
        // Update the cowboy position
        if (playerX > m_Position.x) {
            m_Position.x = m_Position.x + m_Speed * elapsedTime;
        }
        if (playerY > m_Position.y) {
            m_Position.y = m_Position.y + m_Speed * elapsedTime;
        }
        if (playerX < m_Position.x) {
            m_Position.x = m_Position.x - m_Speed * elapsedTime;
        }
        if (playerY < m_Position.y) {
            m_Position.y = m_Position.y - m_Speed * elapsedTime;
        }

        // Face the sprite in the correct direction
        float angle = (atan2(playerY - m_Position.y,
            playerX - m_Position.x)
            * 180) / 3.141f;

        m_Sprite.setRotation(angle);

    }
       */

       

        // Move the sprite
        m_Sprite.setPosition(m_Position);
    
}
void Cowboy::applySpeedVariation(int seed)
{
    // FIXED: Use a better random approach
    std::srand(static_cast<unsigned int>(std::time(0)) * seed);
    float modifier = (std::rand() % 20) + 90; // 90-110% variation
    modifier /= 100;
    m_Speed *= modifier;
}

//  method to get shooting state
bool Cowboy::isReadyToShoot() const {
    return m_CanShoot && m_TimeSinceLastShot >= m_ShootCooldown;
}

//  method to reset shooting timer after shooting
void Cowboy::resetShootTimer() {
    m_TimeSinceLastShot = 0.0f;
    m_ShootCooldown = (std::rand() % 2000 + 1000) / 1000.0f; // 1-3 seconds
}

//method to get cowboy position
sf::Vector2f Cowboy::getCenter() const {
    return m_Position;
}



