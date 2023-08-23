#pragma once
#include "Color.h"
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include <iostream>

class Ball {
  private:
    Velocity _velocity;
    Point _center;
    double _radius;
    Color _color;
    bool _isCollidable;

  public:
    Ball(Point center, Velocity velocity, double radius, Color color,
         bool isCollidable)
        : _center{center}, _velocity{velocity}, _radius{radius}, _color{color},
          _isCollidable{isCollidable} {}

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    Color getColor() const;
    bool getIsCollidable() const;
    void draw(Painter& painter) const;
};
