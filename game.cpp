#include "get.h"


Game::Game()
{

}

bool Game::IsFind(Mouse mouse, Color color)
{
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            mouse.Push(i, j, 2);
            if(color.IsBoom(i, j))
            {
                return true;
            }
        }
    }
    return false;
}

void Game::GetGame(vector<vector<int>>& game)
{
    Color color;
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            if(color.IsBoom(i, j))
            {
                game[i][j] = -2;
            }
        }
    }
}
