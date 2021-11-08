#include"mainwindow.h"
#include"mainwindow.cpp"
#include"showmeum.h"

void ShowInit(Ui::MainWindow * ui)
{
    ui->pushButton_AI->setEnabled(true);
    ui->pushButton_ALL->setEnabled(true);
    ui->textEdit->setText("99");
    ui->textEdit_9->setText("");        // WIN or LOSE
    ui->textEdit_9->setEnabled(false);
    ui->textEdit_10->setEnabled(false);
    ui->textEdit_11->setText(QString::number(winNum));        // 胜局数
    ui->textEdit_11->setEnabled(false);
    ui->textEdit_12->setText(QString::number(gameNum));        // 总局数
    ui->textEdit_12->setEnabled(false);
    if(gameNum == 1)
    {
        ui->textEdit_9->setText("胜负");
        ui->textEdit_10->setText("胜率");
        ui->textEdit_11->setText("胜局");
        ui->textEdit_12->setText("局数");
    }
}

void ShowLose(Ui::MainWindow * ui)
{
    ui->textEdit_9->setText("LOSE");
}

void ShowWinRate(Ui::MainWindow *ui)
{
    ui->textEdit_10->setText(QString::number(winNum/double(gameNum),'f', 4));
    ui->textEdit_11->setText(QString::number(winNum));
    ui->textEdit_12->setText(QString::number(gameNum));
}

void ShowWin(Ui::MainWindow * ui)
{
    ui->textEdit_9->setText("WIN");
}

void ShowSet(Ui::MainWindow *ui)
{
    ui->pushButton_AI->setEnabled(false);
    ui->pushButton_ALL->setEnabled(false);
}

void ShowBoomLeft(Ui::MainWindow *ui, Game* game, vector<vector<int>>& board)
{
    ui->textEdit->setText(QString::number(game->GetBoomLeft(board)));
}
