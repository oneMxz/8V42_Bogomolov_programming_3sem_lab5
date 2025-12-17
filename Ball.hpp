#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include "Color.h"

class Ball {
public:
    Ball() = default;
    Ball(const Point& center, const Velocity& velocity, const Color& color, 
         double radius, bool isCollidable = true);
    
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    
    bool isCollidable() const;
    void setCollidable(bool collidable);
    Color getColor() const;

private:
    Point center;
    Velocity velocity;
    Color color;
    double radius = 0;
    bool collidable = true;
};