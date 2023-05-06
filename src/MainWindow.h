#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startGame();

private:
    Ui::MainWindow *ui;
    QLabel *scoreLabel;
    int score = 0;

private slots:
    void updateScoreLabel(int newScore);
    void gameOver(bool victory);
};
#endif // MAINWINDOW_H
