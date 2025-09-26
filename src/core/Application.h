#pragma once
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "ParticleSystem.h"
#include "PhysicsEngine.h"
#include "Border.h"
#include "InputHandler.h"
#include "Config.h"
#include "serializationManager.h"

class Application {
private:
    GLFWwindow* window;
    Renderer renderer;
    PhysicsEngine physicsEngine;
    bool running;
    SerializationManager serializer;
    int currentFrame;

public:
    Application();
    ~Application();

    bool initialize();
    void run();
    void shutdown();

private:
    bool createWindow();
    void setupCallbacks();
    void mainLoop();
    void handleInput();
    void update(float dt);
    void render();
};