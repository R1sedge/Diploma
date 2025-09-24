#include "Border.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Конструктор
Border::Border() : 
    borderThickness(Config::BORDER_THICKNESS),
    borderLength(Config::BORDER_LENGTH),
    borderHeight(Config::BORDER_HEIGHT)
{
    // Инициализация углов (по часовой стрелке)
    corners = {
        { -1, -1 / Config::BORDER_AR },
        { -1, 1 / Config::BORDER_AR },
        { 1, 1 / Config::BORDER_AR },
        { 1, -1 / Config::BORDER_AR }
    };

    // Нормали: left, up, right, bottom
    normals = {
        { 1, 0 },   // левая грань
        { 0, -1 },  // верхняя грань  
        { -1, 0 },  // правая грань
        { 0, 1 }    // нижняя грань
    };
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