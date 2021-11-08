#include "autosweeper.h"
#include"mainwindow.h"
#include "get.h"

///
/// \brief AutoSweeper::AutoSweeper
/// \param n
/// \param m
/// \param mine
///
AutoSweeper::AutoSweeper(int n, int m, int mine): n(ROW), m(COL), mine(BOOMNUM)
{
    YangHuiTriangle = vector<vector<double>>(n * m + 1, vector<double>(mine + 2, 0));
    YangHuiTriangle[0][0] = 1.0;
    // 杨辉三角 计算 2^n
    for (int i = 0; i < n * m; i++)
    {
        for (int j = 0; j <= mine; j++)
        {
            YangHuiTriangle[i + 1][j] += YangHuiTriangle[i][j];
            YangHuiTriangle[i + 1][j + 1] += YangHuiTriangle[i][j];
        }
    }
}

///
/// \brief AutoSweeper::GetSafePoints
/// \param board
/// \param points
/// \return
///
bool AutoSweeper::GetSafePoints(const vector<vector<int>>& board, vector<pair<int, int>>& pointsLeft)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {	 // 数字格
            if (board[i][j] >= 0)
            {
                int s = 0;
                for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
                {
                    for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
                    {
                        // 周围标记的雷数
                        s += (board[ii][jj] == -2);
                    }
                }
                // 如果 数字格数字==周围已经标记的雷数
                if (s == board[i][j])
                {
                    for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
                    {
                        for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
                        {
                            // 把安全的未知格存起来
                            if (board[ii][jj] == -1)
                            {
                                pointsLeft.push_back(make_pair(ii, jj));
                            }
                        }
                    }
                }
            }
        }
    }
    return pointsLeft.size() > 0;
}

///
/// \brief AutoSweeper::GetBoomPoints
/// \param board
/// \param points
/// \return
///
bool AutoSweeper::GetBoomPoints(const vector<vector<int>>& board, vector<pair<int, int>>& pointsRight)
{
    // 遍历棋盘数组
    pointsRight.clear();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果是数字
            if (board[i][j] > 0)
            {
                int s = 0;
                // 遍历此数字格周围的 8 个格子  max 和 min 保证了边界问题
                for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
                {
                    for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
                    {
                        // 如果是未知格 或 已知标记的地雷
                        s += (board[ii][jj] < 0);
                    }
                }
                // 如果是未知格 + 已知标记的地雷 = 数字
                if (s == board[i][j])
                {
                    for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
                    {
                        for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
                        {
                            // 则未知格-1
                            if((board[ii][jj] == -1))
                            {
                                pointsRight.push_back(make_pair(ii, jj));
                            }
                        }
                    }
                }
            }
        }
    }
    return pointsRight.size()>0;
}

