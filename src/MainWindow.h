/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file MainWindow.h
 * @brief Header file for the main window.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QLabel>
#include <QString>

/**
 * @namespace Ui
 * @brief Namespace for the MainWindow class.
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Represents the main window of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow class.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destructor for the MainWindow class.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Starts a new game.
     */
    void startGame();
    /**
     * @brief Loads a maze file.
     */
    void loadFile();
    /**
     * @brief Loads a replay file.
     */
    void loadReplay();
    /**
     * @brief Loads the menu.
     */
    void loadMenu();
    /**
     * @brief Runs the replay.
     */
    void replayGame(bool start);

private:
    Ui::MainWindow *ui; /**< The MainWindow object*/
    QLabel *scoreLabel; /**< The label displaying the score*/
    QLabel *keysLabel;  /**< The label displaying the number of keys*/
    int score = 0;      /**< The current score*/
    QString fileName;   /**< The name of the maze file*/
    QString replayFile; /**< The name of the replay file*/

private slots:
    /**
     * @brief Updates the score label.
     * @param newScore The new score.
     */
    void updateScoreLabel(int newScore);
    /**
     * @brief Updates the keys label.
     * @param keys The number of keys.
     */
    void updateKeysLabel(int keys);
    /**
     * @brief Displays the game over screen.
     * @param victory A boolean indicating whether the player won or lost.
     */
    void gameOver(bool victory);
};
#endif // MAINWINDOW_H
