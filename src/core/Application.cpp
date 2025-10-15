#include "Application.h"
#include <iostream>

Application::Application()
    : window(nullptr),
    renderer(),
    physicsEngine(),
    serializer(),
    currentFrame(0),
    running(false) {
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    if (!createWindow()) {
        return false;
    }

    InputHandler::initialize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    setupCallbacks();
    renderer.setupProjection();

    physicsEngine.createParticles(Config::PARTICLES_COUNT);

    running = true;

    // Начинаем запись
    //serializer.startRecording("../../src/simulation_data.txt", physicsEngine.particleSystem.getParticleCount());

    return true;
}

void Application::run() {
    mainLoop();
}

void Application::shutdown() 
{
    if (serializer.isRecordingActive()) {
        serializer.stopRecording();
    }

    if (window) {
        glfwTerminate();
        window = nullptr;
    }
}

bool Application::createWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow((int)Config::WINDOW_WIDTH, (int)Config::WINDOW_HEIGHT, "Fluid_simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Центрирование окна 
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor) {
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        if (videoMode) {
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

            int windowPosX = monitorX + (videoMode->width - (int)Config::WINDOW_WIDTH) / 2;
            int windowPosY = monitorY + (videoMode->height - (int)Config::WINDOW_HEIGHT) / 2;

            glfwSetWindowPos(window, windowPosX, windowPosY);
        }
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return true;
}

void Application::setupCallbacks() {
    glfwSetMouseButtonCallback(window, InputHandler::mouseButtonCallback);
    glfwSetCursorPosCallback(window, InputHandler::cursorPositionCallback);
}

void Application::mainLoop() {
    while (running && !glfwWindowShouldClose(window)) {
        handleInput();
        update(1.0f / 30.0f); // 30 FPS 
        render();
    }
}

void Application::handleInput() 
{
}

void Application::update(float dt)  // Базовое dt = 1.0f / 30.0f
{
    // Обновление физики через PhysicsEngine
    physicsEngine.update(dt);


    // Поворот границы если есть изменение угла
    float currentDeltaAngle = InputHandler::getDeltaAngle();
    if (currentDeltaAngle != 0.0f) {
        physicsEngine.rotate(currentDeltaAngle);
        InputHandler::resetDeltaAngle();
    }


    // Записываем кадр
    serializer.recordFrame(currentFrame, physicsEngine);
    currentFrame++;
}

void Application::render() {
    renderer.clearScreen();
    renderer.drawPhysics(physicsEngine);
    renderer.swapBuffers(window);
    glfwPollEvents();
}