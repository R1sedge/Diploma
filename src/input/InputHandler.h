#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler {
private:
    static bool leftMousePressed;
    static float lastAngle;
    static float deltaAngle;
    static glm::vec2 screenCenter;
    static bool firstMoveAfterPress;

public:
    static void initialize(float screenWidth, float screenHeight);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

    static float getDeltaAngle() { return deltaAngle; }
    static void resetDeltaAngle() { deltaAngle = 0.0f; }
    static bool isLeftMousePressed() { return leftMousePressed; }
};