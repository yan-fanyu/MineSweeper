#include<QPainter>
#include"get.h"
#include"vector"
using namespace std;

void DrawUnknown(int, int, QPainter*);// 画未知格
void DrawEmpty(int, int, QPainter*);// 画空白格
void DrawRedflag(int, int, QPainter*);// 画标记格
void DrawNum(int, int, int, QPainter*);     // 画数字格
void DrawMain(const vector<vector<int>>&, QPainter *painter);  //绘制未点击到雷 鼠标事件与结束的雷区
void DrawKillBoom(int, int, QPainter*);     // 画点击到炸弹而失败的那一格
void DrawInit(QPainter*);           // 全画未知格


