QT       += core gui multimedia widgets
TARGET = Game
TEMPLATE = app
SOURCES += main.cpp\
        widget.cpp \
    fly.cpp \
    apple.cpp \
    spider.cpp
HEADERS  += widget.h \
    fly.h \
    apple.h \
    spider.h
FORMS    += widget.ui
RESOURCES += \
    res/game.qrc
