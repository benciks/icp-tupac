#ifndef PACMAN_H
#define PACMAN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PacMan; }
QT_END_NAMESPACE

class PacMan : public QMainWindow
{
    Q_OBJECT

public:
    PacMan(QWidget *parent = nullptr);
    ~PacMan();

private:
    Ui::PacMan *ui;
};
#endif // PACMAN_H
