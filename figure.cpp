#include "figure.h"

// Класс прямоугольник
RectangleShape::RectangleShape(const QRect &otherRect) : rect(otherRect) {}

// Метод для отрисовки прямоугольника
void RectangleShape::draw(QPainter &painter) const {
    painter.drawRect(rect);
}

// Метод для проверки, содержит ли прямоугольник данную точку
bool RectangleShape::contains(const QPoint &point) const {
    return rect.contains(point);
}

// Метод для перемещения прямоугольника
void RectangleShape::move(const QPoint &offset) {
    rect.translate(offset);
}

// Метод для получения области, занимаемой прямоугольником
QRect RectangleShape::boundingRect() const {
    return rect;
}

// Метод для установки новых координат прямоугольника
void RectangleShape::setRect(const QRect &newRect){
    rect = newRect;
}


// Класс эллипс
EllipseShape::EllipseShape(const QRect &otherRect) : rect(otherRect) {}

// Метод для отрисовки эллипса
void EllipseShape::draw(QPainter &painter) const {
    painter.drawEllipse(rect);
}

// Метод для проверки, содержит ли эллипс данную точку
bool EllipseShape::contains(const QPoint &point) const {
    return rect.contains(point);
}

// Метод для перемещения эллипса
void EllipseShape::move(const QPoint &offset) {
    rect.translate(offset);
}

// Метод для получения области, занимаемой эллипсом
QRect EllipseShape::boundingRect() const {
    return rect;
}

// Метод для установки новых координат эллипса
void EllipseShape::setRect(const QRect &newRect){
    rect = newRect;
}


// Класс треугольник
TriangleShape::TriangleShape(const QRect &otherRect) : rect(otherRect) {}

// Метод для отрисовки треугольника
void TriangleShape::draw(QPainter &painter) const {
    QPolygon triangle;
    triangle << QPoint(rect.left() + rect.width() / 2, rect.top())
             << QPoint(rect.bottomLeft())
             << QPoint(rect.bottomRight());
    painter.drawPolygon(triangle);
}

// Метод для проверки, содержит ли треугольник данную точку
bool TriangleShape::contains(const QPoint &point) const {
    return rect.contains(point);
}

// Метод для перемещения треугольника
void TriangleShape::move(const QPoint &offset) {
    rect.translate(offset);
}

// Метод для получения области, занимаемой треугольником
QRect TriangleShape::boundingRect() const {
    return rect;
}

// Метод для установки новых координат треугольника
void TriangleShape::setRect(const QRect &newRect){
    rect = newRect;
}
