#pragma once
#include "Border.h"
#include "ParticleSystem.h"

class PhysicsEngine {
public:
    
    ParticleSystem particleSystem;
    Border border;
    glm::vec2 centerPositionGlobal = glm::vec2(Config::WINDOW_WIDTH / 2, Config::WINDOW_HEIGHT / 2);

private:
    glm::mat2x2 transformMatrix;
    glm::mat2x2 inverseTransformMatrix;

public:
    PhysicsEngine();

    glm::vec2 toLocal(glm::vec2& globalPos) {return inverseTransformMatrix * (globalPos - centerPositionGlobal);};
    glm::vec2 toGlobal(glm::vec2& localPos) {return transformMatrix * localPos + centerPositionGlobal;};

    void update(float dt);
    void rotate(float angle);

    std::vector<Particle> getParticlesGlobal();
    std::vector<glm::vec2> getBorderCornersGlobal();

    void createParticles(int count);

private:
    void checkBorderCollisions(std::vector<Particle>& particles, const Border& border);
    void resolveParticleCollisions(std::vector<Particle>& particles);
};