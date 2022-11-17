//
// Created by Lee on 2022/6/17.
//

#include "AStar.h"
#include "Game.h"

class __attribute__((unused)) Compare_Node_Pointer {
public:
    /* Node::priority 大的优先 */
    bool operator()(Node *&a, Node *&b) const {
        return a->f < b->f;
    }
};


[[noreturn]] void Game::startGame() {
    python->printSnake();
    controller();
}
[[noreturn]] void Game::controller() {
    Node *path = nullptr;
    while (true) {
        if (python->getHead()->data->length == (with / 10) * ((high - 20)  / 10) - 1 )
            gameOver();
        Food::createFood(python->getHead());
        if (python->getAutomaticWayFinding()) {
            if (path == nullptr) {
                Node *foodPath = eatFoodPath();
                Node *afterEatFood = nullptr;
                if (foodPath != nullptr)
                    afterEatFood = tailPathAfterEatFood(foodPath);
                if (afterEatFood == nullptr) {
                    delete foodPath ;  foodPath = stayAwayFromFood();
                }
                if (foodPath == nullptr)
                    foodPath = tailPath();
                path = AStar::reversalPath(foodPath)->father;
                delete afterEatFood;
            }
            autoMove(path->x,path->y);
        } else {
            if (GetAsyncKeyState(VK_UP) && direction != VK_DOWN)
                direction = VK_UP;
            else if (GetAsyncKeyState(VK_DOWN) && direction != VK_UP)
                direction = VK_DOWN;
            else if (GetAsyncKeyState(VK_LEFT) && direction != VK_RIGHT)
                direction = VK_LEFT;
            else if (GetAsyncKeyState(VK_RIGHT) && direction != VK_LEFT)
                direction = VK_RIGHT;
        }
        if (GetAsyncKeyState(VK_F2))
            python->enableCrossWall();
        else if (GetAsyncKeyState(VK_F5))
            python->enableInvincible();
        else if (GetAsyncKeyState(VK_F8))
            python->enableAutomaticWayFinding();
        else if (GetAsyncKeyState(VK_ADD))
            python->speedUp();
        else if (GetAsyncKeyState(VK_SUBTRACT))
            python->speedDown();
        if (!python->crossWall())
            if (python->overEdge())
                gameOver();
        if (!python->getInvincible() && python->hitItself())
            gameOver();
        python->move(direction);
        if (path != nullptr) {
            Node *temp = path;
            path = path->father;
            delete temp;
        }
        Sleep(python->getAutomaticWayFinding() ? 10 : abs(60 - python->getSpeed()) * 10);
    }
}

void Game::autoMove(int x,int y) {
    if (x < python->getHead()->data->x)
        direction = VK_LEFT;
    else if (x > python->getHead()->data->x)
        direction = VK_RIGHT;
    else {
        if (y > python->getHead()->data->y)
            direction = VK_DOWN;
        else if (y < python->getHead()->data->y)
            direction = VK_UP;
    }
}


Node *Game::eatFoodPath() {
    AStar::buildMap(python);
    return aStar->search(new Node(python->getHead()->data->x,python->getHead()->data->y), new Node(python->getFood()[0], python->getFood()[1]), true);
}

Node *Game::tailPathAfterEatFood(Node *foodPath) {
    Snake* newSnake = AStar::buildSnake(foodPath, python);
    snake *tail = newSnake->getHead()->previous;
    Node *endNode = new Node(tail->data->x, tail->data->y);
    AStar::buildMap(newSnake);
    Node *startNode = new Node(newSnake->getHead()->data->x, newSnake->getHead()->data->y);
    Node *result = aStar->search(startNode, endNode, false);
    delete newSnake;
    return result;
}

Node *Game::stayAwayFromFood() {
    std::priority_queue<Node *, std::vector<Node *>, Compare_Node_Pointer> priorityQueue;
    Data *head = python->getHead()->data;
    if (Snake::checkPosition(head->x + STEP, head->y,python->getHead()) && !python->overEdge(head->x + STEP, head->y))
        priorityQueue.push(new Node(head->x + STEP, head->y,
                                    abs(head->x + STEP - python->getFood()[0]) * STEP + abs(head->y - python->getFood()[1]) * STEP));
    if (Snake::checkPosition(head->x - STEP, head->y,python->getHead()) && !python->overEdge(head->x - STEP, head->y))
        priorityQueue.push(new Node(head->x - STEP, head->y,
                                    abs(head->x - STEP - python->getFood()[0]) * STEP + abs(head->y - python->getFood()[1]) * STEP));
    if (Snake::checkPosition(head->x, head->y + STEP,python->getHead()) && !python->overEdge(head->x, head->y + STEP))
        priorityQueue.push(new Node(head->x, head->y + STEP,
                                    abs(head->x - python->getFood()[0]) * STEP + abs(head->y + STEP - python->getFood()[1]) * STEP));
    if (Snake::checkPosition(head->x, head->y - STEP,python->getHead()) && !python->overEdge(head->x, head->y - STEP))
        priorityQueue.push(new Node(head->x, head->y - STEP,
                                    abs(head->x - python->getFood()[0]) * STEP + abs(head->y - STEP - python->getFood()[1]) * STEP));
    Node *path = nullptr;
    Node *node = nullptr;
    while (!priorityQueue.empty() && path == nullptr) {
        node = priorityQueue.top();
        priorityQueue.pop();
        if (node->x == python->getFood()[0] && node->y == python->getFood()[1]) continue;
        node->f = 0;
        Snake* simulatedSnake = python->copySnake();
        simulatedSnake->pushFront(new Data( node->x, node->y,simulatedSnake->getHead()->data->length));
        simulatedSnake->popBack();
        AStar::buildMap(simulatedSnake);
        snake *tail = simulatedSnake->getHead()->previous;
        Node *startNode = new Node(simulatedSnake->getHead()->data->x, simulatedSnake->getHead()->data->y);
        Node *endNode = new Node(tail->data->x, tail->data->y);
        path = aStar->search(startNode, endNode, false);
        delete simulatedSnake;
    }
    while (!priorityQueue.empty()) {
        Node *pNode = priorityQueue.top();
        delete pNode;
        priorityQueue.pop();
    }
    if (path == nullptr) return nullptr;
    delete node->father;
    node->father = new Node(0, 0);
    return node;
}

Node *Game::tailPath() {
    AStar::buildMap(python);
    snake *tail = python->getHead()->previous;
    Node *endNode = new Node(tail->data->x, tail->data->y);
    Node *startNode = new Node(python->getHead()->data->x, python->getHead()->data->y);
    Node *result = aStar->search(startNode, endNode, false);
    delete startNode;
//    delete endNode;
    return result;
}

void Game::gameOver() {
    RECT r = {0, 0, with, high};
    drawtext(_T("游戏结束！"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    delete python;
    delete aStar;
    getchar();
    python = new Snake();
    aStar = new AStar();
    direction = VK_RIGHT;
    cleardevice();
    python->getFood()[2] = 1;
    startGame();
}