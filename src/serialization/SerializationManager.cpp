#include "SerializationManager.h"
#include <iostream>
#include <iomanip>

SerializationManager::SerializationManager()
    : frameCount(0), particleCount(0), isRecording(false) {
}

SerializationManager::~SerializationManager() {
    if (isRecording) {
        stopRecording();
    }
}

void SerializationManager::startRecording(const std::string& outputFilename, int numParticles) {
    if (isRecording) {
        stopRecording();
    }

    filename = outputFilename;
    outputFile.open(filename);
    particleCount = numParticles;
    frameCount = 0;
    isRecording = true;

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        isRecording = false;
        return;
    }

    // Записываем meta-данные
    outputFile << "m version 1.0" << std::endl;
    outputFile << "m particle_count " << particleCount << std::endl;
    outputFile << "m particle_radius " << Config::PARTICLE_RADIUS << std::endl;
    outputFile << "m screen_width " << Config::WINDOW_WIDTH << std::endl;
    outputFile << "m screen_height " << Config::WINDOW_HEIGHT << std::endl;
    outputFile << "m border_length " << Config::BORDER_LENGTH << std::endl;
    outputFile << "m border_height " << Config::BORDER_HEIGHT << std::endl;
    outputFile << "m border_thickness " << Config::BORDER_THICKNESS << std::endl;
    outputFile << "m gravity_x " << Config::GRAVITY.x << std::endl;
    outputFile << "m gravity_y " << Config::GRAVITY.y << std::endl;
    outputFile << "m elasticity " << Config::ELASTICITY << std::endl;
    outputFile << std::endl; // Пустая строка для разделения
}

void SerializationManager::recordFrame(int frameNumber, PhysicsEngine& physicsEngine) {
    if (!isRecording || !outputFile.is_open()) {
        return;
    }

    // Записываем номер кадра
    outputFile << "f " << frameNumber << std::endl;

    // Записываем параметры границы (углы)
    outputFile << "b";

    for (const auto& corner : physicsEngine.border.getCorners()) {
        outputFile << " " << std::fixed << std::setprecision(6) << corner.x
            << " " << std::fixed << std::setprecision(6) << corner.y;
    }
    outputFile << std::endl;

    // Записываем положения частиц
    for (const auto& particle : physicsEngine.particleSystem.getParticles()) {
        outputFile << "p " << std::fixed << std::setprecision(6) << particle.position_current.x
            << " " << std::fixed << std::setprecision(6) << particle.position_current.y << std::endl;
    }

    outputFile << std::endl; // Пустая строка между кадрами
    frameCount++;
}

void SerializationManager::stopRecording() {
    if (outputFile.is_open()) {
        outputFile << "m total_frames " << frameCount << std::endl;
        outputFile.close();
    }
    isRecording = false;
    std::cout << "Recording stopped. Saved " << frameCount << " frames to " << filename << std::endl;
}