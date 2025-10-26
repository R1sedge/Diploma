#include "InputHandler.h"
#include <cmath>
#include <algorithm>
#define M_PI 3.14159265358979323846

// Инициализация статических переменных
bool InputHandler::leftMousePressed = false;
bool InputHandler::firstMoveAfterPress = false;
float InputHandler::lastAngle = 0.0f;
float InputHandler::deltaAngle = 0.0f;
glm::vec2 InputHandler::screenCenter = { 0.0f, 0.0f };

void InputHandler::initialize(float screenWidth, float screenHeight) {
    screenCenter.x = screenWidth / 2.0f;
    screenCenter.y = screenHeight / 2.0f;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMousePressed = true;
            firstMoveAfterPress = true; // Следующее движение будет первым после нажатия
        }
        else if (action == GLFW_RELEASE) {
            leftMousePressed = false;
        }
    }
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (leftMousePressed) {
        // Вычисляем вектор от центра к позиции мыши
        float dx = static_cast<float>(xpos) - screenCenter.x;
        float dy = static_cast<float>(ypos) - screenCenter.y;

        // Вычисляем текущий угол
        float currentAngle = std::atan2(dy, dx);

        if (firstMoveAfterPress) {
            // При первом движении после нажатия не вычисляем дельту
            lastAngle = currentAngle;
            deltaAngle = 0.0f;
            firstMoveAfterPress = false;
        }
        else {
            // Вычисляем разницу углов
            deltaAngle = currentAngle - lastAngle;

            // Корректируем скачок через границу -π/+π
            if (deltaAngle > M_PI) {
                deltaAngle -= 2 * M_PI;
            }
            else if (deltaAngle < -M_PI) {
                deltaAngle += 2 * M_PI;
            }

            lastAngle = currentAngle;
        }
    }
}