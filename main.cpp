#include <iostream>
#include "Game.h"

int main() {
//    srand((unsigned int) (time(nullptr)));
    Map::initMap();
    Game game;
    game.startGame();
}
