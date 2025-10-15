#include "Border.h"
#include <iostream>

// Конструктор
Border::Border() : 
    borderThickness(Config::BORDER_THICKNESS),
    borderLength(Config::BORDER_LENGTH),
    borderHeight(Config::BORDER_HEIGHT)
{
    // Инициализация углов границы
    corners = {
        { -1, -1 / Config::BORDER_AR },
        { -1, 1 / Config::BORDER_AR },
        { 1, 1 / Config::BORDER_AR },
        { 1, -1 / Config::BORDER_AR }
    };

    // Нормали сторон
    for (int i = 0; i < corners.size() - 1; i++) 
    {
        glm::vec2 n = corners[i + 1] - corners[i];
        n = glm::normalize(glm::vec2(-n.y, n.x));
        if (glm::dot(n, (corners[i] - glm::vec2(0, 0))) > 0) 
        {
            n = -n;
        }
        normals.push_back(n);
    }

    glm::vec2 n = corners[0] - corners[corners.size() - 1];
    n = glm::normalize(glm::vec2(-n.y, n.x));
    if (glm::dot(n, (corners[corners.size() - 1] - glm::vec2(0, 0))) > 0)
    {
        n = -n;
    }
    normals.push_back(n);
}

// Поворот границы
void Border::rotate(float deltaAngle) {
    
    glm::mat2x2 rotationMatrix = glm::mat2x2(
        cos(deltaAngle), -sin(deltaAngle),
        sin(deltaAngle), cos(deltaAngle)
    );

    // Поворачиваем углы
    for (auto& corner : corners) {
        corner = rotationMatrix * corner;
    }

    // Поворачиваем нормали
    for (auto& normal : normals) {
        normal = rotationMatrix * normal;
    }
}