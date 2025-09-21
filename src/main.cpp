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
glm::vec2 gravity = glm::vec2( 0.f, -5.f ) / 300.f;
float elasticity = 1.0f;


// Структура частицы
struct Particle
{
    float radius = ParticleRadius;
    glm::vec2 position = { 0.f, 0.f };

    Particle() {}
    Particle(glm::vec2 position) : position(position) {}
    Particle(float r, glm::vec2 position) : radius(r), position(position) {}
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
    float borderThikness;
    float borderLength;
    float borderHeight;
    std::vector<glm::vec2> corners;
    std::vector<glm::vec2> normals;


    int numParticles;
    float localRadius;


    glm::mat2x2 transformMatrix;
    float aspectRatio;


    float rotationAngle = 0.f;
    std::vector<Particle> particles;

    

public:
    Border(float borderLength, float borderHeight, float borderThikness, int numParticles) :
        borderLength(borderLength),
        borderHeight(borderHeight),
        borderThikness(borderThikness),
        numParticles(numParticles)
        {
            aspectRatio = borderLength / borderHeight; // Нужно рассмотреть второй случай
            transformMatrix = glm::mat2x2(borderLength / 2, 0, 0, borderHeight * aspectRatio / 2);
            localRadius = 2 * ParticleRadius / borderLength;

            corners = { { -1, -1 / aspectRatio }, { -1, 1 / aspectRatio }, { 1, 1 / aspectRatio }, { 1, -1 / aspectRatio } }; // Clockwise
            normals = { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } }; // left, up, right, bottom

            CreateParticles(numParticles);
        }

    glm::vec2 ToLocal(glm::vec2 pos)
    {
        return glm::inverse(transformMatrix) * (pos - centerPosition);
    }

    glm::vec2 ToGlobal(glm::vec2 pos)
    {
        return transformMatrix * pos + centerPosition;
    }

    void DrawBorder(std::vector<glm::vec2> corners)
    {

        glLineWidth(borderThikness);
        glBegin(GL_LINE_LOOP);
        for (auto& corner : corners)
        {   
            glm::vec2 globalPos = ToGlobal(corner);
            glVertex2f(globalPos.x, globalPos.y);
        }
        glEnd();
    }

    void DrawParticles()
    {
        for (auto& particle : particles)
        {
            glm::vec2 absolutePos = ToGlobal(particle.position);
            DrawCircle(absolutePos.x, absolutePos.y, particle.radius, res = 20);
        }
    }

    void CheckBorderCollisions()
    {

        for (auto& particle : particles)
        {
            if (glm::dot(particle.position - corners[0] - normals[0] * localRadius, normals[0]) <= 0)
            {
                std::cout << "0" << std::endl;
                particle.position -= glm::dot(particle.position - corners[0] - normals[0] * localRadius, normals[0]) * normals[0];
            }
            else if (glm::dot(particle.position - corners[2] - normals[2] * localRadius, normals[2]) <= 0)
            {
                std::cout << "2" << std::endl;
                particle.position -= glm::dot(particle.position - corners[2] - normals[2] * localRadius, normals[2]) * normals[2];
            }

            if (glm::dot(particle.position - corners[1] - normals[1] * localRadius, normals[1]) <= 0)
            {
                std::cout << "1" << std::endl;
                particle.position -= glm::dot(particle.position - corners[1] - normals[1] * localRadius, normals[1]) * normals[1];
            }
            else if (glm::dot(particle.position - corners[3] - normals[3] * localRadius, normals[3]) <= 0)
            {
                std::cout << "3" << std::endl;
                particle.position -= glm::dot(particle.position - corners[3] - normals[3] * localRadius, normals[3]) * normals[3];
            }
        }

    }
    
    void applyForce(float dt)
    {
        for (auto& particle : particles)
        {
            particle.position += gravity * dt;
        }
    }

    void Update(float dt)
    {
        applyForce(dt);
        CheckBorderCollisions();
    }

    void Draw()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        DrawBorder(corners);
        DrawParticles();
    }

    void CreateParticles(int n = 1)
    {
        for (int i = 0; i < n; ++i)
        {
            Particle particle;
            particle.position = glm::vec2(0, 0);
            particles.push_back(particle);
        }
    }

    void Rotate(float angle)
    {
        rotationAngle += angle;

        glm::mat2x2 rotation_matrix = glm::mat2x2(cos(angle), -sin(angle), sin(angle), cos(angle));  
        for (auto& corner : corners)
        {
            corner = rotation_matrix * corner;
        }

        for (auto& normal : normals)
        {
            normal = rotation_matrix * normal;
        }


        for (auto& particle : particles)
        {
            particle.position = rotation_matrix * particle.position;
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



Border border(300.0f, 400.0f, 2.0f, 1);

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

        border.Rotate(0.01f);
        border.Update(1.f / 3.f);
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