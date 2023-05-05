QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    logic/game.cpp \
    logic/maze.cpp \
    logic/path.cpp \

HEADERS += \
    MainWindow.h \
    logic/game.h \
    logic/maze.h \
    logic/path.h \

FORMS += \
    MainWindow.ui

TARGET = pacman

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Fixes missing path on macos
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

RESOURCES += \
    resources.qrc