///
/// \brief AutoSweeper::GetAllSolution
/// \param markable_unknownNumList
/// \param connected
/// \param unknowNum
/// \param presumeNum
/// \param presumeAsBoomCountList
/// \param presumeStatusList
/// \param totalSolutionNum
/// \param mineMin
/// \param mineMax
/// \param solutionNumOfpresumeBoomNum
/// \param perBoardCnt
/// \param allSolution
/// \param saveAllSolution
///
void AutoSweeper::GetAllSolution(vector<pair<int, int>>& markable_unknownNumList, vector<vector<int>> &connected, int unknowNum,
         int presumeNum, vector<int>& presumeAsBoomCountList, vector<int>& presumeStatusList, int& totalSolutionNum, int mineMin, int mineMax,
         map<int, double>& solutionNumOfpresumeBoomNum, vector<map<int, double>>& perBoardCnt,
         vector<vector<int>>& allSolution, bool saveAllSolution)
{
    // 如果情况总数sum > 1亿
    // 总数太多，算力更不上了
    if (totalSolutionNum > SUMLIMIT)
    {
        flag = false;
        return;
    }
    // 如果最大地雷数<0 || 最小地雷数 > 连通分区中剩余的还未进行假设的未知格子数
    if (mineMax < 0 || mineMin > (unknowNum - presumeNum))
    {
        return;
    }
    // 遍历连通区域一圈的数字格
    for (int j = 0; j < markable_unknownNumList.size(); j++)
    {	// 如果此数字格可标记的雷数<0 || 此数字格周围的未知格 < 可标记的地雷数
        if (markable_unknownNumList[j].first < 0 || markable_unknownNumList[j].second < markable_unknownNumList[j].first)
        {
            return;
        }
    }

    //  如果此连通区域假设的个数==未知的格数，即完成了一次连通区域假设枚举，将更新
    // sum 和 presumeAsBoomCountList[] 和 mineCur
    // 即计算需要累计的总枚举数量 和 所有解的求和情况 和 本次解判定出来的地雷的个数
    if (presumeNum == unknowNum)
    {
        // 情况总数 sum++
        totalSolutionNum++;
        // 此次枚举的地雷数
        int presumeBoomNum = 0;
        // 遍历未知格，将
        for (int i = 0; i < presumeAsBoomCountList.size(); i++)
        {
            // presumeStatusList[i] = 0表示第i个未知格被假设为安全格，presumeStatusList[i] = 1第i个未知格被假设为雷格
            presumeAsBoomCountList[i] += presumeStatusList[i];
            presumeBoomNum += presumeStatusList[i];
        }
        // map<int, double> mineCnt 地雷数为presumeBoomNum的总情况个数
        solutionNumOfpresumeBoomNum[presumeBoomNum]++;
        // 遍历连通分区未知格格数次
        for (int i = 0; i < presumeAsBoomCountList.size(); i++)
        {
            // 如果第i个未知格在此次假设有presumeBoomNum个雷的解中被判定为雷，则perBoardCnt[i][presumeBoomNum]++;
            if (presumeStatusList[i])
            {
                // vector<map<int, double>> perBoardCnt
                // 第 i 个未知格，在连通分区被判定为有presumeBoomNum个雷时，被假设为雷的情况个数
                perBoardCnt[i][presumeBoomNum]++;
            }
        }
        return;
    }


    // **1、假设第i个边缘未知格设为安全格
    presumeStatusList[presumeNum] = 0;
    //遍历与第i个未知格相邻的数字格
    for (int j = 0; j < connected[presumeNum].size(); j++)
    {
        // 与第此未知格相邻的数字格周围的未知格数--
        markable_unknownNumList[connected[presumeNum][j]].second--;
    }

    // 注意：unknowNum, presumeNum + 1 是值传递，是不会更改的
    GetAllSolution(markable_unknownNumList, connected, unknowNum, presumeNum + 1, presumeAsBoomCountList, presumeStatusList, totalSolutionNum, mineMin, mineMax, solutionNumOfpresumeBoomNum, perBoardCnt, allSolution, saveAllSolution);


    // **2、假设第i个边缘未知格设为雷格
    presumeStatusList[presumeNum] = 1;
    // 遍历与第i个未知格相邻的数字格
    for (int j = 0; j < connected[presumeNum].size(); j++)
    {
        // 可标记的雷数--
        markable_unknownNumList[connected[presumeNum][j]].first--;
    }
    GetAllSolution(markable_unknownNumList, connected, unknowNum, presumeNum + 1, presumeAsBoomCountList, presumeStatusList, totalSolutionNum, mineMin - 1, mineMax - 1, solutionNumOfpresumeBoomNum, perBoardCnt, allSolution, saveAllSolution);

    // **3、撤销操作，即将假设为雷格第presumeNum个边缘未知格再变为假设未知格
    presumeStatusList[presumeNum] = 0;
    // 遍历与第i个未知格相邻的数字格
    for (int j = 0; j < connected[presumeNum].size(); j++)
    {
        // 此数字格可再标记的地雷数++
        markable_unknownNumList[connected[presumeNum][j]].first++;
        // 此数字格周围的未知格数++
        markable_unknownNumList[connected[presumeNum][j]].second++;
    }
}


///
/// \brief AutoSweeper::sweepBasedOnConnectedComponent
/// \param board
/// \param pointsLeft
/// \param safeCells
/// \param unknownCells
/// \param prob
/// \param mineMin
/// \param mineMax
/// \param solutionNumOfpresumeBoomNum
/// \param perBoardCnt
/// \param allSolution
/// \param saveAllSolution
///
void AutoSweeper::SweepBasedOnConnectedComponent(
    vector<vector<int>>& board, vector<pair<int, int>> &pointsLeft, vector<pair<int, int>> &pointsRight,
    vector<pair<int, int>> &safeCells, vector<pair<int, int>> &unknownCells,
    vector<vector<double>> &prob, int mineMin, int mineMax,
    map<int, double> &solutionNumOfpresumeBoomNum, vector<map<int, double>> &perBoardCnt,
    vector<vector<int>> &allSolution, bool saveAllSolution)
{
    // set容器的数据结构性质：单一，顺序
    set<pair<int, int>> unknownSet;
    // 遍历连通区域未知格集合
    for (int i = 0; i < (int)unknownCells.size(); i++)
    {
        // 将未知点顺序拷贝到 unknownSet,将unknownCells去重并排序
        unknownSet.insert(unknownCells[i]);
    }

    // 存入安全格pair<数字格周围可再标记的雷的数目, 数字格周围未知格的数目>
    vector<pair<int, int>> markable_unknownNumList;
    // 边界格，存入相邻的第i个未知格和第j个已知格
    vector<vector<int>> connected;

    // 遍历一份安全格的集合，即连通分区的一圈数字格，
    // 对markableNumList进行更新，即更新第i个数字格周围可再标记的地雷数和未知格数
    GetMarkable_unknownNumList(board, safeCells, unknownSet, markable_unknownNumList);

    GetConnectedRelation(safeCells, unknownCells, connected);

    // 连通区域的外圈未知格数量
    int unknowNum = unknownCells.size();
    // 连通区域的总解数
    int totalSolutionNum = 0;
    // 某个未知格在所有的解中被判定为雷的次数
    vector<int> presumeAsBoomCountList(unknowNum, 0);
    // 初始化一维数组，空间大小为未知格数目unknowNum，初始值为0；
    // 保存的是某个未知格被假设的情况，cur = 1 意义为将此未知格假设雷格，cur = 0 意义为将此未知格假设安全格
    vector<int> cur(unknowNum, 0);
    solutionNumOfpresumeBoomNum.clear();
    perBoardCnt.clear();
    perBoardCnt.resize(unknowNum);

    // *****进入GetAllSolution开始对连通区域进行枚举假设*****
    GetAllSolution(markable_unknownNumList, connected, unknowNum, 0, presumeAsBoomCountList, cur, totalSolutionNum, mineMin, mineMax, solutionNumOfpresumeBoomNum, perBoardCnt, allSolution, saveAllSolution);

    // 连通区域解出所有的情况分布，执行下面的代码
    GetSafePointsByConnectedComponent(board, unknownCells, presumeAsBoomCountList, pointsLeft, pointsRight, prob, unknowNum, totalSolutionNum);


    // 只有基于胜率计算时，才会执行这个if语句
    if (true)
    {
        // 遍历连通区域所有解，更新各个解的赋值
        for (int i = 0; i < allSolution.size(); i++)
        {
            vector <int> &curStatus = allSolution[i];
            // 这个嵌套的for循环功能：将假设为安全格的状态更新，例如周围有board[i][j]为雷和假设的雷，相加的和为更新后的状态值
            // 遍历一个解的各个未知格子的假设情况
            for (int j = 0; j < curStatus.size(); j++)
            {
                // 如果目前解的的第j个格子为安全格
                if (curStatus[j] == 0)
                {
                    // 如果目前解的的第j个格子为安全格,遍历此安全格周围的8个格子
                    for (int ii = max(0, unknownCells[j].first - 1); ii < min(n, unknownCells[j].first + 2); ii++)
                    {
                        for (int jj = max(0, unknownCells[j].second - 1); jj < min(m, unknownCells[j].second + 2); jj++)
                        {
                            // curStatus[j], cur[j] = 周围标记雷的个数
                            curStatus[j] += (board[ii][jj] == -2);
                        }
                    }
                    // 再次遍历一个解的各个格子
                    for (int k = 0; k < curStatus.size(); k++)
                    {
                        // curStatus[k] == -2 说明第k个未知格假设成了雷格
                        curStatus[j] += (curStatus[k] == -2 &&
                                         abs(unknownCells[j].first - unknownCells[k].first) <= 1 &&
                                         abs(unknownCells[j].second - unknownCells[k].second) <= 1);
                    }
                }
            }
        }
    }
}

