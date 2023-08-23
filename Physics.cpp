#include "Physics.h"
#include "Dust.hpp"

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& _topLeft, const Point& _bottomRight) {
    topLeft = _topLeft;
    bottomRight = _bottomRight;
}

void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts,
                     const size_t ticks) const {

    for (size_t i = 0; i < ticks; ++i) {
        move(dusts);
        move(balls);
        collideWithBox(balls);
        collideBalls(balls, dusts);
    }
}

void Physics::collideBalls(std::vector<Ball>& balls,
                           std::vector<Dust>& dusts) const {

    for (auto a = balls.begin(); a != balls.end(); ++a) {
        if (!a->getIsCollidable()) {
            continue;
        }
        for (auto b = std::next(a); b != balls.end(); ++b) {
            if (!b->getIsCollidable()) {
                continue;
            }
            const double distanceBetweenCenters2 =
                distance2(a->getCenter(), b->getCenter());
            const double collisionDistance = a->getRadius() + b->getRadius();
            const double collisionDistance2 =
                collisionDistance * collisionDistance;

            if (distanceBetweenCenters2 < collisionDistance2) {
                processCollision(*a, *b, dusts, distanceBetweenCenters2);
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {

    for (Ball& ball : balls) {
        if (!ball.getIsCollidable()) {
            continue;
        }

        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            ball.setVelocity(vector);
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            ball.setVelocity(vector);
        }
    }
}

void Physics::move(std::vector<Dust>& dusts) const {
    for (auto& dust : dusts) {
        move(dust);
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (auto& ball : balls) {
        move(ball);
    }
}

void Physics::move(Ball& ball) const {
    Point newPos = ball.getCenter() + ball.getVelocity().vector() * timePerTick;
    ball.setCenter(newPos);
}

void Physics::processCollision(Ball& a, Ball& b, std::vector<Dust>& dusts,
                               double distanceBetweenCenters2) const {
    // нормированный вектор столкновения
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));

    auto neeDusts = Dust::getCollisionDusts(a, b);
    for (auto& d : neeDusts) {
        dusts.push_back(d);
    }
}
