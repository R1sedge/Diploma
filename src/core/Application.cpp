#include "Application.h"
#include <iostream>

Application::Application()
    : window(nullptr),
    renderer(),
    physicsEngine(),
    running(false) {
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    if (!createWindow()) {
        return false;
    }

    InputHandler::initialize(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    setupCallbacks();
    renderer.setupProjection();

    // Добавляем начальную частицу как в исходном коде
    Particle initialParticle(glm::vec2(0, 0));
    physicsEngine.particleSystem.addParticle(initialParticle);

    running = true;
    return true;
}

void Application::run() {
    mainLoop();
}

void Application::shutdown() {
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

    window = glfwCreateWindow((int)Config::SCREEN_WIDTH, (int)Config::SCREEN_HEIGHT, "Fluid_simulation", NULL, NULL);
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

            int windowPosX = monitorX + (videoMode->width - (int)Config::SCREEN_WIDTH) / 2;
            int windowPosY = monitorY + (videoMode->height - (int)Config::SCREEN_HEIGHT) / 2;

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

void Application::handleInput() {
    // Вся обработка ввода теперь в InputHandler
    // Здесь может быть дополнительная логика если нужно
}

void Application::update(float dt) {
    // Обновление физики через PhysicsEngine
    physicsEngine.update(dt);

    // Поворот границы если есть изменение угла
    float currentDeltaAngle = InputHandler::getDeltaAngle();
    if (currentDeltaAngle != 0.0f) {
        physicsEngine.rotate(currentDeltaAngle);
        InputHandler::resetDeltaAngle();
    }
}

void Application::render() {
    renderer.clearScreen();
    renderer.drawPhysics(physicsEngine);
    renderer.swapBuffers(window);
    glfwPollEvents();
}