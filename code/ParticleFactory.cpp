#include "ParticleFactory.h"
#include <iostream>

using namespace std;
using namespace sf;

unordered_map<string, shared_ptr<ParticleFlyweight>> ParticleFactory::particles;

shared_ptr<ParticleFlyweight> ParticleFactory::getParticleFlyweight(float radius) 
{
    string key = std::to_string(radius);
    auto it = particles.find(key);
    if (it != particles.end()) 
    {
        return it->second;
    }

    shared_ptr<ParticleFlyweight> newParticle = make_shared<ParticleFlyweight>(radius);
    particles[key] = newParticle;
    return newParticle;
}