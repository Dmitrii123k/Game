#ifndef APPLE_H
#define APPLE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

class Apple : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Apple(QObject *parent = 0);
    ~Apple();

signals:

public slots:

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int color; //Цвет яблока
};

#endif // APPLE_H
