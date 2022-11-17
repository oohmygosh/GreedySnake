//
// Created by Lee on 2022/6/18.
//

#include "AStar.h"
#include "Snake.h"

AStar::AStar() = default;

AStar::~AStar()
= default;

Snake *body = nullptr;
Snake *test = nullptr;

Node *AStar::search(Node *startNode, Node *endNode, bool flag) {
    Node *current;
    this->startPos = startNode;
    this->endPos = endNode;
    if (startNode->x < 0 || startNode->x >= row || startNode->y < 20 || startNode->y > col - STEP
        ||
        endPos->x < 0 || endPos->x >= row || endPos->y < 20 || endPos->y > col - STEP)
        return nullptr;
    openList.push_back(startNode);
    /**
     * 主要是这块，把开始的节点放入openList后开始查找旁边的4个节点，
     * 如果坐标超长范围或在closeList就return 如果已经存在openList就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
     * 如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和 f 如果是新节点 加入到openList直到openList为空或找到终点
     **/
    bool result = true;
    while (!openList.empty()) {
        current = openList[0];
        if (current->x == endNode->x && current->y == endNode->y) {
            result = true;
            break;
        } else
            result = false;
        // 每走一步更新一遍障碍物
        if (flag) {
            delete body;
            body = test->copySnake();
            updateBody(current);
        }
        // 把当前节点周围可以走的路径添加到openList
        NextStep(current);
        // 把当前路径标记为已走过
        closeList.push_back(current);
        // 删除当前路径
        openList.erase(openList.begin());
        // 根据路径的权值排序
        sort(openList.begin(), openList.end(), compare);
    }
    vector<Node *>().swap(openList);
    vector<Node *>().swap(closeList);
    delete test;
    delete body;
    delete endNode;
    if(!result){
        delete current;
        current = nullptr;
    }
    return current;
}

void AStar::checkPoint(int x, int y, Node *father, int g) {
    // 边界检测
    if (x < 0 || x >= row || y < 20 || y > col - STEP)
        return;
    // 碰撞检测
    if (AStar::unWalk(x, y))
        return;
    // 是否已经走过
    if (isContains(&closeList, x, y) != -1)
        return;
    int index;
    if ((index = isContains(&openList, x, y)) != -1) {
        Node *point = openList[index];
        if (point->g > father->g + g) {
            point->father = father;
            point->g = father->g + g;
            point->f = point->g + point->h;
        }
    } else {
        Node *point = new Node(x, y, father);
        countGHF(point, endPos, g);
        openList.push_back(point);
    }
}

void AStar::NextStep(Node *current) {
    checkPoint(current->x - STEP, current->y, current, WeightW);//左
    checkPoint(current->x + STEP, current->y, current, WeightW);//右
    checkPoint(current->x, current->y - STEP, current, WeightW);//上
    checkPoint(current->x, current->y + STEP, current, WeightW);//下
}

int AStar::isContains(vector<Node *> *Nodelist, int x, int y) {
    for (int i = 0; i < Nodelist->size(); i++)
        if (Nodelist->at(i)->x == x && Nodelist->at(i)->y == y)
            return i;
    return -1;
}

void AStar::countGHF(Node *sNode, Node *eNode, int g) {
    int h = abs(sNode->x - eNode->x) * WeightW + abs(sNode->y - eNode->y) * WeightW;
    int currentNodeG = sNode->father->g + g;
    int f = currentNodeG + h;
    sNode->f = f;
    sNode->h = h;
    sNode->g = currentNodeG;
}

bool AStar::compare(Node *n1, Node *n2) {
    return n1->f < n2->f;
}


bool AStar::unWalk(int x, int y) const {
    if (x == endPos->x && y == endPos->y)
        return false;
    return !Snake::checkPosition(x, y, body->getHead());
}

void AStar::buildMap(Snake *pSnake) {
    body = pSnake->copySnake();
    test = pSnake->copySnake();
}


Node *AStar::reversalPath(Node *path) {
    if (path == nullptr || path->father == nullptr) {
        return path;
    } else {
        Node *pNode = reversalPath(path->father);
        path->father->father = path;
        path->father = nullptr;
        return pNode;
    }
}

void AStar::updateBody(Node *pNode) {
    Node *temp = pNode;
    while (temp->father != nullptr) {
        body->pushFront(new Data(temp->x, temp->y, body->getHead()->data->length));
        temp = temp->father;
        body->popBack();
    }
}

Snake *AStar::buildSnake(Node *foodPath, Snake *pSnake) {
    Snake *copySnake = pSnake->copySnake();
    stack<Node *> stack;
    copySnake->getHead()->data->length++;
    Node *temp = foodPath;
    while (temp != nullptr) {
        stack.push(temp);
        temp = temp->father;
    }
    unsigned long long int size = stack.size();
    while (!stack.empty()) {
        Node *&pNode = stack.top();
        stack.pop();
        copySnake->pushFront(new Data(pNode->x, pNode->y, copySnake->getHead()->data->length));
        pNode = nullptr;
        delete pNode;
    }
    while (--size > 1) {
        copySnake->popBack();
    }
    return copySnake;
}
