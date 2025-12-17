#include "Dust.h"
#include <cmath>

Dust::Dust(const Point& position, const Velocity& velocity, 
           const Color& color, double lifetime)
    : position(position), velocity(velocity), color(color),
      lifetime(lifetime), initialLifetime(lifetime) {
    // Гарантируем положительное время жизни
    if (initialLifetime <= 0) {
        initialLifetime = 1.0;
        this->lifetime = 1.0;
    }
}

void Dust::update(double dt) {
    // Если частица уже мертва, ничего не делаем
    if (lifetime <= 0.0) return;
    
    // Обновляем позицию
    position.x += velocity.vector().x * dt;
    position.y += velocity.vector().y * dt;
    
    lifetime -= dt * 1.2;  // Умножаем на 1.5 для более быстрого исчезновения
    
    if (lifetime < 0) {
        lifetime = 0;
    }
    
    // Обновляем позицию только если частица еще жива
    if (lifetime > 0) {
        position.x += velocity.vector().x * dt;
        position.y += velocity.vector().y * dt;
    }
}

bool Dust::isAlive() const {
    // Частица жива, если время жизни > очень маленького значения
    // Используем большее значение для надежности
    return lifetime > 0.005;
}

void Dust::draw(Painter& painter) const {
    if (!isAlive()) return;
    
    // Вычисляем, какая часть жизни осталась
    double lifeRatio = lifetime / initialLifetime;
    
    // Размер уменьшается со временем (квадратично для красоты)
    double size = 3.0 * lifeRatio * lifeRatio;
    
    // Если размер стал слишком маленьким, не рисуем
    if (size < 0.3) return;
    
    // Цвет тоже немного бледнеет
    Color fadedColor(
        color.red() * (0.7 + 0.3 * lifeRatio),
        color.green() * (0.7 + 0.3 * lifeRatio),
        color.blue() * (0.7 + 0.3 * lifeRatio)
    );
    
    painter.draw(position, size, fadedColor);
}