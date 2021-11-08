#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <get.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <QMouseEvent>
#include "get.h"
#include "player.h"
#include "autosweeper.h"

//定义常量
#define BLOCK 42
#define ELENUM_LENGHT 40
#define TTILE_BEGIN 32
#define TITILE_HEIGH  (TTILE_BEGIN + 40)
#define ROW 16
#define COL 30
#define BOOMNUM 99
#define LENGTH COL * BLOCK
#define HEIGHT ROW * BLOCK + TITILE_HEIGH



namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Game* game;
    Player* player;
    AutoSweeper* solver;
    int click_x;
    int click_y;
    Ui::MainWindow *ui;
    int paint_flag;
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private slots:
    void on_actionQuit_triggered();
    void on_pushButton_AI_clicked();
    void on_pushButton_ALL_clicked();
    void on_pushButton_NEW_clicked();
    void on_pushButton_TEST_clicked();
    void on_pushButton_100_clicked();

    void sleep(int);

    void TestGame(int**);
};





#endif // MAINWINDOW_H
