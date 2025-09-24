#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Particle.h"
#include "Config.h"

class Border {
private:
    glm::vec2 centerPosition = { Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2 };
    float borderThickness;
    float borderLength;
    float borderHeight;

    std::vector<glm::vec2> corners;
    std::vector<glm::vec2> normals;
    
    float rotationAngle = 0.0f;

public:
    Border();

    void rotate(float deltaAngle);

    // Геттеры для коллизий
    const std::vector<glm::vec2>& getCorners() const { return corners; }
    const std::vector<glm::vec2>& getNormals() const { return normals; }
    float getLocalRadius() const { return 2 * Config::PARTICLE_RADIUS / borderLength; }
    glm::vec2 getCenterPosition() const { return centerPosition; }

    // Геттеры для отрисовки
    float getThickness() const { return borderThickness; }

};