#pragma once
#include "Point.h"
#include "Velocity.h"
#include "Painter.h"
#include "Color.h"
#include <memory>

class Dust {
public:
    Dust(const Point& position, const Velocity& velocity, 
         const Color& color, double lifetime = 0.5);
    
    void update(double dt);
    bool isAlive() const;
    void draw(Painter& painter) const;
    
private:
    Point position;
    Velocity velocity;
    Color color;
    double lifetime;
    double initialLifetime;
};