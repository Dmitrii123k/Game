#include "apple.h"

Apple::Apple(QObject *parent) : QObject(parent), QGraphicsItem() {
  color = qrand()%3 + 1; // Cлучайный номер цвета яблока 1-3
}

Apple::~Apple() { }

QRectF Apple::boundingRect() const {
 return QRectF(-20,-20,40,40);// Ограничиваем область, в которой лежит яблоко
}

void Apple::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);  Q_UNUSED(widget); //Пока не используем эти параметры
    // Отрисовка яблока
    QPainterPath path(QPointF(0,-10));
    path.cubicTo (0,-10, -5,-14,  -12,-8);
    path.cubicTo (-12,-8, -20,12, -10,15);
    path.cubicTo (-10,15, -5,20, 0,16);
    path.cubicTo (0,16, 5,20, 10,15);
    path.cubicTo (10,15, 20,12, 12,-8);
    path.cubicTo (12,-8, 5,-14,  0,-10);
    // Выбор цвета
    switch (color) {
     case 1:
        painter->setBrush(Qt::red);
     break;
     case 2:
        painter->setBrush(Qt::green);
     break;
     case 3:
        painter->setBrush(Qt::yellow);
     break;
    }
    painter->drawPath(path);
    // Хвостик
    painter->setPen(QPen(Qt::black, 2));
    QPainterPath path2(QPointF(0,-10));
    path2.cubicTo(0,-10,4,-18,10,-20);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path2);
    //Листик
    painter->setPen(QPen(Qt::black, 1));
    QPainterPath path3(QPointF(0,-10));
    path3.cubicTo(0,-10,-2,-20,-15,-20);
    path3.cubicTo(-15,-20,-14,-12,0,-10);
    painter->setBrush(Qt::green);
    painter->drawPath(path3);
}
