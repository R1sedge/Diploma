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
            //std::cout << particle.position.x << " " << particle.position.y << std::endl;
            particle.position += Config::GRAVITY * dt;
        }
}

