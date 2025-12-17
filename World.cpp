#include "World.h"
#include "Painter.h"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iostream>

static constexpr double timePerTick = 0.001;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

World::World(const std::string& worldFilePath) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    std::ifstream stream(worldFilePath);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + worldFilePath);
    }
    
    stream >> topLeft >> bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    // Чтение данных о шарах
    while (stream.peek() != EOF && stream.good()) {
        Point center;
        Velocity velocity;
        Color color;
        double radius;
        std::string isCollidableStr;
        
        if (stream >> center >> velocity >> color >> radius >> isCollidableStr) {
            bool isCollidable = (isCollidableStr == "true" || isCollidableStr == "1");
            balls.emplace_back(center, velocity, color, radius, isCollidable);
        }
    }
}

void World::show(Painter& painter) const {
    // Рисуем границу мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));
    
    // Сначала рисуем частицы (они должны быть под шарами)
    for (const auto& dust : dustParticles) {
        if (dust->isAlive()) {
            dust->draw(painter);
        }
    }
    
    // Затем рисуем шары (они поверх частиц)
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }
}

void World::update(double time) {
    // 1. ВСЕГДА обновляем частицы на прошедшее время
    for (auto& dust : dustParticles) {
        dust->update(time);
    }
    
    // 2. Сразу удаляем мертвые частицы
    for (auto it = dustParticles.begin(); it != dustParticles.end(); ) {
        if (!(*it)->isAlive()) {
            it = dustParticles.erase(it);
        } else {
            ++it;
        }
    }
    
    // 3. Только потом обновляем физику шаров (если нужно)
    restTime += time;
    const auto ticks = static_cast<size_t>(std::floor(restTime / timePerTick));
    
    if (ticks > 0) {
        // Для детекции столкновений сохраняем старые скорости
        static std::vector<Velocity> previousVelocities = [&]() {
            std::vector<Velocity> v;
            for (const auto& ball : balls) {
                v.push_back(ball.getVelocity());
            }
            return v;
        }();
        
        // Обновляем физику
        physics.update(balls, ticks);
        
        // Детектируем столкновения по изменению скорости
        for (size_t i = 0; i < balls.size(); ++i) {
            Velocity oldVel = previousVelocities[i];
            Velocity newVel = balls[i].getVelocity();
            
            Point oldVec = oldVel.vector();
            Point newVec = newVel.vector();
            
            double dx = newVec.x - oldVec.x;
            double dy = newVec.y - oldVec.y;
            double velChange2 = dx * dx + dy * dy;
            
            // Если скорость значительно изменилась - было столкновение
            if (velChange2 > 50.0 && balls[i].isCollidable()) {
                createDust(balls[i].getCenter(), balls[i].getColor());
            }
            
            // Обновляем сохраненную скорость
            previousVelocities[i] = newVel;
        }
        
        // Вычитаем обработанное время
        restTime -= ticks * timePerTick;
    }
}

void World::createDust(const Point& position, const Color& color) {
    // Создаём 5-8 частиц в случайных направлениях
    int count = 5 + std::rand() % 4;
    for (int i = 0; i < count; ++i) {
        double angle = (std::rand() / (double)RAND_MAX) * 2 * M_PI;
        double speed = 30 + (std::rand() / (double)RAND_MAX) * 40; // 30-70
        
        // Создаем вектор скорости
        Velocity vel(Point(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        ));
        
        // Добавляем небольшое случайное смещение
        Point dustPos = position;
        dustPos.x += (std::rand() / (double)RAND_MAX - 0.5) * 10;
        dustPos.y += (std::rand() / (double)RAND_MAX - 0.5) * 10;
        
        // Создаем частицу с временем жизни 1.5 секунды
        dustParticles.push_back(
            std::make_unique<Dust>(dustPos, vel, color, 1.5)
        );
    }
}

void World::clearAllDust() {
    dustParticles.clear();
    std::cout << "All dust particles cleared" << std::endl;
}