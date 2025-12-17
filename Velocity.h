#pragma once
#include "Point.h"
#include <cmath>
#include <iostream>

class Velocity {
public:
    Velocity() = default;
    
    // Конструктор из компонентов x, y
    Velocity(double vx, double vy) {
        vec = Point(vx, vy);
    }
    
    // Конструктор из вектора Point
    Velocity(const Point& vector) : vec(vector) {}
    
    void setVector(const Point& vector) {
        vec = vector;
    }
    
    Point vector() const {
        return vec;
    }
    
private:
    Point vec;
};

inline std::istream& operator>>(std::istream& stream, Velocity& velocity) {
    double vx, vy;
    stream >> vx >> vy;
    velocity.setVector(Point(vx, vy));
    return stream;
}