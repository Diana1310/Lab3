#include "draw.h"

#include "draw.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QImage>
#include <QDebug>

DrawingArea::DrawingArea(QWidget *parent)
    : QFrame(parent), currentMode(Mode::None), movingIndex(-1)
{
    // Устанавливаем форму фрейма в виде прямоугольной коробки
    setFrameShape(QFrame::Box);
    // Задаем стиль фона и границы
    setStyleSheet("background-color: white; border: 1px solid black;");
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    // Если нажата правая кнопка мыши
    if (event->button() == Qt::RightButton) {
        // Останавливаем текущее рисование и удаляем фигуру
        currentShape = nullptr;
        currentMode = Mode::None;
        update();
        return;
    }

    switch (currentMode) {
    case Mode::Move:
        // Если нажата левая кнопка мыши
        if (event->button() == Qt::LeftButton) {
            // Проверяем, содержит ли какая-либо из фигур нажатую точку
            for (int i = 0; i < shapes.size(); i++) {
                if (shapes[i]->contains(event->pos())) {
                    // Запоминаем индекс перемещаемой фигуры и последнюю позицию мыши
                    movingIndex = i;
                    lastMousePos = event->pos();
                    // Меняем курсор на закрытую ладонь
                    setCursor(Qt::ClosedHandCursor);
                    return;
                }
            }
            // Если нажата правая кнопка мыши
        } else if (event->button() == Qt::RightButton) {
            // Сбрасываем индекс перемещаемой фигуры и меняем курсор на стрелку
            movingIndex = -1;
            setCursor(Qt::ArrowCursor);
            update();
        }
        break;
    case Mode::Delete:
        // Если нажата левая кнопка мыши
        if (event->button() == Qt::LeftButton) {
            // Проверяем, содержит ли какая-либо из фигур нажатую точку
            for (int i = 0; i < shapes.size(); i++) {
                if (shapes[i]->contains(event->pos())) {
                    // Удаляем фигуру и обновляем область
                    removeShape(i);
                    update();
                    return;
                }
            }
            // Если нажата правая кнопка мыши
        } else if (event->button() == Qt::RightButton) {
            // Переключаемся в режим "Нет"
            currentMode = Mode::None;
            update();
        }
        break;
    case Mode::Connection:
        // Если нажата левая кнопка мыши
        if (event->button() == Qt::LeftButton) {
            // Проверяем, содержит ли какая-либо из фигур нажатую точку
            for (int i = 0; i < shapes.size(); ++i) {
                if (shapes[i]->contains(event->pos())) {
                    // Если точка начала соединения не определена
                    if (connectionStartPoint.isNull()) {
                        // Запоминаем центр текущей фигуры как точку начала соединения
                        connectionStartPoint = shapes[i]->boundingRect().center();
                        connectionEndPoint = connectionStartPoint;
                        startShapeIndex = i;
                    } else {
                        // Определяем точку конца соединения и добавляем соединение
                        connectionEndPoint = shapes[i]->boundingRect().center();
                        if (startShapeIndex != i) {
                            connections.push_back(std::make_pair(startShapeIndex, i));
                        }
                        connectionStartPoint = QPoint();
                        connectionEndPoint = QPoint();
                        currentMode = Mode::None;
                    }
                    update();
                    return;
                }
            }
            // Если точка начала соединения не определена, сбрасываем ее
            connectionStartPoint = QPoint();
            connectionEndPoint = QPoint();
            currentMode = Mode::None;
            update();
            // Если нажата правая кнопка мыши
        } else if (event->button() == Qt::RightButton) {
            // Сбрасываем точки начала и конца соединения, а также режим
            connectionStartPoint = QPoint();
            connectionEndPoint = QPoint();
            currentMode = Mode::None;
            update();
        }
        break;
    case Mode::Rectangle:
    case Mode::Ellipse:
    case Mode::Triangle:
        // Если нажата левая кнопка мыши
        if (event->button() == Qt::LeftButton) {
            // Запоминаем начальную точку
            startPoint = event->pos();
            // Создаем прямоугольник с начальной и конечной точкой в одном месте
            QRect rect;
            rect.setTopLeft(startPoint);
            rect.setBottomRight(startPoint);
            // Создаем фигуру в соответствии с текущим режимом
            if (currentMode == Mode::Rectangle) {
                currentShape = std::make_shared<RectangleShape>(rect);
            } else if (currentMode == Mode::Ellipse) {
                currentShape = std::make_shared<EllipseShape>(rect);
            } else if (currentMode == Mode::Triangle) {
                currentShape = std::make_shared<TriangleShape>(rect);
            }
            update();
        }
        update();
        break;
    default:
        break;
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    // Если текущий режим - "Перемещение" и какая-то фигура перемещается
    if (currentMode == Mode::Move && movingIndex >= 0) {
        // Вычислить смещение между текущей и предыдущей позицией мыши
        QPoint offset = event->pos() - lastMousePos;
        // Переместить фигуру на рассчитанное смещение
        shapes[movingIndex]->move(offset);
        // Обновить предыдущую позицию мыши
        lastMousePos = event->pos();
        // Запросить перерисовку области рисования
        update();
    }
    // Если текущий режим - "Соединение" и точка начала соединения установлена
    else if (currentMode == Mode::Connection && !connectionStartPoint.isNull()) {
        // Обновить точку конца соединения на текущую позицию мыши
        connectionEndPoint = event->pos();
        // Запросить перерисовку области рисования
        update();
    }
    // Если текущий режим не "Нет" и есть текущая фигура
    else if (currentMode != Mode::None && currentShape) {
        // Вычислить ограничивающий прямоугольник текущей фигуры на основе текущей позиции мыши
        QRect rect = currentShape->boundingRect();
        rect.setBottomRight(event->pos());
        // Обновить размер текущей фигуры
        currentShape->setRect(rect);
        // Запросить перерисовку области рисования
        update();
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    // Если текущий режим - "Перемещение" и фигура перемещалась, и отпущена левая кнопка мыши
    if (currentMode == Mode::Move && movingIndex >= 0 && event->button() == Qt::LeftButton) {
        // Остановить перемещение фигуры
        movingIndex = -1;
        // Сбросить курсор к стандартной стрелке
        setCursor(Qt::ArrowCursor);
        // Запросить перерисовку области рисования
        update();
    }
    // Если есть текущая фигура и отпущена левая кнопка мыши
    else if (currentShape && event->button() == Qt::LeftButton) {
        // Если нажата правая кнопка мыши, отменить создание фигуры
        if (event->button() == Qt::RightButton){
            currentShape = nullptr;
            currentMode = Mode::None;
            return;
        }
        // Добавить текущую фигуру в список фигур
        shapes.push_back(currentShape);
        // Сбросить текущую фигуру и режим
        currentShape = nullptr;
        currentMode = Mode::None;
        // Запросить перерисовку области рисования
        update();
    }
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    // Вызвать paintEvent родительского класса
    QFrame::paintEvent(event);
    // Создать painter для области рисования
    QPainter painter(this);

    // Нарисовать все существующие фигуры
    for (const auto &shape : shapes) {
        shape->draw(painter);
    }

    // Нарисовать все соединения между фигурами
    for (const auto &connection : connections) {
        painter.drawLine(shapes[connection.first]->boundingRect().center(),
                         shapes[connection.second]->boundingRect().center());
    }

    // Если есть текущая фигура, нарисовать ее
    if (currentShape) {
        currentShape->draw(painter);
    }

    // Если текущий режим - "Соединение" и точка начала соединения установлена, нарисовать линию соединения
    if (currentMode == Mode::Connection && !connectionStartPoint.isNull()) {
        painter.drawLine(connectionStartPoint, connectionEndPoint);
    }
}

void DrawingArea::keyPressEvent(QKeyEvent *event) {
    // Если текущий режим не "Нет" и нажата клавиша "Escape"
    if (currentMode != Mode::None && event->key() == Qt::Key_Escape) {
        // Отменить текущее создание фигуры или режим
        currentShape = nullptr;
        currentMode = Mode::None;
        update();
    }
}

void DrawingArea::removeConnections(int shapeIndex)
{
    // Создать новый вектор для хранения обновленных соединений
    QVector<QPair<int, int>> newConnections;

    // Пройти по существующим соединениям
    for (const auto &connection : connections) {
        // Если соединение не касается указанного индекса фигуры, добавить его в новый вектор соединений
        if (connection.first != shapeIndex && connection.second != shapeIndex) {
            newConnections.push_back(connection);
        }
    }

    // Обновить индексы соединений для оставшихся соединений, если удаленный индекс фигуры был до них
    for (auto &connection : newConnections) {
        if (connection.first > shapeIndex) {
            --connection.first;
        }
        if (connection.second > shapeIndex) {
            --connection.second;
        }
    }

    // Обновить вектор соединений новыми соединениями
    connections = newConnections;
    // Запросить перерисовку области рисования
    update();
}

void DrawingArea::removeShape(int index)
{
    // Удалить соединения, связанные с указанным индексом фигуры
    removeConnections(index);

    // Если указанный индекс находится в диапазоне вектора фигур, удалить фигуру
    if (index >= 0 && index < shapes.size()) {
        shapes.remove(index);
    }
}

void DrawingArea::saveToFile(const QString &fileName)
{
    // Открыть файл для записи
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        // Создать поток данных для записи в файл
        QDataStream out(&file);

        // Записать количество фигур
        int count = shapes.size();
        out << count;

        // Записать информацию о каждой фигуре
        for (const auto &shape : shapes) {
            if (dynamic_cast<RectangleShape*>(shape.get())) {
                out << QString("Rectangle") << shape->boundingRect();
            } else if (dynamic_cast<EllipseShape*>(shape.get())) {
                out << QString("Ellipse") << shape->boundingRect();
            } else if (dynamic_cast<TriangleShape*>(shape.get())) {
                out << QString("Triangle") << shape->boundingRect();
            }
        }

        // Записать количество соединений
        int count2 = connections.size();
        out << count2;

        // Записать информацию о каждом соединении
        for (const auto &connection : connections) {
            out << connection.first << connection.second;
        }
    }
}

void DrawingArea::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        int shapeCount;
        in >> shapeCount;
        shapes.clear();
        for (int i = 0; i < shapeCount; ++i) {
            QString shapeType;
            QRect rect;
            in >> shapeType >> rect;

            if (shapeType == "Rectangle") {
                shapes.push_back(std::make_shared<RectangleShape>(rect));
            } else if (shapeType == "Ellipse") {
                shapes.push_back(std::make_shared<EllipseShape>(rect));
            } else if (shapeType == "Triangle") {
                shapes.push_back(std::make_shared<TriangleShape>(rect));
            }
        }

        int connectionCount;
        in >> connectionCount;
        connections.clear();
        for (int i = 0; i < connectionCount; ++i) {
            int first, second;
            in >> first >> second;
            connections.push_back(qMakePair(first, second));
        }
    }

    update();
}