///
/// \brief AutoSweeper::getMarkable_unknownNumList
/// \param board
/// \param safeCells
/// \param unknownSet
/// \param markable_unknownNumList
///
void AutoSweeper::GetMarkable_unknownNumList(const vector<vector<int>>& board,const vector<pair<int, int> >&safeCells,
     const set<pair<int, int>>& unknownSet, vector<pair<int, int>>& markable_unknownNumList)
{
    for (int i = 0; i < safeCells.size(); i++)
    {
        // 第 i 个已知点safeCells[i]
        pair<int, int> it = safeCells[i];
        // 将 pair<可标记的雷数, 周边未知格数> 存入markableNumList
        // 将所有的安全格坐标存入cnt + (0, 0)
        markable_unknownNumList.push_back(make_pair(board[it.first][it.second], 0));
        // 遍历第 i 个安全格周围的8个点
        for (int ii = max(0, it.first - 1); ii < min(n, it.first + 2); ii++)
        {
            for (int jj = max(0, it.second - 1); jj < min(m, it.second + 2); jj++)
            {
                // board[ii][jj]如果是标记的地雷格，则可再标记的雷数--
                if(board[ii][jj] == -2)
                {
                    markable_unknownNumList[i].first--;
                }
                // 如果此周围格未知
                else if (board[ii][jj] == -1)
                {
                    // 如果此数字的周围格是未知格且在unknownSet里面, 则此数字格周围的未知格数量++
                    if (unknownSet.find(make_pair(ii, jj)) != unknownSet.end())
                    {
                        markable_unknownNumList[i].second++;
                    }
                }
            }
        }
    }
}

bool AutoSweeper::IsIsolated(const vector<vector<int>>& board, int i, int j)
{
    for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
    {
        for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
        {
            // board[ii][jj]如果是标记的地雷格，则可再标记的雷数--
            if(board[ii][jj] != -1 || (board[ii][jj] > 0 && board[ii][jj] <= 8))
            {
                return false;
            }
        }
    }
    return true;
}

pair<int, int> AutoSweeper::GetAngle(const vector<vector<int> > &board, pair<int, int>& orgin)
{
    if(IsIsolated(board, 0, 0))
    {
        return make_pair(0, 0);
    }
    if(IsIsolated(board, ROW - 1, 0))
    {
        return make_pair(ROW - 1, 0);
    }
    if(IsIsolated(board, 0, COL - 1))
    {
        return make_pair(0, COL - 1);
    }
    if(IsIsolated(board, ROW - 1,  COL - 1))
    {
        return make_pair(ROW - 1,  COL - 1);
    }

    int j = 0;
    int i = 0;
    for(i = 0; i < ROW; i++)
    {
        if(IsIsolated(board, i, j))
        {
            return make_pair(i, j);
        }
    }

    j = COL - 1;
    for(i = 0; i < ROW; i++)
    {
        if(IsIsolated(board, i, j))
        {
            return make_pair(i, j);
        }
    }

    i = 0;
    for(j = 0; j < COL; j++)
    {
        if(IsIsolated(board, i, j))
        {
            return make_pair(i, j);
        }
    }

    i = ROW - 1;
    for(j = 0; j < COL; j++)
    {
        if(IsIsolated(board, i, j))
        {
            return make_pair(i, j);
        }
    }

    for(int i = 1; i < ROW -1; i++)
    {
        for(int j = 1; j < COL - 1; j++)
        {

        }
    }
}

