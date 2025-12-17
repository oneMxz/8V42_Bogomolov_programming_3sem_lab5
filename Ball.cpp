
#include "Ball.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ball::Ball(const Point& center, const Velocity& velocity, const Color& color, 
           double radius, bool isCollidable)
    : center(center), velocity(velocity), color(color), 
      radius(radius), collidable(isCollidable) {}

void Ball::setVelocity(const Velocity& velocity) {
    this->velocity = velocity;
}

Velocity Ball::getVelocity() const {
    return velocity;
}

void Ball::draw(Painter& painter) const {
    painter.draw(center, radius, color);
}

void Ball::setCenter(const Point& center) {
    this->center = center;
}

Point Ball::getCenter() const {
    return center;
}

double Ball::getRadius() const {
    return radius;
}

double Ball::getMass() const {
    return (4.0 / 3.0) * M_PI * radius * radius * radius;
}

bool Ball::isCollidable() const {
    return collidable;
}

void Ball::setCollidable(bool collidable) {
    this->collidable = collidable;
}

Color Ball::getColor() const {
    return color;
}
