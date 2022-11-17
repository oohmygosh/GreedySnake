//
// Created by Lee on 2022/6/16.
//

#include <cstdio>
#include "Snake.h"


#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"

void Snake::popFront() {
    erase(this->head->next);
}

void Snake::popBack() {
    erase(this->head->previous);
}

void Snake::insert(snake *pos, Data *data) {
    auto *newNode = new snake(data);
    newNode->previous = pos;
    pos->next->previous = newNode;
    newNode->next = pos->next;
    pos->next = newNode;
}

void Snake::pushFront(Data *data) {
    insert(head->previous, data);
    head = head->previous;
}

void Snake::pushBack(Data *data) {
    insert(this->head, data);
}

void Snake::erase(snake *pos) {
    snake *previous = pos->previous;
    snake *next = pos->next;
    delete pos;
    previous->next = next;
    next->previous = previous;
}

bool Snake::checkPosition(int x, int y, snake *s) {
    snake *temp = s;
    while ((temp = temp->next) != s)
        if (temp->data->x == x && temp->data->y == y)
            return false;
    return true;
}

bool Snake::overEdge() {
    return
            this->head->data->x >= with || this->head->data->y > high - 10 ||
            this->head->data->x < 0 || this->head->data->y < 20;
}

bool Snake::overEdge(int x, int y) {
    return
            x >= with || y > high - 10 ||
            x < 0 || y < 20;
}

bool Snake::hitItself() {
    snake *h = this->head;
    snake *body = this->head;
    while ((body = body->previous) != this->head->next)
        if (h->data->x == body->data->x && h->data->y == body->data->y)
            return true;
    return false;
}


int *Snake::getFood() {
    return curFood;
}

void Snake::move(int direction) {
    this->pushFront(new Data(this->head->data->x, this->head->data->y, this->head->data->length));
    switch (direction) {
        case VK_LEFT:
            this->head->data->x -= 10;
            break;
        case VK_RIGHT:
            this->head->data->x += 10;
            break;
        case VK_DOWN:
            this->head->data->y += 10;
            break;
        case VK_UP:
            this->head->data->y -= 10;
            break;
        default:
            return;
    }
    // ´©Ç½
    if (canCrossWall)
        crossWallMove();
    if (this->head->data->x == curFood[0] && this->head->data->y == curFood[1]) {
        this->head->data->length++;
        curFood[2] = 1;
    } else {
        clearBody(this->head->previous);
        this->popBack();
    }
    printSnake();
}


Snake::Snake() {
    initSnake();
}

// ´©Ç½
void Snake::crossWallMove() {
    Data *data = this->head->data;
    if (data->x == with)
        data->x = 0;
    else if (data->x == -STEP)
        data->x = with - STEP;
    else if (data->y == STEP)
        data->y = high - 10;
    else if (data->y == high)
        data->y = 20;
    clearBody(head);
}

void Food::createFood(snake *head) {
    if (curFood[2] == 0)
        return;
    int x, y;
    while (true) {
        x = rand() % (abs(with / 10)) * 10;
        y = ((rand() % (abs(high / 10) - 2)) + 2) * 10;
        if (Snake::checkPosition(x, y, head) &&  (x != head->data->x && y != head->data->y))
            break;
    }
    //»æÖÆÊ³Îï
    setfillcolor(RGB(253, 168, 198));
    curFood[0] = x;
    curFood[1] = y;
    curFood[2] = 0;
    fillrectangle(curFood[0], curFood[1], curFood[0] + STEP, curFood[1] + STEP);
}

void *Snake::initSnake() {
    int x = 0;
    int y = 20;
    int length = 1;
    this->head = new snake(new Data(x, y, length));
    for (int i = 1; i < SNAKE_LEN; ++i)
        pushFront(new Data(x += STEP, y, ++length));
}

void Snake::printDescription() {
    line(0, 20, with, 20);
    TCHAR score[5];
    TCHAR speed[5];
    sprintf(speed, _T("%s:%d"), "speed", this->sleepTime);
    outtextxy(with * 0.4, 0, speed);
    if (invincible) {
        TCHAR Invincible[5];
        sprintf(Invincible, _T("%s"), "ÎÞµÐ");
        outtextxy(with - 80, 0, Invincible);
    } else {
        outtextxy(with - 80, 0, "        ");
    }
    if (canCrossWall) {
        TCHAR crossWall[5];
        sprintf(crossWall, _T("%s"), "´©Ç½");
        outtextxy(with - 120, 0, crossWall);
    } else {
        outtextxy(with - 120, 0, "        ");
    }
    if (automaticWayFinding) {
        TCHAR autoMatic[5];
        sprintf(autoMatic, _T("%s"), "ÍÐ¹Ü");
        outtextxy(with - 40, 0, autoMatic);
    } else {
        outtextxy(with - 40, 0, "        ");
    }

    sprintf(score, _T("%s:%d"), "score", head->data->length);
    outtextxy(0, 0, score);
}

void Snake::printSnake() {
    snake *body = this->head;
    while ((body = body->previous) != this->head)
        printBody(body);
    printHead();
    printTail();
    printDescription();
}

void Snake::printHead() {
    setfillcolor(RED);
    fillrectangle(head->data->x, head->data->y,
                  head->data->x + STEP, head->data->y + STEP);
}

void Snake::printBody(snake *body) {
    setfillcolor(GREEN);
    fillrectangle(body->data->x, body->data->y,
                  body->data->x + STEP, body->data->y + STEP);
}

void Snake::clearBody(snake *body) {
    clearrectangle(body->data->x, body->data->y,
                   body->data->x + STEP, body->data->y + STEP);
}

Data::Data(int x, int y, int length) : x(x), y(y), length(length) {
    this->x = x;
    this->y = y;
    this->length = length;
}

Snake *Snake::copySnake() {
    snake *temp = this->head;
    auto newSnake = new Snake(new Data(temp->data->x, temp->data->y, temp->data->length));
    while ((temp = temp->previous) != this->head)
        newSnake->pushFront(new Data(temp->data->x, temp->data->y, temp->data->length));
    newSnake->head = newSnake->head->previous;
    return newSnake;
}

void Snake::enableCrossWall() {
    canCrossWall = !canCrossWall;
}

void Snake::enableInvincible() {
    invincible = !invincible;
}

bool Snake::getInvincible() const {
    return invincible;
}

void Snake::speedUp() {
    if (sleepTime < 50)
        sleepTime += 10;
}

void Snake::speedDown() {
    if (sleepTime > 10)
        sleepTime -= 10;
}

Snake::Snake(Data *data) {
    this->head = new snake(data);
}

void Snake::printTail() {
    setfillcolor(BLUE);
    fillrectangle(head->previous->data->x, head->previous->data->y,
                  head->previous->data->x + STEP, head->previous->data->y + STEP);
}

Snake::~Snake() {
    delete head;
}

#pragma clang diagnostic pop