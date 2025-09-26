#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <PhysicsEngine.h>
#include "Config.h"

class Renderer {
private:
    float screenWidth;
    float screenHeight;

public:
    Renderer(float width = Config::WINDOW_WIDTH, float height = Config::WINDOW_HEIGHT);

    void setupProjection() const;
    void clearScreen() const;
    void drawCircle(float cx, float cy, float radius, int res = Config::CIRCLE_RESOLUTION) const;
    void drawParticles(std::vector<Particle>& particlesGlobal);
    void drawBorder(std::vector<glm::vec2> cornersGlobal);
    void drawPhysics(PhysicsEngine& physicsEngine);
    void swapBuffers(GLFWwindow* window) const;
};