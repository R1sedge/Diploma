#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

Renderer::Renderer(float width, float height)
    : screenWidth(width), screenHeight(height) {
}

void Renderer::setupProjection() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::clearScreen() const {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Белый фон
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawCircle(float cx, float cy, float radius, int res) const {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= res; i++) {
        float angle = 2.0f * M_PI * (float)i / res;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

void Renderer::drawParticles(std::vector<Particle>& particlesGlobal)  {
    glColor3f(0.25f, 0.66f, 1.0f); // Голубой цвет для частиц

    for (const auto& particle : particlesGlobal) {
        //std::cout << particle.position.x << " " << particle.position.y << std::endl;
        drawCircle(particle.position_current.x, particle.position_current.y, particle.radius, Config::CIRCLE_RESOLUTION);
    }
}

void Renderer::drawBorder(std::vector<glm::vec2> cornersGlobal)
{
    glColor3f(0.0f, 0.0f, 0.0f); // Чёрный цвет для границы
    glLineWidth(Config::BORDER_THICKNESS);
    glBegin(GL_LINE_LOOP);
    for (auto& corner : cornersGlobal) {
        //std:: cout << corner.x << " " << corner.y << std::endl;
        glVertex2f(corner.x, corner.y);
    }
    glEnd();
}

void Renderer::drawPhysics(PhysicsEngine& physicsEngine)
{
    drawParticles(physicsEngine.getParticlesGlobal());
    drawBorder(physicsEngine.getBorderCornersGlobal());
}

void Renderer::swapBuffers(GLFWwindow* window) const {
    glfwSwapBuffers(window);
}