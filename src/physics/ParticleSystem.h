#pragma once
#include <vector>
#include "Particle.h"
#include "Border.h"

class ParticleSystem {
private:
    std::vector<Particle> particles;

public:
    // Пустой генератор
    ParticleSystem();

    void addParticle(Particle& particle);

    void clearParticles();

    void applyGravity(float dt);

    void update(float dt);

    // Геттеры
    std::vector<Particle>& getParticles() { return particles; }
    size_t getParticleCount() const { return particles.size(); }
};