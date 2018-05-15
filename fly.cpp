#include "fly.h"

Fly::Fly(QObject *parent) :  QObject(parent), QGraphicsItem() {
    angle = 0;      // Угол поворота
    setRotation(angle);
    steps = 1;      // Стартовое положение ног
    countForSteps = 0;      // Счётчик для отсчета тиков таймера, при которых мы нажимали на кнопки
    m_player = new QMediaPlayer(this);          // Инициализация плейера
    m_playlist = new QMediaPlaylist(m_player);  // Инициализация плейлиста
    m_player->setPlaylist(m_playlist);          // Установка плейлиста в аудио плейер
    m_playlist->addMedia(QUrl("qrc:/background.wav"));       // Добавление трека в плейлист
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); // Зацикливание трека
}

Fly::~Fly() { }

QRectF Fly::boundingRect() const {
 return QRectF(-40,-50,80,100);   // Ограничиваем область, в которой располагается муха
}

void Fly::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option); Q_UNUSED(widget); //Пока не используем эти параметры
    painter->setPen(QPen(Qt::blue, 2)); // Перо
    //Отрисовка ног
    if(steps == 0) {     // Первое положение ног
        // Левая 1
        painter->drawLine(-24,-37,-22,-25);
        painter->drawLine(-22,-25,-17,-15);
        painter->drawLine(-17,-15,-10,-5);
        // Правая 1
        painter->drawLine(37,-28,28,-18);
        painter->drawLine(28,-18,24,-8);
        painter->drawLine(24,-8,10,-5);

        // Левая 2
        painter->drawLine(-35,-20,-25,-11);
        painter->drawLine(-25,-11,-14,-5);
        painter->drawLine(-14,-5,0,5);
        // Правая 2
        painter->drawLine(37,-12,32,-4);
        painter->drawLine(32,-4,24,2);
        painter->drawLine(24,2,0,5);

        // Левая 3
        painter->drawLine(-35,35,-26,24);
        painter->drawLine(-26,24,-16,16);
        painter->drawLine(-16,16,0,0);
        // Правая 3
        painter->drawLine(37,26,32,17);
        painter->drawLine(32,17,24,8);
        painter->drawLine(24,8,0,0);
    }
    else if (steps == 1){     // Второе положение ног
        // Левая 1
        painter->drawLine(-32,-32,-25,-22);
        painter->drawLine(-25,-22,-20,-12);
        painter->drawLine(-20,-12,-10,-5);
        // Правая 1
        painter->drawLine(32,-32,25,-22);
        painter->drawLine(25,-22,20,-12);
        painter->drawLine(20,-12,10,-5);

        // Левая 2
        painter->drawLine(-39,-15,-30,-8);
        painter->drawLine(-30,-8,-18,-2);
        painter->drawLine(-18,-2,0,5);
        // Правая 2
        painter->drawLine(39,-15,30,-8);
        painter->drawLine(30,-8,18,-2);
        painter->drawLine(18,-2,0,5);

        // Левая 3
        painter->drawLine(-39,30,-30,20);
        painter->drawLine(-30,20,-20,12);
        painter->drawLine(-20,12,0,0);
        // Правая 3
        painter->drawLine(39,30,30,20);
        painter->drawLine(30,20,20,12);
        painter->drawLine(20,12,0,0);
    }
    else if (steps == 2){     // Третье положение ног
        // Левая 1
        painter->drawLine(-37,-28,-28,-18);
        painter->drawLine(-28,-18,-24,-8);
        painter->drawLine(-24,-8,-10,-5);
        // Правая 1
        painter->drawLine(24,-37,22,-25);
        painter->drawLine(22,-25,17,-15);
        painter->drawLine(17,-15,10,-5);

        // Левая 2
        painter->drawLine(-37,-12,-32,-4);
        painter->drawLine(-32,-4,-24,2);
        painter->drawLine(-24,2,0,5);
        // Правая 2
        painter->drawLine(35,-20,25,-11);
        painter->drawLine(25,-11,14,-5);
        painter->drawLine(14,-5,0,5);

        // Левая 3
        painter->drawLine(-37,26,-32,17);
        painter->drawLine(-32,17,-24,8);
        painter->drawLine(-24,8,0,0);
        // Правая 3
        painter->drawLine(35,35,26,24);
        painter->drawLine(26,24,16,16);
        painter->drawLine(16,16,0,0);
    }
    // Усики
    QPainterPath path(QPointF(-5,-34));
    path.cubicTo(-5,-34, 0,-36,0,-30);
    path.cubicTo(0,-30, 0,-36,5,-34);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);

    painter->setPen(QPen(Qt::black, 1));
    // Тушка
    painter->setBrush(Qt::black);
    painter->drawEllipse(-15, -20, 30, 50);
    // Голова
    painter->drawEllipse(-15, -30, 30, 20);
    // Глаза
    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::green : Qt::red);
      // Если поблизости от мухи есть объект, глаза краснеют
    painter->drawEllipse(-15, -27, 12, 15);
    painter->drawEllipse(3, -27, 12, 15);
    // Левое крыло
    QPainterPath path2(QPointF(-10, -10));
    path2.cubicTo(-18, -10, -30, 10, -25, 35);
    path2.cubicTo(-25,35,-20,50,-15,40);
    path2.cubicTo(-15,40,0,20,-3,5 );
    path2.cubicTo(-3,5, -8,8,-10,-10);
    painter->setBrush(Qt::white);
    painter->drawPath(path2);
    // Правое крыло
    QPainterPath path3(QPointF(10, -10));
    path3.cubicTo(18, -10, 30, 10, 25, 35);
    path3.cubicTo(25,35,20,50,15,40);
    path3.cubicTo(15,40,0,20,3,5 );
    path3.cubicTo(3,5, 8,8,10,-10);
    painter->setBrush(Qt::white);
    painter->drawPath(path3);
}

