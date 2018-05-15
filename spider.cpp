#include "spider.h"
#include "math.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle) { //Нормализовать угол в пределы [0;2*Pi]
 while (angle < 0) angle += TwoPi;
 while (angle > TwoPi) angle -= TwoPi;
 return angle;
}

Spider::Spider(QGraphicsItem *target, QObject *parent) : QObject(parent), QGraphicsItem() {
    angle = 0;      // Задаём угол поворота графического объекта
    steps = 0;      // Задаём стартовое положение ножек мухи
    countForSteps = 0;      // Счётчик для отсчета тиков таймера, при которых мы нажимали на кнопки
    setRotation(angle);     // Устанавливаем угол поворота графического объекта
    this->target = target;  // Устанавливаем цель паука
    timer = new QTimer();   // Инициализируем игровой таймер паука
    connect(timer, &QTimer::timeout, this, &Spider::slotGameTimer); // Подключаем сигнал таймера к игровому слоту паука
    timer->start (20);       // Запускаем таймер, делаем паука помедленней :)
}

Spider::~Spider() { }

QRectF Spider::boundingRect() const {
 return QRectF(-40,-50,80,100); // Ограничиваем область, в которой располагается паук
}

void Spider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) { // Отрисовка паука
    Q_UNUSED(option); Q_UNUSED(widget); // Пока не используем эти параметры
    painter->setPen(QPen(Qt::black, 2)); // Перо
    // Отрисовка ног
    if(steps == 0) { // Первое положение ног
        // Левая 1
        painter->drawLine(-24,-45,-28,-35);
        painter->drawLine(-28,-35,-22,-10);
        painter->drawLine(-22,-10,0,0);
        // Правая 1
        painter->drawLine(24,-45,28,-35);
        painter->drawLine(28,-35,22,-10);
        painter->drawLine(22,-10,0,0);

        // Левая 2
        painter->drawLine(-35,-38,-30,-18);
        painter->drawLine(-30,-18,-25,-3);
        painter->drawLine(-25,-3,0,0);
        // Правая 2
        painter->drawLine(35,-38,30,-18);
        painter->drawLine(30,-18,25,-3);
        painter->drawLine(25,-3,0,0);

        // Левая 3
        painter->drawLine(-35,38,-30,18);
        painter->drawLine(-30,18,-25,3);
        painter->drawLine(-25,3,0,0);
        // Правая 3
        painter->drawLine(35,38,30,18);
        painter->drawLine(30,18,25,3);
        painter->drawLine(25,3,0,0);

        // Левая 4
        painter->drawLine(-24,45,-28,35);
        painter->drawLine(-28,35,-22,10);
        painter->drawLine(-22,10,0,0);
        // Правая 4
        painter->drawLine(24,45,28,35);
        painter->drawLine(28,35,22,10);
        painter->drawLine(22,10,0,0);
    }
    else if (steps == 1) { // Второе положение ног
        // Левая 1
        painter->drawLine(-23,-40,-24,-30);
        painter->drawLine(-24,-30,-19,-9);
        painter->drawLine(-19,-9,0,0);
        // Правая 1
        painter->drawLine(20,-50,23,-40);
        painter->drawLine(23,-40,15,-12);
        painter->drawLine(15,-12,0,0);

        // Левая 2
        painter->drawLine(-30,-35,-27,-24);
        painter->drawLine(-27,-24,-23,-5);
        painter->drawLine(-23,-5,0,0);
        // Правая 2
        painter->drawLine(40,-27,35,-10);
        painter->drawLine(35,-10,28,-1);
        painter->drawLine(28,-1,0,0);

        // Левая 3
        painter->drawLine(-40,27,-35,10);
        painter->drawLine(-35,10,-28,1);
        painter->drawLine(-28,1,0,0);
        // Правая 3
        painter->drawLine(30,35,27,24);
        painter->drawLine(27,24,23,5);
        painter->drawLine(23,5,0,0);

        // Левая 4
        painter->drawLine(-20,50,-27,30);
        painter->drawLine(-27,30,-20,12);
        painter->drawLine(-20,12,0,0);
        // Правая 4
        painter->drawLine(23,40,24,30);
        painter->drawLine(24,30,19,9);
        painter->drawLine(19,9,0,0);
    }
    else if (steps == 2) { // Третье положение ног
        // Левая 1
        painter->drawLine(-20,-50,-23,-40);
        painter->drawLine(-23,-40,-15,-12);
        painter->drawLine(-15,-12,0,0);
        // Правая 1
        painter->drawLine(23,-40,24,-30);
        painter->drawLine(24,-30,19,-9);
        painter->drawLine(19,-9,0,0);

        // Левая 2
        painter->drawLine(-40,-27,-35,-10);
        painter->drawLine(-35,-10,-28,-1);
        painter->drawLine(-28,-1,0,0);
        // Правая 2
        painter->drawLine(30,-35,27,-24);
        painter->drawLine(27,-24,23,-5);
        painter->drawLine(23,-5,0,0);

        // Левая 3
        painter->drawLine(-30,35,-27,24);
        painter->drawLine(-27,24,-23,5);
        painter->drawLine(-23,5,0,0);
        // Правая 3
        painter->drawLine(40,27,35,10);
        painter->drawLine(35,10,28,1);
        painter->drawLine(28,1,0,0);

        // Левая 4
        painter->drawLine(-23,40,-24,30);
        painter->drawLine(-24,30,-19,9);
        painter->drawLine(-19,9,0,0);
        // Правая 4
        painter->drawLine(20,50,27,30);
        painter->drawLine(27,30,20,12);
        painter->drawLine(20,12,0,0);
    }

    painter->setPen(QPen(Qt::black, 1));
    // Левое жвало
    QPainterPath path1(QPointF(0, -20));
    path1.cubicTo(0, -20, -5, -25, -3, -35);
    path1.cubicTo(-3,-35,-15,-25,-8,-17);
    path1.cubicTo(-8,-17,-5,15,0,-20 );
    painter->setBrush(Qt::black);
    painter->drawPath(path1);

    // Правое жвало
    QPainterPath path2(QPointF(0, -20));
    path2.cubicTo(0, -20, 5, -25, 3, -35);
    path2.cubicTo(3,-35,15,-25,8,-17);
    path2.cubicTo(8,-17,5,15,0,-20 );
    painter->setBrush(Qt::black);
    painter->drawPath(path2);

    // Голова
    painter->setBrush(QColor(146, 115, 40, 255));
    painter->drawEllipse(-10,-25,20,15);
    // Тело
    painter->drawEllipse(-15, -15, 30, 30);
    // Задняя часть
    painter->drawEllipse(-20, 0, 40,50);
    painter->setPen(QPen(Qt::white,3));
    painter->drawLine(-10,25,10,25);
    painter->drawLine(0,35,0,15);
    // Левый глаз
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(Qt::red);
    painter->drawEllipse(-8,-23,6,8);
    // Правый глаз
    painter->setBrush(Qt::red);
    painter->drawEllipse(2,-23,6,8);
}

