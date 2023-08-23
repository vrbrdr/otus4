#pragma once
#include "Ball.hpp"
#include "Color.h"
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include <vector>

class Dust : public Ball {
  public:
    /// @brief Возвращает частички осколков при столкновении двух шаров
    /// @param ball1 первый столкнувшийся шар
    /// @param ball2 второй столкнувшийся шар
    /// @return  получисшиеся от столкновения осколки
    static std::vector<Dust> getCollisionDusts(Ball ball1, Ball ball2);
    bool expired() const;

  private:
    double _maxPathdistance;
    Point _originalCenter;

  private:
    Dust(Point center, Velocity velocity, double radius, Color color,
         double maxPathdistance)
        : Ball(center, velocity, radius, color, false), _originalCenter{center},
          _maxPathdistance{maxPathdistance} {}

    /// @brief Получить точку касания шаров при столкновении
    static Point getCollisionPoint(Ball ball1, Ball ball2);
};