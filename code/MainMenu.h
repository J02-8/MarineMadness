#pragma once
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Particle.h"
#include "ParticleFactory.h"

using namespace std;
using namespace sf;

class MainMenu : public Menu
{
public:
    MainMenu(float width, float height);

    void updateParticles(float dt);
    void draw(RenderWindow& window) override;

private:
    vector<Particle> particles;
};