
#include <QPainter>
#include"get.h"
#include "mainwindow.h"
#include"drawmap.h"
#include "autosweeper.h"

// 画未知格子
void DrawUnknown(int i, int j, QPainter* painter)
{
    painter->setPen(QPen(QColor(158,158,158),1,Qt::SolidLine));//设置画笔形式
    painter->setBrush(QBrush(QColor(230,230,230),Qt::SolidPattern));//设置画刷形式
    painter->drawRect(j * BLOCK, i * BLOCK + TITILE_HEIGH, BLOCK, BLOCK);
}

// 画红旗标记格子
void DrawRedflag(int i, int j, QPainter* painter)
{
    painter->setPen(QPen(Qt::white,1,Qt::SolidLine));//设置画笔形式
    painter->setBrush(QBrush(Qt::red,Qt::SolidPattern));//设置画刷形式
    painter->drawRect(j * BLOCK, i * BLOCK + TITILE_HEIGH, BLOCK, BLOCK);
}

// 画数字格子
void DrawNum(int i, int j, int n, QPainter* painter)
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(15);
    painter->setFont(font);
    switch (n)
    {
        // 画笔是画线条和数字文字的
        case 0: DrawEmpty(i, j, painter); return;
        case 1: painter->setPen(QPen(QColor(8,8,255, 255),  1,Qt::SolidLine));break;
        case 2: painter->setPen(QPen(QColor(42,176,79, 255),1,Qt::SolidLine));break;
        case 3: painter->setPen(QPen(QColor(248,1,0, 255),  1,Qt::SolidLine));break;
        case 4: painter->setPen(QPen(QColor(0,254,242, 255),1,Qt::SolidLine));break;
        case 5: painter->setPen(QPen(QColor(253,0,254, 255),1,Qt::SolidLine));break;
        case 6: painter->setPen(QPen(QColor(253,255,1, 255),1,Qt::SolidLine));break;
        case 7: painter->setPen(QPen(QColor(255,98,0, 255), 1,Qt::SolidLine));break;
        case 8: painter->setPen(QPen(QColor(96,96,96, 255), 1,Qt::SolidLine));break;
    }
    // 画刷是画区域的
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));//设置画刷形式
    painter->drawRect(j * BLOCK, i * BLOCK + TITILE_HEIGH, BLOCK, BLOCK);
    // 设置字体的对齐方式和内容
    painter->drawText(j * BLOCK + 12, i * BLOCK + TITILE_HEIGH + 34, QString::number(n));

}

// 画空白格子
void DrawEmpty(int i, int j, QPainter* painter)
{
    painter->setPen(QPen(QColor(158,158,158),1,Qt::SolidLine));//设置画笔形式
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));//设置画刷形式
    painter->drawRect(j * BLOCK, i * BLOCK + TITILE_HEIGH, BLOCK, BLOCK);
}

// 游戏信息：雷格-2；空白0；数字1~8
// 玩家信息：标记-2；未知-1；空白0；数字1~8
void DrawMain(const vector<vector<int>>& board, QPainter *painter)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (board[i][j] == -2)
            {
                DrawRedflag(i, j, painter);
            }
            else if(board[i][j] == -1)
            {
                DrawUnknown(i, j, painter);
            }
            // 画数字和空白格
            else if (board[i][j] >= 0 && board[i][j] <= 8)
            {
                DrawNum(i, j, board[i][j], painter);
            }
            else if (board[i][j] == -999)
            {
                DrawKillBoom(i, j, painter);
            }
        }
    }

}

void DrawKillBoom(int i, int j, QPainter *painter)
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(18);
    painter->setFont(font);

    // 画笔是画线条和数字文字的
    painter->setPen(QPen(Qt::red,  1,Qt::SolidLine));

    // 画刷是画区域的
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));//设置画刷形式
    painter->drawRect(j * BLOCK, i * BLOCK + TITILE_HEIGH, BLOCK, BLOCK);
    // 设置字体的对齐方式和内容
    painter->drawText(j * BLOCK + 5, i * BLOCK + TITILE_HEIGH + 34, QString("炸"));
}

void DrawInit(QPainter *paintmywindow)
{
    //绘制雷区
    for(int i = 0;i < ROW; i++)
    {
         for(int j = 0; j < COL; j++)
         {
             DrawUnknown(i, j, paintmywindow);
         }
    }
}


