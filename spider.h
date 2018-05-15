#ifndef SPIDER_H
#define SPIDER_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>
#include <QDebug>

class Spider : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Spider(QGraphicsItem * target, QObject *parent = 0);
    ~Spider();
    void pause();   // Сигнал для паузы

signals:
    void signalCheckGameOver();  // Сигнал для вызова состояния "Игра окончена"

public slots:

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qreal angle;        // Угол поворота
    int steps;          // Номер положения ног паука
    int countForSteps;  // Счетчик для изменения положения ног
    QTimer      *timer;     // Внутренний таймер паука, по которому инициализируется его движение
    QGraphicsItem * target; // Цель движения паука, целью будет муха

private slots:
    void slotGameTimer();   // Слот игрового таймера паука
};

#endif // SPIDER_H