void Spider::slotGameTimer() { // Поведение паука
    QLineF lineToTarget (QPointF(0, 0), mapFromItem(target, 0, 0)); // Определяем расстояние до мухи
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length()); // Угол поворота по направлению к мухе
    if (lineToTarget.dy() < 0) angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    // Нправление поворота в данном тике, скорость зависит от угла поворота
    if (angleToTarget > 0 && angleToTarget < Pi) { // Поворот влево с заданным углом
        if(angleToTarget > Pi / 5) { angle = -15; }
        else if(angleToTarget > Pi / 10) { angle = -5; }
        else { angle = -1; }
    }
    else if (angleToTarget <= TwoPi && angleToTarget > (TwoPi - Pi)) { // Поворот вправо с заданным углом
        if (angleToTarget < (TwoPi - Pi / 5)) { angle = 15; }
        else if (angleToTarget < (TwoPi - Pi / 10)) { angle = 5; }
        else { angle = 1; }
    }
    else if(angleToTarget == 0) { angle = 0; }

    setRotation(rotation() + angle); // Разворот

    // Движение в сторону мухи
    if (lineToTarget.length() >= 40) {
        setPos(mapToParent(0, -(qrand() % ((4 + 1) - 1) + 1)));
        // Движение ног
        countForSteps++;
        if (countForSteps == 6) { steps = 1;  }
        else if (countForSteps == 12) { steps = 0; }
        else if (countForSteps == 18) { steps = 2; }
        else if (countForSteps == 24) { steps = 0; countForSteps = 0; }
        update(QRectF(-40,-50,80,100));
   }

    /* Чтобы определить, наткнулся ли паук на какой-нибудь
     * элемент на графической сцене, задаём небольшую область перед пауком,
     * в которой будем искать элементы */
    QList <QGraphicsItem *> foundItems = scene()->items(QPolygonF()
     << mapToScene(0, 0)
     << mapToScene(-2, -2)
     << mapToScene(2, -2));

    foreach (QGraphicsItem *item, foundItems) { // Проверяем все элементы
     if (item == this) continue; // Сам себя паук не ест
     if (item == target) { // А если это не он сам - выслать сигнал об окончании игры
      emit signalCheckGameOver();
     }
    }

    // Если вылез за игровые координаты, подкорректировать
    if (this->x() - 10 < -250) {
     this->setX(-240); // слева
    }
    if(this->x() + 10 > 250){
     this->setX(240); // справа
    }
    if(this->y() - 10 < -250){
     this->setY(-240); // сверху
    }
    if(this->y() + 10 > 250){
     this->setY(240); // снизу
    }
}

void Spider::pause() { // Пауза, включить или выключить таймер
 if (timer->isActive()) {
  timer->stop();
 }
 else {
  timer->start(15);
 }
}
