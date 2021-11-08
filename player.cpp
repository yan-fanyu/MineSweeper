#include "player.h"

Player::Player()
{
    vector<vector<int>> newBoard(16, vector<int>(30, -1));
    board = newBoard;
}

Player::~Player()
{

}
