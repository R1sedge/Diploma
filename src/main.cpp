#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

GLFWwindow* StartGLFW(); // Функция создания окна
float screenWidth = 800.0f;
float screenHeight = 800.0f;

float centerX = screenWidth / 2.0f;
float centerY = screenHeight / 2.0f;
int res = 20;
float ParticleRadius = 30.f;

// Simulation constats
glm::vec2 gravity = glm::vec2( 1.f, 9.81f ) / 100.f;
float elasticity = 1.0f;


// Структура частицы
struct Particle
{
    float radius = ParticleRadius;
    glm::vec2 position = { 0.f, 0.f };
    glm::vec2 velocity = { 0.f, 0.f };
};


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


/// 
/// ОПИСАНИЕ ГРАНИЦЫ
/// 

class Border
{
    glm::vec2 centerPosition = { screenWidth / 2, screenHeight / 2 };
    float borderThikness = 2.0f;
    float borderLength = 100.0f;
    float borderHeight = 100.0f;
    int numParticles = 1;


    float rotationAngle = 0.f;
    std::vector<Particle> particles;

    

public:
    Border(float borderLength, float borderHeight, float borderThikness, int numParticles):
        borderHeight(borderHeight),
        borderLength(borderLength),
        borderThikness(borderThikness),
        numParticles(numParticles)
        {
            CreateParticles(numParticles);
        }

    void DrawBorder()
    {
        glLineWidth(borderThikness);
        glBegin(GL_LINE_LOOP);
        glVertex2f(centerPosition.x - borderLength / 2, centerPosition.y - borderHeight / 2);
        glVertex2f(centerPosition.x + borderLength / 2, centerPosition.y - borderHeight / 2);
        glVertex2f(centerPosition.x + borderLength / 2, centerPosition.y + borderHeight / 2);
        glVertex2f(centerPosition.x - borderLength / 2, centerPosition.y + borderHeight / 2);
        glEnd();
    }

    void DrawParticles()
    {
        for (auto& particle : particles)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.radius, res = 20);
        }
    }

    void ApplyGravity()
    {
        for (auto& particle : particles)
        {
            particle.velocity -= gravity;
            particle.position += particle.velocity;
        }
    }

    void CheckBorderCollisions()
    {
        float minX = centerPosition.x - borderLength / 2 + borderThikness + ParticleRadius;
        float maxX = centerPosition.x + borderLength / 2 - borderThikness - ParticleRadius;
        float minY = centerPosition.y - borderHeight / 2 + borderThikness + ParticleRadius;
        float maxY = centerPosition.y + borderHeight / 2 - borderThikness - ParticleRadius;

        for (auto& particle : particles)
        {
            if (particle.position.y < minY)
            {
                particle.position.y = 2 * minY - particle.position.y;
                particle.velocity.y *= -1 * elasticity;
            }
            else if (particle.position.y > maxY)
            {
                particle.position.y = 2 * maxY - particle.position.y;
                particle.velocity.y *= -1 * elasticity;
            }

            if (particle.position.x < minX)
            {
                particle.position.x = 2 * minX - particle.position.x;
                particle.velocity.x *= -1 * elasticity;
            }
            else if (particle.position.x > maxX)
            {
                particle.position.x = 2 * maxX - particle.position.x;
                particle.velocity.x *= -1 * elasticity;
            }
        }

    }

    void Update()
    {
        ApplyGravity();
        CheckBorderCollisions();
    }

    void Draw()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        DrawBorder();
        DrawParticles();
    }

    void CreateParticles(int n = 1)
    {
        for (int i = 0; i < n; ++i)
        {
            Particle particle;
            particle.position = this->centerPosition;
            particles.push_back(particle);
        }
    }
};



void SetupProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // Устанавливаем ортографическую проекцию
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



Border border(400.0f, 400.0f, 2.0f, 1);

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

        border.Update();
        border.Draw();


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