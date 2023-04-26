#include "pacman.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacMan w;
    w.show();
    return a.exec();
}
