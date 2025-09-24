#include "InputHandler.h"
#include <cmath>
#include <algorithm>

// »нициализаци€ статических переменных
bool InputHandler::leftMousePressed = false;
float InputHandler::lastAngle = 0.0f;
float InputHandler::deltaAngle = 0.0f;
glm::vec2 InputHandler::screenCenter = { 0.0f, 0.0f };

void InputHandler::initialize(float screenWidth, float screenHeight) {
    screenCenter.x = screenWidth / 2.0f;
    screenCenter.y = screenHeight / 2.0f;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftMousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftMousePressed = false;
    }
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (leftMousePressed) {
        // ¬ычисл€ем вектор от центра к позиции мыши
        float dx = static_cast<float>(xpos) - screenCenter.x;
        float dy = static_cast<float>(ypos) - screenCenter.y;

        // ¬ычисл€ем угол в радианах
        float currentAngle = std::atan2(dy, dx);
        deltaAngle = currentAngle - lastAngle;

        // ќграничиваем изменение угла (как в исходном коде)
        deltaAngle = deltaAngle > 0 ?
            std::min(deltaAngle, 0.1f) :
            std::max(deltaAngle, -0.1f);

        lastAngle = currentAngle;
    }
}