void Fly::slotGameTimer() {
    // Состояние клавиш проверяем методом GetAsyncKeyState (асинхронное получение состояния клавиши) из Windows API
    if (GetAsyncKeyState(VK_LEFT) ||
       GetAsyncKeyState(VK_RIGHT) ||
       GetAsyncKeyState(VK_UP) ||
       GetAsyncKeyState(VK_DOWN)) { // Если нажата какая-то из клавиш управления
        if(GetAsyncKeyState(VK_LEFT)) {
            angle -= 5;        // Задаём поворот на 5 градусов влево
            setRotation(angle); // Поворачиваем объект
        }
        if(GetAsyncKeyState(VK_RIGHT)) {
            angle += 5;        // Задаём поворот на 5 градусов вправо
            setRotation(angle); // Поворачиваем объект
        }
        if(GetAsyncKeyState(VK_UP)) {
            setPos(mapToParent(0, -2)); // Продвигаем объект вперёд (используются координаты сцены)
        }
        if(GetAsyncKeyState(VK_DOWN)) {
            setPos(mapToParent(0, 2)); // Продвигаем объект назад (используются координаты сцены)
        }
        // Двигаем ноги
        countForSteps++;
        if(countForSteps == 6) { steps = 2; }
        else if (countForSteps == 12) { steps = 1; }
        else if (countForSteps == 18) { steps = 0; }
        else if (countForSteps == 24) { steps = 1; countForSteps = 0; }
        update(QRectF(-40,-50,80,100));
    }
    else {
     // Делаем что-нибудь, что нужно, если не нажаты клавиши управления
    }

    /* Чтобы определить, наткнулась ли муха на какой-нибудь
     * элемент на графической сцене, задаём небольшую область перед мухой,
     * в которой будем искать элементы */
    QList<QGraphicsItem *> foundItems = scene()->items(QPolygonF()
      << mapToScene(0, 0)
      << mapToScene(-20, -20)
      << mapToScene(20, -20));

    foreach (QGraphicsItem *item, foundItems) { // Проверяем все элементы
     if (item == this) continue; // Сама с собой муха не сталкивается
     emit signalCheckItem(item); // дл остальных объектов выслыаем сигнал
    }

    // Если вылезла за игровые координаты, подкорректировать
    if (this->x() - 10 < -250) {
     this->setX(-240); // слева
    }
    if (this->x() + 10 > 250) {
     this->setX(240); // справа
    }
    if (this->y() - 10 < -250) {
     this->setY(-240); // сверху
    }
    if (this->y() + 10 > 250) {
     this->setY(240); // снизу
    }
}
