//
// Created by Lee on 2022/6/17.
//

#ifndef GREEDYSNAKE_GAME_H
#include "Snake.h"
#include "AStar.h"

#define GREEDYSNAKE_GAME_H


class Game {
private:
    int direction = VK_RIGHT;
    AStar* aStar = new AStar();
    Snake* python = new Snake();
public:
    [[noreturn]] void startGame();
    [[noreturn]] void controller();
    void gameOver();
    // 吃食物后的路径
    Node *eatFoodPath();
    // 吃完食物后蛇头到尾巴的路径
    Node *tailPathAfterEatFood(Node *foodPath);
    // 离食物最远的位置移动一步
    Node *stayAwayFromFood();
    // 追尾路径
    Node *tailPath();

    void autoMove(int x, int y);
};


#endif //GREEDYSNAKE_GAME_H