///
/// \brief AutoSweeper::getSafePointsByConnectedComponent
/// \param board
/// \param unknownCells
/// \param presumeAsBoomCountList
/// \param pointsLeft
/// \param prob
/// \param unknowNum
/// \param totalSolutionNum
///
void AutoSweeper::GetSafePointsByConnectedComponent(vector<vector<int>>& board, const vector<pair<int, int>>& unknownCells, const vector<int> presumeAsBoomCountList, vector<pair<int, int>> &pointsLeft, vector<pair<int, int>> &pointsRight, vector<vector<double>> &prob, int unknowNum, int totalSolutionNum)
{
    // 连通区域解出所有的情况分布，执行下面的代码
    for (int i = 0; i < unknowNum; i++)
    {
        // 如果某未知格在该连通区域的每一个解中均未被判定为雷，则就100%是安全的
        if (presumeAsBoomCountList[i] == 0)
        {
            // 存入未知数组
            pointsLeft.push_back(unknownCells[i]);
        }
        // 如果某未知格在该连通区域的每一个解中均被判定为雷，则就100%是雷
        else if (presumeAsBoomCountList[i] == totalSolutionNum)
        {
            // 变为已知地雷, 就不存入pointsRight了，在这直接更数据吧~
            pointsRight.push_back(make_pair(unknownCells[i].first, unknownCells[i].second));
        }
        else
        {
            // 通过连通区域无法确定的部分，第i个未知格为雷的概率prob[i][j]为s[i] / sum, 后面会用到
            prob[unknownCells[i].first][unknownCells[i].second] = (double)presumeAsBoomCountList[i] / totalSolutionNum;
        }
    }
}

///
/// \brief AutoSweeper::getConnectedComponent
/// \param board
/// \param use
/// \param safeCells
/// \param unkownCells
/// \param i
/// \param j
///
void AutoSweeper::GetConnectedComponent(vector<vector<int>> &board, set<pair<int, int>> &use, vector<pair<int, int>> &safeCells, vector<pair<int, int>> &unkownCells, int i, int j)
{
    //  如果ij格不是已知雷(未知，空白，数字) && ij格是use的最后一个,把传入的ij格 存到 use 里面
    use.insert(make_pair(i, j));
    // 把未知的格子存起来
    if (board[i][j] == -1 )
    {
        unkownCells.push_back(make_pair(i, j));
    }
    // 把数字格和空白格的格子存起来
    else
    {
        safeCells.push_back(make_pair(i, j));
    }
    // 遍历(i, j)点周围的8个点
    for (int ii = max(0, i - 1); ii < min(n, i + 2); ii++)
    {
        for (int jj = max(0, j - 1); jj < min(m, j + 2); jj++)
        {
            // && 此周围格(未知，空白，数字)
            // && 此周围格不在use里面
            // && 此周围格 和 此格 是未知或数字(未知-1，数字1~8)
            if (board[ii][jj] != -2
                && use.find(make_pair(ii, jj)) == use.end()
                && (board[ii][jj] * board[i][j] < 0))
            {
                // 递归了
                GetConnectedComponent(board, use, safeCells, unkownCells, ii, jj);
            }
        }
    }
}




