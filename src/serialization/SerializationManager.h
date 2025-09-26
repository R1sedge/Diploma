#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <PhysicsEngine.h>
#include "Config.h"

class SerializationManager {
private:
    std::ofstream outputFile;
    std::string filename;
    int frameCount;
    int particleCount;
    bool isRecording;

public:
    SerializationManager();
    ~SerializationManager();

    void startRecording(const std::string& outputFilename, int numParticles);
    void recordFrame(int frameNumber, PhysicsEngine& physicsEngine);
    void stopRecording();
    bool isRecordingActive() const { return isRecording; }
};