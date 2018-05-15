#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    this->resize(600,600);          // Задаем размеры виджета
    this->setFixedSize(600,600);    // Фиксируем размеры
    this->setGeometry(QStyle::alignedRect( //Центрируем на экране
                          Qt::LeftToRight,
                          Qt::AlignCenter,
                          this->size(),
                          qApp->desktop()->availableGeometry()));
    scene = new QGraphicsScene();  // Инициализируем графическую сцену
scene->setBackgroundBrush( QImage("D:/3.jpg"));
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали
    scene->setSceneRect(-250,-250,500,500); //Игровые координаты от (-250,-250) до (250,250)

    timer = new QTimer();
    timerCreateApple = new QTimer();
    gameState = GAME_STOPED;
    pauseKey = new QShortcut(this);
    pauseKey->setKey(Qt::Key_Pause);
    connect(pauseKey, &QShortcut::activated, this, &Widget::slotPause);
}

Widget::~Widget() { delete ui; }

void Widget::slotDeleteApple(QGraphicsItem *item) {
 /* Получив сигнал от мухи, перебираем список яблок и удаляем найденное */
 foreach (QGraphicsItem *apple, apples) {
        if (apple == item) {
            scene->removeItem(apple);   // Удаляем со сцены
            apples.removeOne(apple);    // Удаляем из списка
            delete apple;               // Удаляем из памяти
            ui->lcdNumber->display(count++);    // Увеличиваем счёт на единицу и отображаем его
            // Звук поедания яблока
            QMediaPlayer * m_player = new QMediaPlayer(this);   // Инициализируем плейер
            QMediaPlaylist * m_playlist = new QMediaPlaylist(m_player); // Создаём плейлист
            m_player->setPlaylist(m_playlist);  // Устанавливаем плейлист в плейер
            m_playlist->addMedia(QUrl("qrc:/eating.wav")); // Добавляем аудио в плейер
            m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce); // Проигрываем один раз
            m_player->play();   // Запускаем плейер
        }
    }
}

void Widget::slotCreateApple() {
    if (apples.size() > MAX_APPLES) return; // Ограничение на количество яблок
    Apple *apple = new Apple(); // Создаём яблоко
    scene->addItem(apple);      // Помещаем его в сцену со случайной позицией
    apple->setPos((qrand() % (251)) * ((qrand()%2 == 1)?1:-1),
                  (qrand() % (251)) * ((qrand()%2 == 1)?1:-1)); //Так как игровые координаты от (-250,-250) до (250,250)
    apple->setZValue(-1); // Помещаем яблоко в слой ниже мухи
    if (apples.size() < MAX_APPLES) apples.append(apple); // Добавляем яблоко в список
}

void Widget::on_pushButton_clicked() { // Кнопка Старт
 if (gameState == GAME_STOPED) {
    count = 0;
    ui->lcdNumber->display(count); // Инициализируем счёт
    fly = new Fly(); // Создаём муху
    scene->addItem(fly); // Добавляем её на сцену
    fly->setPos(0,0); // Ставим в начло координат (центр)
    spider = new Spider(fly);  // Создаём паука
    scene->addItem(spider);
    spider->setPos(250,250); //Ставим в угол
    fly->m_player->play(); //Включем музыку

    connect(spider, &Spider::signalCheckGameOver, this, &Widget::slotGameOver); //Сигнал для паука
    connect(timer, &QTimer::timeout, fly, &Fly::slotGameTimer); //Соединяем таймер со слотом игры
    timer->start(1000 / 100); //Скорость таймера - 100 раз в секунду
    connect(timerCreateApple, &QTimer::timeout, this, &Widget::slotCreateApple); //Создание яблока
    timerCreateApple->start(1000); //Каждую секунду
    connect(fly, &Fly::signalCheckItem, this, &Widget::slotDeleteApple); //Сигнал от мухи для передачи объектов, на которые она наткнулась
    //ui->pushButton->setEnabled(false);
    gameState = GAME_STARTED;
    ui->pushButton->setText("Пауза");
 }
 else {
     slotPause();
 }
}

void Widget::slotGameOver() { //Конец игры
    {
    timer->stop(); timerCreateApple->stop(); //Отключить таймеры
    //Звук "Муха съедена"
    QMediaPlayer * m_player = new QMediaPlayer(this);       // Создаём плеер
    QMediaPlaylist * m_playlist = new QMediaPlaylist(m_player); // Создаём плейлист
    m_player->setPlaylist(m_playlist);      // Устанавливаем плейлист в плеер
    m_playlist->addMedia(QUrl("qrc:/oh.wav")); // Добавляем аудио в плейлист
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);   // Проигрываем трек один раз
    m_player->play();   // Запускаем трек

    ui->lcdNumber->display(count); // Показать на табло окончательный счёт
    QMessageBox::warning(this,
                         "Игра окончена",
                         "К сожалению, вашу муху съел паук. Счёт игры = "+QString::number(count));
    // Отключаем сигналы от слотов
    disconnect(timerCreateApple, &QTimer::timeout, this, &Widget::slotCreateApple);
    disconnect(fly, &Fly::signalCheckItem, this, &Widget::slotDeleteApple);
    disconnect(spider, &Spider::signalCheckGameOver, this, &Widget::slotGameOver);
    // Удаляем объекты со сцены
    spider->deleteLater();
    fly->deleteLater();
    foreach (QGraphicsItem *apple, apples) {
        scene->removeItem(apple);   // Удаляем со сцены
        apples.removeOne(apple);    // Удаляем из списка
        delete apple;               // Вообще удаляем
    }
   // ui->pushButton->setEnabled(true);
    ui->pushButton->setText("Старт");
    gameState = GAME_STOPED; // Устанавливаем состояние игры в GAME_STOPED
   }
}

void Widget::slotPause() { //Слот Пауза
 if (gameState == GAME_STARTED) { // Если игра активна
  if (timer->isActive()) { // И активен таймер
   timer->stop(); // Остановить таймеры
   timerCreateApple->stop();
   fly->m_player->stop();
   ui->pushButton->setText("Старт");
  }
  else { // Игра не активна
   timer->start(1000/100); // Запустить таймеры
   timerCreateApple->start(1000);
   fly->m_player->play();
   ui->pushButton->setText("Пауза");
  }
  spider->pause();
 }
}

