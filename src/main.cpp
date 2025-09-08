#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

GLFWwindow* StartGLFW(); // ������� �������� ����
float screenWidth = 800.0f;
float screenHeight = 800.0f;

float centerX = screenWidth / 2.0f;
float centerY = screenHeight / 2.0f;
int res = 50;

// Simulation constats
glm::vec2 gravity = glm::vec2( 5.f, -9.81f ) / 333.f;
float elasticity = 1.f;

float ParticleRadius = 5.f;
int numParticles = 2000;


// ��������� �������
struct Particle
{
    float radius = ParticleRadius;
    glm::vec2 position = { 0.f, 0.f };
    glm::vec2 velocity = { 0.f, 0.f };
};



void DrawCircle(float cx, float cy, float radius)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= res; i++) // <= res ��� �������� �����
    {
        float angle = 2.0f * 3.1415926f * (float)i / res;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}


/// 
/// �������� �������
/// 

class Border
{
    glm::vec2 centerPosition = { screenWidth / 2, screenHeight / 2 };
    float borderThikness = 2.0f;
    float borderLength = 100.0f;
    float borderHeight = 100.0f;


    float rotationAngle = 0.f;
    std::vector<Particle> particles;

    

public:
    Border(float borderLength, float borderHeight, float borderThikness):
        borderHeight(borderHeight),
        borderLength(borderLength),
        borderThikness(borderThikness)
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
            DrawCircle(particle.position.x, particle.position.y, ParticleRadius);
        }
    }

    void ApplyGravity()
    {
        for (auto& particle : particles)
        {
            particle.velocity += gravity;
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
        int particlesPerRow = (int)sqrt(n);
        int particlesPerCol = (n - 1) / particlesPerRow + 1;
        float spacing = ParticleRadius * 2 + 5.f;

        for (int i = 0; i < n; i++)
        {
            Particle particle;
            particle.position = this->centerPosition;
            particle.position.x += (i % particlesPerRow - particlesPerRow / 2.f + 0.5f) * spacing;
            particle.position.y += (i / particlesPerRow - particlesPerCol / 2.f + 0.5f) * spacing;
            particles.push_back(particle);
        }
    }
};



void SetupProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // ������������� ��������������� ��������
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



Border border(400.0f, 400.0f, 2.0f);

int main()
{
    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // ����������� fps � ������� ������



    // ��������� ��������
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

    // ������ ����
    GLFWwindow* window = glfwCreateWindow((int)screenWidth, (int)screenHeight, "Fluid_simulation", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // �������� ���������� �������� � ������������� ���� �� ������
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