#include "MainMenu.h"

// Constructor
MainMenu::MainMenu(float width, float height)
    : Menu(width, height, { "Play", "Load", "Scoreboard", "Options", "Exit" })
{
    font.loadFromFile("fonts/ByteBounce.ttf");

    // Create particles
    const int PARTICLE_COUNT = 1000;
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        Vector2f position(rand() % (int)width, rand() % (int)height);
        Vector2f velocity((std::rand() % 2001) / 100.f - 10.f, (std::rand() % 2001) / 100.f - 10.f);
        auto flyweight = ParticleFactory::getParticleFlyweight(2.f); // small white particles
        particles.emplace_back(flyweight, position, velocity);
    }
}

void MainMenu::updateParticles(float dt)
{
    for (size_t i = 0; i < particles.size(); ++i)
    {
        particles[i].update(dt);

        for (size_t j = i + 1; j < particles.size(); ++j)
        {
            particles[i].checkCollision(particles[j]);
        }
    }
}

void MainMenu::draw(RenderWindow& window)
{
    // Draw particles behind menu text
    for (auto& p : particles)
    {
        p.draw(window);
    }

    // Draw the menu text from the base class
    Menu::draw(window);
}