///
/// \brief AutoSweeper::Explore
/// \param board
/// \param pointsLeft
/// \param prob
/// \return
///
bool AutoSweeper::Explore(vector<vector<int>> &board, vector<pair<int, int>> &pointsLeft, vector<pair<int, int>> &pointsRight, vector<vector<double>> &prob)
{
    // 未知格数
    int unkownNum = 0;
    // 未知雷数
    int mineNum = mine;
    // 计算未知个数和未知雷数
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            unkownNum += (board[i][j] == -1);	  // 未知的格数++
            mineNum -= (board[i][j] == -2);	  // 未知的地雷--
        }
    }


    // 安全联通区集合
    vector<vector<pair<int, int>>> safeCellsList;
    // 未知联通区集合
    vector<vector<pair<int, int>>> unkownCellsList;
    //地雷数为mineCur的连通分区总情况个数 map<连通区判定出来的雷数，连通区解的个数>
    map<int, double> solutionNumOfpresumeBoomNum;
    vector<map<int, double>> solutionNumOfpresumeBoomNumList;
    //第i个未知格在连通区域被假设为有n个地雷时，本未知格是雷的情况个数 vector<<连通区判定出来的雷数，连通区解的个数>>; vector<i>表示第几个未知格
    vector<map<int, double>> perBoardCnt;
    vector<vector<map<int, double>>> perBoardCntList;
    // 全部的解情况(未初始化)
    vector<vector<int>> allStatus;
    //将探索过的非标记格顺序去重存入use
    set<pair<int, int>> use;

    // 遍历棋盘,通过一个格子以扩散的方式寻找连通区域，如果找到安全的格子，返回Main()
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果ij格不是已知雷(未知，空白，数字) && ij格不在use里面，即(i, j)格没有被探索过
            if (board[i][j] != -2 && use.find(make_pair(i, j)) == use.end())
            {
                // 安全格（数字1-8，空白0，已知雷-2）存起来
                vector<pair<int, int>> safeCells;
                // 未知格-1 存起来
                vector<pair<int, int>> unkownCells;

                // 递归将找出一个连通区域
                GetConnectedComponent(board, use, safeCells, unkownCells, i, j);

                // 如果安全格容器不为空 && 未知格容器不为空，即排除了全空白的格子
                if (safeCells.size() >= 1 && unkownCells.size() >= 1)
                {
                    // 将连通分区传入进行枚举
                    SweepBasedOnConnectedComponent(board, pointsLeft, pointsRight, safeCells, unkownCells, prob, 0, mineNum, solutionNumOfpresumeBoomNum, perBoardCnt, allStatus, false);
                    // 如果找到了安全的格子，就返回到Main函数，否则，将这个联通区存入连通区数组
                    if (pointsLeft.size() > 0)
                    {
                        return true;
                    }
                    // 将一个连通分区存入连通分区集合
                    safeCellsList.push_back(safeCells);
                    unkownCellsList.push_back(unkownCells);
                    solutionNumOfpresumeBoomNumList.push_back(solutionNumOfpresumeBoomNum);
                    perBoardCntList.push_back(perBoardCnt);
                }
            }
        }
    }

    // 没有通过全部的连通分区找到安全的格子，则执行下面的代码
    // 非连通区域的未知格的格子数
    int isolatedBoardNum = 0;
    // 计算非连通区域的未知格子数
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if(prob[i][j] < 0)
            {
                isolatedBoardNum++;
            }
        }
    }

    // 通过全局再精细一点计算每个连通区域的各个未知格的为雷概率；遍历各个连通分区的内圈安全格，目的：计算各个连通分区各个未知格的为雷概率
    GetAccurateProb(board, prob ,mineNum, isolatedBoardNum,safeCellsList, unkownCellsList, solutionNumOfpresumeBoomNumList, perBoardCntList);

    return false;
}


