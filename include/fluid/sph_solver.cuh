// Просто затычка

#pragma once

struct Particle {
    float x, y, z;
    float vx, vy, vz;
};

class SphSolver {
public:
    void initialize();
    void update();
};