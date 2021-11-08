#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#ifndef AutoSweeper_H
#define AutoSweeper_H

struct AutoSweeper
{
    bool ALL;
    bool TEST;
    int SUMLIMIT = 100000000;
    int DEEPDFSLIMIT = 1000;
    bool flag;
    int n, m, mine;
    // 地雷的概率数组，相当于二维数组
    vector<vector<double>> YangHuiTriangle;

    // 构造函数
    AutoSweeper (int, int, int);

    // 找出100%的安全点
    bool GetSafePoints(const vector<vector<int>>&, vector<pair<int, int>>&);

    // 找出100%的地雷
    bool GetBoomPoints(const vector<vector<int>>&, vector<pair<int, int>>&);

    // 枚举一个连通区域的所有解
    void GetAllSolution(vector<pair<int, int>>&, vector<vector<int>>&, int,int,
             vector<int>&, vector<int>&, int&, int, int,map<int, double>&, vector<map<int, double>>&,
             vector<vector<int>>&, bool);

    // 基于一个连通区域扫雷
    void SweepBasedOnConnectedComponent(
        vector<vector<int>>&, vector<pair<int, int>>&, vector<pair<int, int>>&, vector<pair<int, int>>&, vector<pair<int, int>>&,
        vector<vector<double>>&, int, int, map<int, double>&, vector<map<int, double>>&,
        vector<vector<int>>&, bool);

    // 获取一个连通区域
    void GetConnectedComponent(vector<vector<int>>&, set<pair<int, int>>&,
                               vector<pair<int, int>>&, vector<pair<int, int>>&, int, int);

    // 探索
    bool Explore(vector<vector<int>>&, vector<pair<int, int>>&, vector<pair<int, int>>&, vector<vector<double>>&);

    // 通过连通区域的各个解，寻找该连通区域的 安全格、雷格、不确定是雷格还是安全格的未知格的为雷概率
    void GetSafePointsByConnectedComponent(vector<vector<int>>& board, const vector<pair<int, int>>& unknownCells, const vector<int> presumeAsBoomCountList, vector<pair<int, int>> &pointsLeft,
                                 vector<pair<int, int>> &,vector<vector<double>> &prob, int unknowNum, int totalSolutionNum);

    // 通过全局来计算精确的概率
    void GetAccurateProb(const vector<vector<int>> &board, vector<vector<double>> &prob, int mineNum, int isolatedBoardNum,
                         const vector<vector<pair<int, int>>>& safeCellsList, const vector<vector<pair<int, int>>>& unkownCellsList, const vector<map<int, double>>& solutionNumOfpresumeBoomNumList ,
                         const vector<vector<map<int, double>>>& perBoardCntList);

    // 获取猜局的为雷概率最小的一个未知格的坐标
    pair<int, int> GetMinBoomProbBlock(const vector<vector<int>>& board, vector<vector<double>>& prob);

    // 获取一个连通区域的未知格与数字格的相邻关系
    void GetConnectedRelation(const vector<pair<int, int>> &safeCells, const vector<pair<int, int>> &unknownCells, vector<vector<int>>& connected);

    // 获取一个连通区域的数字格周围可再标记的雷数和未知格数
    void GetMarkable_unknownNumList(const vector<vector<int>>& board, const vector<pair<int, int>> &safeCells,
                                    const set<pair<int, int>>& unknownSet, vector<pair<int, int>>& markable_unknownNumList);

    bool IsIsolated(const vector<vector<int>>&, int i, int j);

    pair<int, int> GetAngle(const vector<vector<int>>& board, pair<int, int>&);

    // 主入口
    bool Main(vector<vector<int>>, vector<pair<int, int>>&, vector<pair<int, int>>&);

    pair<vector<pair<int, int>>, vector<pair<int, int>>> CheckTwoUncoveredCell(const vector<vector<int>>& board, int x1, int y1, int x2, int y2) ;

    bool IsPointInRange(int x, int y);

    bool GetPointByTwoConnected(const vector<vector<int>>& board, vector<pair<int, int>> &pointsLeft, vector<pair<int, int>> &pointsRight);

    double CalculateAvgNumOfSafeCells(vector<vector<int>> &, int, int, int** ccGraph);

    vector<pair<int, int>> GetAround(int x, int y);



};

#endif // QQ_H

