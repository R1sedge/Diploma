#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

GLFWwindow* StartGLFW();
float screenWidth = 800.0f;
float screenHeight = 800.0f;

float centerX = screenWidth / 2.0f;
float centerY = screenHeight / 2.0f;
int res = 50;

// Simulation constats
glm::vec2 gravity = glm::vec2( 0.f, -9.81f ) / 500.f;
float elasticity = 0.f;

float ParticleRadius = 10.f;
int numParticles = 5;

//
// Particle definition
//
struct Particle
{
    float radius = ParticleRadius;
    float mass = 1.f;
    glm::vec2 position = { 0.f, 0.f };
    glm::vec2 velocity = { 0.f, 0.f };
    glm::vec2 acceleration = { 0.f, 0.f };
    glm::vec2 force = { 0.f, 0.f };
    
    void ApplyForce()
    {
        this->acceleration = this->force / this->mass;
        this->velocity += acceleration;
        this->position += velocity;
    }
};



void DrawCircle(float cx, float cy, float radius)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= res; i++)
    {
        float angle = 2.0f * 3.1415926f * (float)i / res;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}


/// 
/// Border
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

    // Vizualization
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

    void Draw()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        DrawBorder();
        DrawParticles();
    }

    // Physics
    void ApplyForces()
    {
        for (auto& particle : particles)
        {
           particle.ApplyForce();
        }
    }

    void ApplyGravity()
    {
        for (auto& particle : particles)
        {
            particle.force += gravity * particle.mass;
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
                particle.force += glm::vec2(0.f, particle.force.y * -1);
                particle.velocity.y = 0.f;
            }
            else if (particle.position.y > maxY)
            {
                particle.force += glm::vec2(0.f, particle.force.y * -1);
                particle.velocity.y = 0.f;
            }

            if (particle.position.x < minX)
            {
                particle.force += glm::vec2(particle.force.x * -1, 0.f);
                particle.velocity.x = 0.f;
            }
            else if (particle.position.x > maxX)
            {
                particle.force += glm::vec2(particle.force.x * -1, 0.f);
                particle.velocity.x = 0.f;
            }
        }

    }

    void ResetForces()
    {
        for (auto& particle : particles)
        {
            particle.force = { 0.f, 0.f };
        }
    }

    void Update()
    {
        ApplyGravity();
        DetectCollisions();
        CheckBorderCollisions();
        ApplyForces();
        for (int i = 0; i < particles.size(); i++)
        {
            //std::cout << particles.position.x << " " << particles.position.y << std::endl;
            //std::cout << particle.velocity.x << " " << particle.velocity.y << std::endl;
            //std::cout << particle.acceleration.x << " " << particle.acceleration.y << std::endl;
            std::cout << i << " " << particles[i].force.x << " " << particles[i].force.y << std::endl;
        }
        ResetForces();
    }

    void DetectCollisions()
    {
        for (int i = 0; i < particles.size(); i++)
            for (int j = 0; j < particles.size(); j++)
            {
                if (i != j)
                {
                    if (glm::distance(particles[i].position, particles[j].position) <= ParticleRadius * 2)
                    {
                        particles[i].force += -particles[i].mass * gravity * glm::dot(glm::vec2(0, -1), glm::normalize(particles[j].position - particles[i].position));
                    }
                }
            }
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