///
/// \brief AutoSweeper::getAccurateProb
/// \param board
/// \param pointsLeft
/// \param prob
/// \param mineNum
/// \param isolatedBoardNum
/// \param safeCellsList
/// \param unkownCellsList
/// \param solutionNumOfpresumeBoomNumList
/// \param perBoardCntList
///
void AutoSweeper::GetAccurateProb(const vector<vector<int>> &board, vector<vector<double>> &prob, int mineNum, int isolatedBoardNum,
     const vector<vector<pair<int, int>>>& safeCellsList, const vector<vector<pair<int, int>>>& unkownCellsList, const vector<map<int, double>>& solutionNumOfpresumeBoomNumList ,
     const vector<vector<map<int, double>>>& perBoardCntList)
{
    /***************************  核心代码  *************************************/
    for (int i = 0; i < int(safeCellsList.size()); i++)
    {
        //当前连通区域为主时，f[boomNum] 表示有boonNum个雷时的总的可能个数
        // 表示该连通区域以外的未知格子数为N时，排除该连通区域的区域的总可能情况数
        vector<double> solutionOfOtherConnectedComponent = YangHuiTriangle[isolatedBoardNum];
        // 遍历各个连通分区
        // 嵌套for循环计算，只考虑本连通区域的解时，其他连通区域自由组合 * 本连通区域的解
        // f[n] 表示只考虑本连通区域时，其他连通区域做2^n计算
        for (int j = 0; j < int(safeCellsList.size()); j++)
        {
            // 遍历其他的连通区域
            if (i != j)
            {
                // 从目前剩余的雷数开始--
                for (int ii = mineNum; ii >= 0; ii--)
                {
                    double s = 0.0;
                    // 遍历第j个连通分区的 当有假设有n个雷时的局面数 集合     it = map<int, double>
                    for (auto it : solutionNumOfpresumeBoomNumList[j])
                    {
                        // 这个if是为了保证全局剩余不能小于当前连通分区假设的雷数
                        if (ii >= it.first)
                        {
                            // ii - it.first 表示游戏显示的剩余雷数 - 第j个连通区域的假设雷数
                            // s += sum(杨辉三角) * 有多少个雷时的解数，即总的可能局面数 = 非该连通区域情况数(2^n) * 该连通区域解数
                            s += solutionOfOtherConnectedComponent[ii - it.first] * it.second;
                        }
                    }
                    // 例如：此连通区域有10或11个雷的情况共n种解，且当有10个雷有n1种解，有11个雷时有n2种解(其中n1，n2满足n1+n2=n)
                    // 但当时局面上总共有未知雷20个，
                    solutionOfOtherConnectedComponent[ii] = s;
                    //qDebug("%d %lf", ii, s);
                }
            }
            // 再进入下一个连通区域进行计算
        }


        // 一个连通分区局面解数
        double totalSolutionNum = 0.0;

        // vector<pair<int, double>> it , 即<地雷数, 解数>
        // 遍历当前连通分区的当假设的雷数为n时的连通分区总情况个数
        for (auto it : solutionNumOfpresumeBoomNumList[i])
        {
            /****** 核心分析 ******/
            // it.first  表示该连通区域的被假设的雷数 记为n
            // it.second 表示当该连通区域被假设有n个雷时的解数
            // mineNum - it.first 表示当该连通区域被假设有n个雷时，非该连通区域的其他的连通区域未知格子数 记为N
            // YangHuiList[mineNum - it.first] 表示该连通区域以外的未知格子数为N时，排除该连通区域的区域的总可能情况数
            // it.second * YangHuiList[mineNum - it.first] 表示当该连通区域被假设有n个雷, 全局的可能情况数
            // 因此对 it.second * YangHuiList[mineNum - it.first] 求和，即可得到以该联通为准的全局的可能情况数
            totalSolutionNum += it.second * solutionOfOtherConnectedComponent[mineNum - it.first];
            //qDebug("%d %f", it.first, it.second);

        }

        // 遍历当前连通区域未知格数次，计算错误的解数
        for (int j = 0; j < int(unkownCellsList[i].size()); j++)
        {
            double wrongSolutionNum = 0.0;
            // 第i个连通区域中，即当前连通区域第j个未知格在当前连通区域被假设为有n个地雷时，第j个未知格是雷的情况个数 map<地雷数，是雷格的情况数>
            for (auto it : perBoardCntList[i][j])
            {
                /*** 核心分析 此处分析同上 ***/
                // it.first  表示该连通区域的被假设的雷数 记为n
                // it.second 表示当该连通区域被假设有it.first个雷时的，第j个未知格被判定为雷格的解数
                // mineNum - it.first 表示当该连通区域被假设有n个雷时，该连通区域以外的未知格子数 记为N
                // YangHuiList[mineNum - it.first] 表示该连通区域以外的未知格子数为N时，排除该连通区域的区域的总可能情况数
                // it.second * YangHuiList[mineNum - it.first] 表示当该连通区域被假设有n个雷且第j个未知格被判定为雷格时, 全局的可能情况数，即第i个连通分区被判定为n个雷格时，第j个未知格被判定为雷格时的全局总情况数
                // 因此对 it.second * YangHuiList[mineNum - it.first] 求和，即对该连通分区的每个未知格遍历，即可得到错误的总数
                wrongSolutionNum += it.second * solutionOfOtherConnectedComponent[mineNum - it.first];

            }
            // 则得第i个连通区域的第j个未知格的精确概率
            prob[unkownCellsList[i][j].first][unkownCellsList[i][j].second] = wrongSolutionNum / totalSolutionNum;
        }
        // 再进入最外层的for循环，计算下一个连通区域的各个未知格精准概率
    }

    // 假设的雷数
    double psumBoomNum = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果board[i][j]是未知格 && 为雷概率 > - 0.5，即筛选出连通区域的未知格
            // 由精确概率通过求数学期望来近似计算连通区域假设的雷 注意：求出的雷数是浮点数，不是整数，因为是近似计算
            if (board[i][j] == -1 && prob[i][j] > 0)
            {
                psumBoomNum += prob[i][j] * 1;
            }
        }
    }

    // 计算非联通区域的为雷概率
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果board[i][j]是未知格 && 为雷概率 < - 0.5
            if (board[i][j] == -1 && prob[i][j] < 0)
            {
                // 非联通区域的为雷概率 = (整个局面 剩余雷数 - 假设的雷数) / 非连通区域的未知格子数
                prob[i][j] = (mineNum - psumBoomNum) / isolatedBoardNum;
            }
        }
    }

}


///
/// \brief AutoSweeper::getConnectedRelation
/// \param safeCells
/// \param unknownCells
/// \param connected
///
void AutoSweeper::GetConnectedRelation(const vector<pair<int, int>>& safeCells, const vector<pair<int, int>>& unknownCells, vector<vector<int>>& connected)
{
    // 遍历连通区域外圈的未知集合
    // connected[i][j]表示第i个未知格与第j个已知格相邻(j可以有多个)
    for (int i = 0; i < int(unknownCells.size()); i++)
    {
        // vector<vector<int>> connected;
        connected.push_back(vector<int>());
        for (int j = 0; j < int(safeCells.size()); j++)
        {
            // 如果第i个未知格和第j个已知格相邻
            if (abs(safeCells[j].first - unknownCells[i].first) <= 1 && abs(safeCells[j].second - unknownCells[i].second) <= 1)
            {
                // 将ij存入边界数组，
                connected[i].push_back(j);
            }
        }
    }
}



