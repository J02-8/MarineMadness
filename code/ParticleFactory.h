#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ParticleFlyweight.h"
#include <iostream>

using namespace std;
using namespace sf;

class ParticleFactory 
{
public:
    static shared_ptr<ParticleFlyweight> getParticleFlyweight(float radius);

private:
    static unordered_map<string, shared_ptr<ParticleFlyweight>> particles;
};