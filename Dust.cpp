#include "Dust.hpp"

std::vector<Dust> Dust::getCollisionDusts(Ball ball1, Ball ball2) {
    std::vector<Dust> res;

    Point dustCenter = getCollisionPoint(ball1, ball2);

    // ПО простому, 4 осколка разлетаются в разные стороны два от одного шара и
    // два от другого

    // фиксированное количество осколков
    static const int dustCount{32};
    // шаг угла для вектора скорости шара
    static const auto angleStep{360. / dustCount};

    // скорость разлета =  суммарная скорость обоих шаров
    auto v1 = ball1.getVelocity().vector();
    auto v2 = ball2.getVelocity().vector();
    auto speed =
        sqrt(v1.x * v1.x + v1.y * v1.y) + sqrt(v2.x * v2.x + v2.y * v2.y);

    for (int i = 0; i < dustCount; ++i) {
        auto angle = angleStep * i;

        // вектор скорости равномерно во все стороны от точки соударения
        Velocity dustVelocity(Point(speed * cos(angle), speed * sin(angle)));

        // для вычисления параметров осколка берем параметры одного из
        // столкнувшихся шаров
        auto ball{i % 2 ? ball1 : ball2};

        // цвет осколка берется от "своего шара"
        Color dustColor{ball.getColor()};

        // радиус осколка зависит от размера "своего шара"
        double radius{ball.getRadius() / 5};

        // длина пути осколка
        double maxPathdistance{ball.getRadius() * 5};

        res.push_back(
            Dust(dustCenter, dustVelocity, radius, dustColor, maxPathdistance));
    }

    return res;
}

bool Dust::expired() const {
    auto pathdistance = sqrt(distance2(_originalCenter, getCenter()));
    return pathdistance > _maxPathdistance;
}

Point Dust::getCollisionPoint(Ball ball1, Ball ball2) {
    // формула точки на отрезке
    // Let a(p,q), b(r,s). Then, what you want is
    // ((1−t)p+tr,(1−t)q+ts)
    // where t is a real number such that 0≤t≤1.
    // Setting t=0  gives you a and setting t=1  gives you b
    auto c1 = ball1.getCenter();
    auto c2 = ball2.getCenter();
    auto t = ball1.getRadius() / (ball1.getRadius() + ball2.getRadius());
    return Point((1 - t) * c1.x + t * c2.x, (1 - t) * c2.y + t * c2.y);
}
