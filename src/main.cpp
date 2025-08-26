#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

GLFWwindow* StartGLFW(); // Функция создания окна
float screenHeight = 600.0f;
float screenWidth = 800.0f;

float centerX = screenWidth / 2.0f;
float centerY = screenHeight / 2.0f;
int res = 20;

// Simulation constats
float gravity = 9.81f;




// Структура частицы
struct Particle 
{
    float radius = 20.0f;
    std::vector<float> position = { 0.f, 0.f };
    std::vector<float> velocity = { 0.f, 0.f };
};

Particle particle1{ 20.f, {400.f, 580.f}, {0.f, 0.f} };

void ApplyGravity(Particle& particle)
{
    particle.velocity[1] -= gravity / 100;
    particle.position[0] += particle.velocity[0];
    particle.position[1] += particle.velocity[1];
}

void CheckBorderCollision(Particle& particle)
{
    if (particle.position[1] < particle.radius) 
    { 
        particle.position[1] = 2 * particle.radius - particle.position[1];
        particle.velocity[1] *= -0.95; 
    }
    else if (particle.position[1] > screenHeight - particle.radius)
    {
        particle.position[1] = 2 * (screenHeight - particle.radius) - particle.position[1];
        particle.velocity[1] *= -0.95;
    }

    if (particle.position[0] < particle.radius)
    { 
        particle.position[0] = 2 * particle.radius - particle.position[0];
        particle.velocity[0] *=  -0.95;
    }
    else if (particle.position[0] > screenWidth - particle.radius)
    {
        particle.position[0] = 2 * (screenWidth - particle.radius) - particle.position[0];
        particle.velocity[0] *= -0.95;
    }
}

void DrawCircle(float cx, float cy, float radius, int res)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= res; i++) // <= res для закрытия круга
    {
        float angle = 2.0f * 3.1415926f * (float)i / res;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}


void DrawParticle(Particle& particle)
{
    DrawCircle(particle.position[0], particle.position[1], particle.radius, res = 20);
}


void SetupProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // Устанавливаем ортографическую проекцию
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}




int main()
{
    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Ограничение fps в частоту экрана



    // Настройка проекции
    SetupProjection();

 
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        // Рисуем круг
        glColor3f(0.0f, 0.0f, 0.0f);
        DrawParticle(particle1);
        
        ApplyGravity(particle1);
        CheckBorderCollision(particle1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




GLFWwindow* StartGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // Создаём окно
    GLFWwindow* window = glfwCreateWindow((int)screenWidth, (int)screenHeight, "Fluid_simulation", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Получаем разрешение монитора и позиционируем окно по центру
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor)
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        if (videoMode)
        {
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

            int windowPosX = monitorX + (videoMode->width - (int)screenWidth) / 2;
            int windowPosY = monitorY + (videoMode->height - (int)screenHeight) / 2;

            glfwSetWindowPos(window, windowPosX, windowPosY);
        }
    }

    return window;
}