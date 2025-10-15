#pragma once
#include <glm/glm.hpp>
#include "Config.h"

struct Particle {
    float radius = Config::PARTICLE_RADIUS;
    glm::vec2 position = { 0.f, 0.f };
    glm::vec2 velocity = { 0.f, 0.f };
    glm::vec2 acceleration = { 0.f, 0.f };

    Particle() = default;
    Particle(const glm::vec2& pos) : position(pos) {}
    Particle(float r, const glm::vec2& pos) : radius(r), position(pos) {}

    void update(float dt);
    void accelerate(glm::vec2 acc);
};