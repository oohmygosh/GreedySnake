//
// Created by Lee on 2022/6/18.
//

#ifndef GREEDYSNAKE_ASTAR_H
#define GREEDYSNAKE_ASTAR_H


#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include<algorithm>
#include "Snake.h"
#include "Map.h"

using namespace std;

typedef struct Node {
    int x, y;
    int g; //起始点到当前点实际代价
    int h;//当前节点到目标节点最佳路径的估计代价
    int f;//估计值
    Node *father;

    Node(int x, int y) {
        this->x = x;
        this->y = y;
        this->g = 0;
        this->h = 0;
        this->f = 0;
        this->father = nullptr;
    }

    Node(int x, int y, int f) {
        this->x = x;
        this->y = y;
        this->g = 0;
        this->h = 0;
        this->f = f;
        this->father = nullptr;
    }

    Node(int x, int y, Node *father) {
        this->x = x;
        this->y = y;
        this->g = 0;
        this->h = 0;
        this->f = 0;
        this->father = father;
    }

    virtual ~Node() = default;
} Node;

class AStar {
public:
    AStar();
    ~AStar();
    // 入口函数
    Node *search(Node *startNode, Node *endNode, bool flag);
    // 检测节点
    void checkPoint(int x, int y, Node *father, int g);
    // 探测周围节点
    void NextStep(Node *currentPoint);
    // 检测节点是否已经探测过
    static int isContains(vector<Node *> *Nodelist, int x, int y);
    // 计算节点的GHF值
    static void countGHF(Node *sNode, Node *eNode, int g);
    // 比较节点F值
    static bool compare(Node *n1, Node *n2);
    // 节点是否可达
    bool unWalk(int x, int y) const;
    // 构建障碍物
    static void buildMap(Snake* pSnake);
    // 构建虚拟蛇
    static Snake * buildSnake(Node *foodPath, Snake* pSnake);
    // 反转链表
    static Node *reversalPath(Node *path);
    // 更新障碍物
    static void updateBody(Node *pNode);
    vector<Node *> openList;
    vector<Node *> closeList;
    Node *endPos{};
    static const int WeightW = STEP;// 正方向消耗
    int row = with;
    int col = high;
    Node *startPos{};
};


#endif //GREEDYSNAKE_ASTAR_H
