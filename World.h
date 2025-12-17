#pragma once
#include "Ball.hpp"
#include "Physics.h"
#include "Dust.h"
#include <vector>
#include <memory>

class Painter;

class World {
public:
    World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time);
    
    void createDust(const Point& position, const Color& color);
    
    // Метод для принудительной очистки всей пыли
    void clearAllDust();

private:
    Point topLeft;
    Point bottomRight;
    Physics physics;
    std::vector<Ball> balls;
    
    std::vector<std::unique_ptr<Dust>> dustParticles;
    
    double restTime = 0.;
};