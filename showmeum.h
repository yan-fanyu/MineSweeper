#ifndef SHOWMEUM_H
#define SHOWMEUM_H
#include<QTimer>
#include "mainwindow.h"
#include "get.h"

/*
 菜单栏要展示的东西都在这
*/


void ShowInit(Ui::MainWindow *);            // 展示初始化
void ShowLose(Ui::MainWindow *);            // 显示失败LOSE
void ShowWin(Ui::MainWindow *);            // 显示失败LOSE
void ShowWinRate(Ui::MainWindow *);         // 显示胜率
void ShowSet(Ui::MainWindow *);
void ShowBoomLeft(Ui::MainWindow *ui, Game* game, vector<vector<int>>&);


#endif // SHOWMEUM_H