///
/// \brief AutoSweeper::getMinBoomProbBlock
/// \param board
/// \param prob
/// \return
///
pair<int, int> AutoSweeper::GetMinBoomProbBlock(const vector<vector<int>>& board, vector<vector<double>>& prob)
{
    vector<pair<int, int>> minProbBlock;
    double minP = 1;
    pair<int, int> minBoomProbBlock(-1, -1);

    // 100%的没有了，遍历为雷概率数组，找出为雷概率最小的左击
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果未知格 && p[i][j]即为雷概率较小
            if (board[i][j] == -1 && (prob[i][j] < minP  || (abs(prob[i][j] - minP) < 1e-8)))
            {
                minP = prob[i][j];
                minBoomProbBlock = pair<int, int>(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // 如果未知格 && p[i][j]即为雷概率较小
            if (board[i][j] == -1 && prob[i][j] == minP)
            {
                minProbBlock.push_back(make_pair(i, j));
            }
        }
    }



    if(IsIsolated(board, minBoomProbBlock.first, minBoomProbBlock.second))
    {
        minBoomProbBlock = GetAngle(board, minBoomProbBlock);
    }

    return  minBoomProbBlock;
}


///
/// \brief AutoSweeper::Main
/// \param board
/// \param pointsLeft
/// \param pointsRight
/// \return
///
bool AutoSweeper::Main(vector<vector<int>> board, vector<pair<int, int>> &pointsLeft, vector<pair<int, int>> &pointsRight)
{
    // 清空点集合
    pointsLeft.clear();
    pointsRight.clear();



    // **1、如果有100%是地雷的点
    if(GetBoomPoints(board, pointsRight))
    {
        return true;
    }

    // **2、如果有100%安全的点
    if (GetSafePoints(board, pointsLeft))
    {
        return true;
    }

    // **3、减法公式
    if(GetPointByTwoConnected(board, pointsLeft, pointsRight))
    {
        return true;
    }

    // **4、如果通过连通区域找到100%安全的格子
    vector<vector<double>> prob(n, vector<double>(m, -1));
    if (Explore(board, pointsLeft, pointsRight, prob))
    {
        return true;
    }

    // **5、如果通过连通区域没找到100%安全的格子，寻找概率最小的点，存入pointsLeft
    pair<int, int> minBoomProbBlock = GetMinBoomProbBlock(board, prob);
    pointsLeft.push_back(minBoomProbBlock);
    return false;

    // **0、如果第一个格子未知，即刚开局第一步点哪个
    if (board[0][0] == -1)
    {
        // 我要左击(0, 0)
        pointsLeft.push_back(make_pair(0, 0));
        return true;
    }
}

// 返回指定半径周围的数字格数量
//int AutoSweeper::getNumberCellCntAround(const vector<vector<int>>& board, int x, int y, int radius)
//{
//    int res = 0;
//    for (int i = x - radius + 1; i < x + radius; ++i)
//        for (int j = y - radius + 1; j < y + radius; ++j) {
//            if (i < 0 || i >= ROW || j < 0 || j >= COL) continue;
//            if (i == x && j == y) continue;
//            if (board[i][j] >= 1 && board[i][j] <= 8) ++res;
//    }
//    return res;
//}

pair<vector<pair<int, int>>, vector<pair<int, int>>> AutoSweeper::CheckTwoUncoveredCell(const vector<vector<int>>& board, int x1, int y1, int x2, int y2)
{
    pair<vector<pair<int, int>>, vector<pair<int, int>>> value;
    int num1 = board[x1][y1], num2 = board[x2][y2];
    int diffX = x2 - x1, diffY = y2 - y1;
    if (abs(diffX) + abs(diffY) != 1)
        return value;


    vector<pair<int, int>> around1;
    vector<pair<int, int>> around2;

    for (int i = -1; i < 2; ++i)
    {
        int xx1 = x1 - diffX + diffY * i, yy1 = y1 - diffY + diffX * i;
        if (IsPointInRange(xx1, yy1))
        {
            int pp1 = board[xx1][yy1];
            if (pp1 == -2) --num1;
            else if (pp1 == -1) around1.push_back(make_pair(xx1, yy1));
        }

        int xx2 = x2 + diffX + diffY * i, yy2 = y2 + diffY + diffX * i;
        if (IsPointInRange(xx2, yy2))
        {
            int pp2 = board[xx2][yy2];
            if (pp2 == -2) --num2;
            else if (pp2 == -1) around2.push_back(make_pair(xx2, yy2));
        }
    }
    if (num2 - num1 - around2.size() == 0)
    {
        value = make_pair(around1, around2);
    }
    else if (num1 - num2 - around1.size() == 0)
    {
        value = make_pair(around2, around1);
    }
    return value;
}

bool AutoSweeper::IsPointInRange(int x, int y)
{
    return x >= 0 && x < ROW && y >= 0 && y < COL;
}

bool AutoSweeper::GetPointByTwoConnected(const vector<vector<int> > &board, vector<pair<int, int> > &pointsLeft, vector<pair<int, int> > &pointsRight)
{
    pair<vector<pair<int, int>>, vector<pair<int, int>>> value;
    for(int x1 = 0; x1 < ROW; x1++)
    {
        for(int y1 = 0; y1 < COL; y1++)
        {
            for (int i = 0; i < 2; ++i)
            {
                value.first.clear();
                value.second.clear();
                int x2 = x1 + i, y2 = y1 + 1 - i;
                if (!IsPointInRange(x2, y2)) continue;
                if(board[x1][y1] >= 1 && board[x1][y1] <=8 && board[x2][y2] >= 1 && board[x2][y2] <=8)
                    value = CheckTwoUncoveredCell(board , x1, y1, x2, y2);
                if(value.first.size() > 0)
                {
                    for(auto p: value.first)
                    {
                        pointsLeft = value.first;
                    }
                }
                if(value.second.size() > 0)
                {
                    pointsRight = value.second;
                }
            }
        }
    }
    if(pointsLeft.size() + pointsRight.size() > 0)
        return true;
    else
        return false;
}


