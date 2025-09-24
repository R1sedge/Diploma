#pragma once
#include "Border.h"
#include "ParticleSystem.h"

class PhysicsEngine {
public:
    
    ParticleSystem particleSystem;
    Border border;

private:
    glm::mat2x2 transformMatrix;
    glm::mat2x2 inverseTransformMatrix;

public:
    PhysicsEngine();

    glm::vec2 toLocal(glm::vec2& globalPos) {return inverseTransformMatrix * (globalPos - border.getCenterPosition());};
    glm::vec2 toGlobal(glm::vec2& localPos) {return transformMatrix * localPos + border.getCenterPosition();};

    void update(float dt);
    void rotate(float angle);

    std::vector<Particle> getParticlesGlobal();
    std::vector<glm::vec2> getBorderCornersGlobal();

private:
    void checkBorderCollisions(std::vector<Particle>& particles, const Border& border);
    void resolveParticleCollisions(std::vector<Particle>& particles);
};