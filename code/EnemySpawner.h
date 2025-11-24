#pragma once
#include "Enemy.h"
#include "Dinosaur.h"
#include "Cowboy.h"
#include "Android.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

template<typename EnemyType>

// Spawn enemies on map
void EnemySpawner(vector<unique_ptr<Enemy>>& enemies, int count, IntRect arena)
{
    enemies.clear();

    for (int i = 0; i < count; ++i)
    {
        float x, y;
        int maxY = arena.height - 20;
        int minY = arena.top + 20;
        int maxX = arena.width - 20;
        int minX = arena.left + 20;

        srand(static_cast<int>(time(0)) * i);
        int side = rand() % 4;

        switch (side)
        {
        case 0: x = minX; y = (rand() % maxY) + minY; break;
        case 1: x = maxX; y = (rand() % maxY) + minY; break;
        case 2: x = (rand() % maxX) + minX; y = minY; break;
        case 3: x = (rand() % maxX) + minX; y = maxY; break;
        }

        auto e = make_unique<EnemyType>();
        e->spawn(x, y, i);
        enemies.push_back(move(e));
    }
}