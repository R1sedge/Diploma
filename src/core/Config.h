#pragma once
#include <glm/glm.hpp>
#define M_PI 3.14159265358979323846

struct Config {
	// Окно
	static constexpr float WINDOW_WIDTH = 800.0f;
	static constexpr float WINDOW_HEIGHT = 800.0f;

	// Физика
	static constexpr glm::vec2 GRAVITY = glm::vec2(0.0f, -9.81f) / 20.0f;
	static constexpr float PARTICLE_RADIUS = 10.0f;
	static constexpr float ELASTICITY = 0.0f;

	// Граница
	static constexpr float BORDER_LENGTH = 300.0f;
	static constexpr float BORDER_HEIGHT = 400.0f;
	static constexpr float BORDER_AR = BORDER_LENGTH / BORDER_HEIGHT;
	static constexpr float BORDER_THICKNESS = 2.0f;
	static constexpr int PARTICLES_COUNT = 2;

	// Рендеринг
	static constexpr int CIRCLE_RESOLUTION = 20;
};