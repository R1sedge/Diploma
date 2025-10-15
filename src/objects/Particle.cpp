#include "Particle.h"

void Particle::update(float dt)
{
	position = position + velocity * dt;
	velocity = velocity + acceleration * dt;
	acceleration = {0., 0.};
}

void Particle::accelerate(glm::vec2 acc)
{
	acceleration += acc;
}