//double AutoSweeper::CalculateAvgNumOfSafeCells(vector<vector<int> > &board, int x, int y, int** ccGraph)
//{
//    // 假设当前格子不是雷, 计算该格子所涉及的连通分量
//    vector<pair<int, int>> unkownCells;
//    vector<pair<int, int>> safeCells;
//    Set<Integer> ccSet = new HashSet<>(8);
//    bool[ROW][COL] vis;
//    vis[x][y] = true;
//    for (auto p : GetAround(x, y))
//    { // 周围一圈的未知格子都属于该分量
//        if (board[p.first][p.second] == -1)
//        {
//            vis[p.first][p.second] = true;
//            newCcPoints.add(p);
//            if (ccGraph[p.first][p.second] > 0) ccSet.add(ccGraph[p.first][p.second]);
//        }
//    }
//    for (int i = 0; i < newCcPoints.size(); ++i)
//    {
//        // 周围一圈格子所涉及的连通分量也都属于该新分量
//        Point p = newCcPoints.get(i);
//        for (Point pa : game.getAround(p.x, p.y)) {
//            if (!vis[pa.x][pa.y] && ccSet.contains(ccGraph[pa.x][pa.y])) {
//                vis[pa.x][pa.y] = true;
//                newCcPoints.add(pa);
//            }
//        }
//    }
//    // 连通分量太长的不算, 不然太耗时
//    if (newCcPoints.size() > 16) return -1;

//    // 根据新连通分量, 计算: 当目标格子为 0~8 时, 后续可以确定多少个格子必为非雷
//    // (只统计必非雷的格子, 没统计必为雷的格子, 因为标雷对棋局没什么实质性帮助)
//    int alwaysSafe = 0, allPermCnt = 0;
//    for (int num = 0; num <= 8; ++num) {
//        board[x][y] = num;
//        Map<Integer, int[]> perm = new HashMap<>(16);
//        backtrackAllPossiblePermutations(game, board, newCcPoints, perm, 0, 0);
//        int[] allCount = new int[newCcPoints.size() + 1];
//        for (int[] v : perm.values()) {
//            for (int i = 0; i < allCount.length; ++i) {
//                allCount[i] += v[i];
//            }
//        }
//        final int permCnt = allCount[newCcPoints.size()];
//        if (permCnt == 0) continue;
//        allPermCnt += permCnt;
//        for (int mineCnt : allCount) {
//            if (mineCnt == 0) alwaysSafe += permCnt;
//        }
//    }
//    board[x][y] = -1;

//    // 计算: 当当前格不为雷时, 则挖开该格子后, 局面上平均有多少格未知格子因此被确定不是雷
//    return (double) alwaysSafe / allPermCnt;
//}

vector<pair<int, int>> AutoSweeper::GetAround(int x, int y)
{
    vector<pair<int, int>> around;
    // 遍历(i, j)点周围的8个点
    for (int ii = max(0, x - 1); ii < min(ROW, x + 2); ii++)
    {
        for (int jj = max(0, y - 1); jj < min(COL, y + 2); jj++)
        {
            if(ii == x && jj == y) continue;
            around.push_back(make_pair(ii, jj));
        }
    }
    return around;
}

//int backtrackAllPossiblePermutations(Game game, vector<vector<int, int>>& board, vector<pair<int, int>> points,
//                                                       Map<Integer, int[]> ccPerm, int curIndex, int curMine)
//{
//    // 成功找到一个可能的排列
//    if (curIndex >= points.size())
//    {
//        int[] count; // count 前 points.size() 位保存所有情况中格子有雷的次数, 最后一位保存回溯出多少种情况
//        if (ccPerm.containsKey(curMine)) count = ccPerm.get(curMine);
//        else {
//            count = new int[points.size() + 1];
//            ccPerm.put(curMine, count);
//        }
//        for (int i = 0; i < points.size(); ++i) {
//            auto p = points[i];
//            if (board[p.first][p.second] == -2) ++count[i];
//        }
//        ++count[points.size()]; // 排列个数总数
//        return 1;
//    }

//    // 分别递归考虑当前格子是雷、不是雷的情况
//    Point cur = points.get(curIndex);
//    int res = 0;
//    board[cur.x][cur.y] = -2;
//    if (curMine < game.getBoomLeft(board) && isUncheckedCellLegal(game, board, cur.x, cur.y))
//    {
//        res += backtrackAllPossiblePermutations(game, board, points, ccPerm, curIndex + 1, curMine + 1);
//    }
//    board[cur.x][cur.y] = MineSweeper.NOT_MINE;
//    if (isUncheckedCellLegal(game, board, cur.x, cur.y))
//    {
//        res += backtrackAllPossiblePermutations(game, board, points, ccPerm, curIndex + 1, curMine);
//    }
//    board[cur.x][cur.y] = -1;
//    return res;
//}




