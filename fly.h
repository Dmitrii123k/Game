#ifndef FLY_H
#define FLY_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <windows.h>
/* Подключаем библиотеку, отвечающую за использование WinAPI
 * Она здесь нужна для асинхронной проверки состояния клавиш
*/

class Fly : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    explicit Fly(QObject *parent = 0);
    ~Fly();
    QMediaPlayer * m_player;        // Аудиоплейер
    QMediaPlaylist * m_playlist;    // Плейлист

signals:
    void signalCheckItem (QGraphicsItem *item); //Сигнал, который передаётся, если муха наткнулась на объект
    void signalSoundStart();
    void signalSoundStop();

public slots:
    void slotGameTimer(); // Слот, который отвечает за обработку перемещения

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qreal angle;        // Угол поворота
    int steps;          // Номер положения ног
    int countForSteps;  // Счётчик для отсчета тиков таймера, при которых мы нажимали на кнопки

};

#endif // FLY_H
