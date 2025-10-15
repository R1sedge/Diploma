#include "PhysicsEngine.h"
#include "ParticleSystem.h"
#include <iostream>


PhysicsEngine::PhysicsEngine(): 
    particleSystem(),
    border()
{

    transformMatrix = glm::mat2x2(Config::BORDER_LENGTH / 2, 0, 0, Config::BORDER_HEIGHT * Config::BORDER_AR / 2);
    inverseTransformMatrix = glm::inverse(transformMatrix);
}

void PhysicsEngine::update(float dt) {

    // ќбновл€ем положение частиц
    particleSystem.applyGravity(dt);

    resolveParticleCollisions(particleSystem.getParticles());

    // ѕровер€ем коллизии с границами дл€ каждой частицы
    checkBorderCollisions(particleSystem.getParticles(), border);

}

void PhysicsEngine::rotate(float angle) {
    border.rotate(angle);
}

std::vector<glm::vec2> PhysicsEngine::getBorderCornersGlobal()
{
    std::vector<glm::vec2> corners = border.getCorners();
    std::vector<glm::vec2> cornersGlobal(corners.size());
    for (int i = 0; i < corners.size(); i++) {
        cornersGlobal[i] = toGlobal(corners[i]);
    }
    return cornersGlobal;
}

std::vector<Particle> PhysicsEngine::getParticlesGlobal()
{
    std::vector<Particle> particles = particleSystem.getParticles();
    std::vector<Particle> particlesGlobal(particles.size());
    for (int i = 0; i < particles.size(); i++) {
        particlesGlobal[i] = particles[i];
        particlesGlobal[i].position = toGlobal(particles[i].position);
    }
    return particlesGlobal;
}

void PhysicsEngine::checkBorderCollisions(std::vector<Particle>& particles, const Border& border) {
    const auto& corners = border.getCorners();
    const auto& normals = border.getNormals();
    float localRadius = border.getLocalRadius();

    for (auto& particle : particles)
    {
        // ѕроверка коллизий с левой и правой границами
        if (glm::dot(particle.position - corners[0] - normals[0] * localRadius, normals[0]) <= 0) {
            particle.position -= glm::dot(particle.position - corners[0] - normals[0] * localRadius, normals[0]) * normals[0];
            particle.onBorder = true;
        }
        else if (glm::dot(particle.position - corners[2] - normals[2] * localRadius, normals[2]) <= 0) {
            particle.position -= glm::dot(particle.position - corners[2] - normals[2] * localRadius, normals[2]) * normals[2];
            particle.onBorder = true;
        }
        else {
            particle.onBorder = false;
        }

        // ѕроверка коллизий с верхней и нижней границами
        if (glm::dot(particle.position - corners[1] - normals[1] * localRadius, normals[1]) <= 0) {
            particle.position -= glm::dot(particle.position - corners[1] - normals[1] * localRadius, normals[1]) * normals[1];
            particle.onBorder = true;
        }
        else if (glm::dot(particle.position - corners[3] - normals[3] * localRadius, normals[3]) <= 0) {
            particle.position -= glm::dot(particle.position - corners[3] - normals[3] * localRadius, normals[3]) * normals[3];
            particle.onBorder = true;
        }
        else {
            particle.onBorder = false;
        }
    }
}

void PhysicsEngine::resolveParticleCollisions(std::vector<Particle>& particles) 
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i + 1; j < particles.size(); j++)
        {
            glm::vec2 distVec = particles[i].position - particles[j].position;
            float dist = glm::length(distVec);
            distVec = glm::normalize(distVec);
            float localRadius = border.getLocalRadius();
            if (dist < localRadius * 2)
            {
                particles[i].position += distVec * (localRadius - dist * 0.5f);
                particles[j].position -= distVec * (localRadius - dist * 0.5f);
            }
        }
    }
    
}

void PhysicsEngine::createParticles(int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.position = glm::vec2((rand() % 1000 - 500) / 500.f, (rand() % 1000 - 500) / 500.f);
        particleSystem.addParticle(p);

    }
}