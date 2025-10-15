#include "ParticleSystem.h"
#include <iostream>

// Конструктор
ParticleSystem::ParticleSystem() {}

// Добавление частицы
void ParticleSystem::addParticle(Particle& particle) {
    particles.push_back(particle);
}

// Очистка всех частиц
void ParticleSystem::clearParticles() {
    particles.clear();
}

void ParticleSystem::applyGravity(float dt) {
        for (auto& particle : particles) {
            particle.accelerate(Config::GRAVITY);
        }
}

void ParticleSystem::update(float dt)
{
    for (auto& particle : particles)
    {
        particle.update(dt);
    }
}

