#include "Particle.h"

void Particle::update(float dt)
{
	glm::vec2 velocity = position_current - position_old;
	position_old = position_current;
	position_current = position_current + velocity + acceleration * dt * dt;
	acceleration = {0., 0.};
}

void Particle::accelerate(glm::vec2 acc)
{
	acceleration += acc;
}