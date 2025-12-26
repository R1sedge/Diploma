#include "PhysicsEngine.h"
#include "ParticleSystem.h"
#include <iostream>


PhysicsEngine::PhysicsEngine(): 
    particleSystem(),
    border()
{
    // »нициализаци€ матриц преобразовани€
    transformMatrix = glm::mat2x2(Config::BORDER_LENGTH / 2, 0, 0, Config::BORDER_HEIGHT * Config::BORDER_AR / 2); 
    inverseTransformMatrix = glm::inverse(transformMatrix);
}

void PhysicsEngine::update(float dt) {

    float sub_dt = dt / Config::sub_steps;
    for (int i = 0; i < Config::sub_steps; i++) 
    {
        particleSystem.applyGravity(sub_dt);
        applyConstraint(particleSystem.getParticles(), border);
        resolveCollisions(particleSystem.getParticles());
        particleSystem.update(sub_dt);
    }
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
        particlesGlobal[i].position_current = toGlobal(particles[i].position_current);
    }
    return particlesGlobal;
}

void PhysicsEngine::applyConstraint(std::vector<Particle>& particles, const Border& border) {
    const auto& corners = border.getCorners();
    const auto& normals = border.getNormals();

    for (auto& particle : particles)
    {
        float localRadius = border.getLocalRadius(particle);

        // ѕроверка коллизий с левой и правой границами
        if (glm::dot(particle.position_current - corners[0] - normals[0] * localRadius, normals[0]) <= 0) {
            particle.position_current -= glm::dot(particle.position_current - corners[0] - normals[0] * localRadius, normals[0]) * normals[0];
        }
        else if (glm::dot(particle.position_current - corners[2] - normals[2] * localRadius, normals[2]) <= 0) {
            particle.position_current -= glm::dot(particle.position_current - corners[2] - normals[2] * localRadius, normals[2]) * normals[2];
        }

        // ѕроверка коллизий с верхней и нижней границами
        if (glm::dot(particle.position_current - corners[1] - normals[1] * localRadius, normals[1]) <= 0) {
            particle.position_current -= glm::dot(particle.position_current - corners[1] - normals[1] * localRadius, normals[1]) * normals[1];
        }
        else if (glm::dot(particle.position_current - corners[3] - normals[3] * localRadius, normals[3]) <= 0) {
            particle.position_current -= glm::dot(particle.position_current - corners[3] - normals[3] * localRadius, normals[3]) * normals[3];
        }
    }
}

void PhysicsEngine::resolveCollisions(std::vector<Particle>& particles) 
{
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& particle1 = particles[i];
        for (int j = i + 1; j < particles.size(); j++)
        {
            Particle& particle2 = particles[j];
            glm::vec2 collision_axis = particle2.position_current - particle1.position_current;
            float dist = glm::length(collision_axis);
            float localRadius1 = border.getLocalRadius(particle1);
            float localRadius2 = border.getLocalRadius(particle2);
            float radiusSum = localRadius1 + localRadius2;
            if (dist < radiusSum)
            {
                glm::vec2 n = collision_axis / dist;
                float delta = radiusSum - dist;
                particle1.position_current -= 0.5f * delta * n;
                particle2.position_current += 0.5f * delta * n;

            }
        }
    }
    
}

void PhysicsEngine::createParticles(int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.position_current = glm::vec2((rand() % 1000 - 500) / 500.f, (rand() % 1000 - 500) / 500.f);
        p.position_old = p.position_current;
        p.radius = rand() % 10 + 5;
        particleSystem.addParticle(p);
    }
}