#include "World.h"
#include "Painter.h"
#include <algorithm>
#include <fstream>

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {

    std::ifstream stream(worldFilePath);

    stream >> topLeft >> bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream.peek(), stream.good()) {
        Point center;
        Point velocityPoint;
        Color color;
        double radius;
        bool isCollidable;

        // Читаем координаты центра шара (x, y) и вектор
        // его скорости (vx, vy)
        stream >> center >> velocityPoint;

        //  Читаем три составляющие цвета шара
        stream >> color;

        // Читаем радиус шара
        stream >> radius;

        // Читаем свойство шара isCollidable, которое
        // указывает, требуется ли обрабатывать пересечение
        // шаров как столкновение. Если true - требуется.
        stream >> std::boolalpha >> isCollidable;

        Ball b(center, velocityPoint, radius, color, isCollidable);
        balls.push_back(b);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    static Color worldColor(1, 1, 1);
    painter.draw(topLeft, bottomRight, worldColor);

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }

    // Вызываем отрисовку каждого осколка
    for (const Ball& dust : dusts) {
        dust.draw(painter);
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    /**
     * В реальном мире время течет непрерывно. Однако
     * компьютеры дискретны по своей природе. Поэтому
     * симуляцию взаимодействия шаров выполняем дискретными
     * "тиками". Т.е. если с момента прошлой симуляции
     * прошло time секунд, time / timePerTick раз обновляем
     * состояние мира. Каждое такое обновление - тик -
     * в physics.update() перемещаем шары и обрабатываем
     * коллизии - ситуации, когда в результате перемещения
     * один шар пересекается с другим или с границей мира.
     * В общем случае время не делится нацело на
     * длительность тика, сохраняем остаток в restTime
     * и обрабатываем на следующей итерации.
     */

    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    // удаляем старые осколки
    dusts.erase(std::remove_if(dusts.begin(), dusts.end(),
                               [](Dust& p) { return p.expired(); }),
                dusts.end());

    physics.update(balls, dusts, ticks);
}