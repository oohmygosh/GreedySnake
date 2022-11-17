//
// Created by Lee on 2022/6/16.
//

#ifndef GREEDYSNAKE_SNAKE_H

#include "Map.h"

#define SNAKE_LEN 4
#define STEP 10
#define GREEDYSNAKE_SNAKE_H

typedef struct Data {
    int x, y, length;
    Data(int x, int y, int length);
} Data;
typedef struct snake {
    struct Data *data;
    struct snake *previous, *next;
    __attribute__((unused)) explicit snake(Data *const data) {
        this->data = data; // 存储 蛇的 X,Y坐标点和长度
        this->previous = this;
        this->next = this;
    }
    virtual ~snake() = default;
} snake;

class Snake {
private:
    snake *head{};
    // 无敌
    bool invincible = false;
    // 穿墙开关
    bool canCrossWall = false;
    // 速度
    int sleepTime = 10;
    bool automaticWayFinding = false;
public:
    // 初始化
    void *initSnake();
    Snake();
    explicit Snake(Data *data);
    virtual ~Snake();
    // 移动函数
    void move(int direction);
    // 打印信息
    void printDescription();
    // 打印蛇
    void printSnake();
    // 插入
    static void insert(snake *pos, Data *data);
    // 删除
    static void erase(snake *pos);
    // 弹出蛇头
    void popFront();
    // 获取蛇头
    snake *getHead() {
        return this->head;
    };
    // 弹出蛇尾
    void popBack();
    // 打印蛇头
    void printHead();
    // 打印身体
    static void printBody(snake *body);
    // 清除尾巴的打印效果
    static void clearBody(snake *body);
    // 检查坐标是否合法
    static bool checkPosition(int x, int y, snake *s);
    // 超出边界
    bool overEdge();
    // 判断蛇头是否触碰蛇身
    bool hitItself();
    // 头插
    void pushFront(Data *data);
    // 尾插
    void pushBack(Data *data);
    // 穿墙
    void crossWallMove();
    // 获取速度
    int getSpeed() const {
        return sleepTime;
    }
    // 穿墙开关
    int crossWall() const {
        return canCrossWall;
    }
    // 自动寻路开关
    void enableAutomaticWayFinding() {
        automaticWayFinding = !automaticWayFinding;
    };
    // 获取自动寻路开关状态
    bool getAutomaticWayFinding() const {
        return automaticWayFinding;
    };
    // 穿墙开关
    void enableCrossWall();
    // 无敌开关
    void enableInvincible();
    // 获取无敌状态
    bool getInvincible() const;
    // 加速
    void speedUp();
    // 减速
    void speedDown();
    // 复制蛇
    Snake *copySnake();
    // 获取食物
    int *getFood();
    // 检查坐标是否超出边界
    bool overEdge(int x, int y);
    // 打印蛇尾
    void printTail();
};

typedef int food[3];
static food curFood = {100, 100, 1};

class Food {
public:
    static void createFood(snake *head);
    static int * getFood(){
        return curFood;
    };
};

#endif //GREEDYSNAKE_SNAKE_H
