
#include"mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include"drawmap.h"
#include"showmeum.h"
#include "autosweeper.h"
#include"get.h"
#include "player.h"
#include "QTime"



int TestArr[ROW][COL];

static int gameNum = 1;
static int winNum = 0;

static bool TEST = false;
static bool ALL = false;
static bool FIRST = true;
static bool ON = false;
static bool PLAYER = false;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    //构建窗口大小
    ui->setupUi(this);
    this->setMinimumSize(LENGTH, HEIGHT);
    this->setMaximumSize(LENGTH, HEIGHT);

    game = new Game(ROW, COL, BOOMNUM);
    player = new Player();
    solver = new AutoSweeper(ROW, COL, BOOMNUM);
    ShowInit(ui);
    paint_flag = 0;
    //update();
}

MainWindow::~MainWindow()
{
    delete ui;   
    delete game;
    delete player;
    delete solver;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    click_x = event->x();
    click_y = event->y();
    if (click_y < TITILE_HEIGH)
        return;

    PLAYER = true;

    if(FIRST)
    {
        FIRST = false;
        ON = true;
    }

    if(ON == false && !FIRST)
    {
        return;
    }

    if(game->IsWin())
    {
        winNum++;
        ShowWin(ui);
        ON = false;
        ALL =false;
        ShowWinRate(ui);
        ShowSet(ui);
        return;
    }

    //确定落点
    click_x = click_x / BLOCK;
    click_y = (click_y - TITILE_HEIGH) / BLOCK;

    if (event->buttons() == (Qt::LeftButton))
    {
        // 左键点到未知的炸弹
        if (game->myMap[click_y][click_x] == -2 && player->board[click_y][click_x] == -1)
        {
            player->board[click_y][click_x] = -999;
            paint_flag = 4;
            ON = false;
            //update();
            return;
        }
        // 左键点到其他未知格 (未知数字，未知空白)
        else if (game->myMap[click_y][click_x] >= 0 && game->myMap[click_y][click_x] <= 8)
        {
            game->opens.insert(make_pair(click_y, click_x));
            player->board[click_y][click_x] = game->myMap[click_y][click_x];
            if(game->myMap[click_y][click_x] == 0)
            {
                game->Showblock(click_y, click_x, player->board);
            }
            paint_flag = 2;
            //update();
            return;
        }

    }
    else if (event->buttons() == Qt::RightButton)
    {
        // 点击插旗(雷，空白，数字)格
        if (player->board[click_y][click_x] == -2)
        {
            //变为未知(雷，空白，数字)格
            game->opens.erase(make_pair(click_y, click_x));
            player->board[click_y][click_x] = -1;
            //ShowBoomLeft(ui, game, player->board);
            paint_flag = 3;
            //update();
            return;
        }
        // 点击未知(雷，空白，数字)格
        else if (player->board[click_y][click_x] == -1)
        {
            //变为插旗(雷，空白，数字)格
            game->opens.insert(make_pair(click_y, click_x));
            player->board[click_y][click_x] = -2;
            //ShowBoomLeft(ui, game, player->board);
            paint_flag = 3;
            //update();
            return;
        }
    }
}

//绘图事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainter* paintmywindow = &painter;

    //如果是第一次打开，则进行初始化
    if (paint_flag == 0)
    {
        DrawInit(paintmywindow);
    }
    //左键点到炸弹绘制
    else if (paint_flag == 4)
    {
        //ShowLose(ui);
        ShowSet(ui);
        ALL = false;
        DrawMain(player->board, paintmywindow);
    }
    //左键点到空白绘制
    else if (paint_flag == 2 || paint_flag == 3 || paint_flag == 5)
    {
        DrawMain(player->board, paintmywindow);
    }
    if(ALL)
    {
        on_pushButton_AI_clicked();
    }
}

// 退出
void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

// 扫一下
void MainWindow::on_pushButton_AI_clicked()
{
    if(!ON && FIRST)
    {
        ON = true;
    }
    vector<pair<int, int>> pointsLeft;
    vector<pair<int, int>> pointsRight;
    solver->Main(player->board, pointsLeft, pointsRight);

    for (auto point : pointsLeft)
    {
        paint_flag =2;
        // opens 是 set容器可以去重
        game->opens.insert(point);
        // 玩家信息                          // 游戏信息 -1 未知；-2雷，0~8

        if(game->myMap[point.first][point.second] == -2)
        {
            paint_flag = 4;
            player->board[point.first][point.second] = -999;
            //update();
            ON = false;
            ShowSet(ui);
            break;
        }
        player->board[point.first][point.second] = game->myMap[point.first][point.second];
    }

    // 游戏更新
    game->UpdateBoard(player->board);
    //update();

    for (auto point : pointsRight)
    {
        paint_flag = 3;
        player->board[point.first][point.second] = -2;
        //ShowBoomLeft(ui, game, player->board);
        //update();
    }
    if(ON == false)
    {
        ShowLose(ui);
        ShowWinRate(ui);
        ui->pushButton_AI->setEnabled(false);
        ui->pushButton_ALL->setEnabled(false);
    }
    ShowBoomLeft(ui, game, player->board);
    if(game->IsWin())
    {
        winNum++;
        ShowWin(ui);
        ON = false;
        ALL =false;
        ShowWinRate(ui);
        ShowSet(ui);
    }

    if(ALL && ON)
    {
        on_pushButton_AI_clicked();
    }
    else
    {
        return;
    }
}

// ALL
void MainWindow::on_pushButton_ALL_clicked()
{
    ALL = true;
    ON = true;
    on_pushButton_AI_clicked();
}

// NEW
void MainWindow::on_pushButton_NEW_clicked()
{
    gameNum++;
    // 不加下面三行内存就爆炸了
    delete game;
    delete player;
    delete solver;
    game = new Game(ROW, COL, BOOMNUM);
    TestGame(game->p);
    player = new Player();
    solver = new AutoSweeper(ROW, COL, BOOMNUM);
    TEST = false;
    ALL = false;
    FIRST = true;
    ON = false;
    paint_flag = 0;
    game->opens.clear();
    this->setMinimumSize(LENGTH, HEIGHT);
    this->setMaximumSize(LENGTH, HEIGHT);
    ShowInit(ui);
    //update();
}

// TEST
void MainWindow::on_pushButton_TEST_clicked()
{
    on_pushButton_NEW_clicked();
    on_pushButton_ALL_clicked();
//    int second = 0;
//    second = rand() % 10 + 20;
//    sleep(second);
//    on_pushButton_TEST_clicked();
}

// 扫雷100局
void MainWindow::on_pushButton_100_clicked()
{
    for (int i = 0; i < 1000; i++)
    {
        on_pushButton_TEST_clicked();
    }
    ON = false;
    FIRST = false;

    //on_pushButton_TEST_clicked();
}

void MainWindow::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::TestGame(int** game)
{
    double min = gameNum;
    double max = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if(game[i][j] == -2)
            {
                TestArr[i][j]++;
            }
            if(TestArr[i][j] < min)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                min = TestArr[i][j];
            }
            if(TestArr[i][j] > max)
            {
                max = TestArr[i][j];
            }
        }
    }
    if(gameNum % 99 == 0)
    {
        qDebug("gameNum = %d, prob = %lf", gameNum, (max - min) / gameNum);
    }
}
