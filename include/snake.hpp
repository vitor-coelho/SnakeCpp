#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <vector>


typedef struct{
    int x;
    int y;
} pos;


class Snake{
    private:
        std::vector<pos> body;
        int direction = 0;  // 0: right, 1: up, 2: left, 3: down
        int size = 3;
        bool alive = true;

    public:
        Snake(int xBoundary, int yBoundary);
        ~Snake();

        int getDir();
        void setDir(int newDir);
        bool getAlive();
        int getScore();
        std::vector<pos> getBody();
        pos getHead();

        bool checkPosInBody(int x, int y);
        bool checkWallCollision(int xBoundary, int yBoundary);
        bool checkBodyCollision();
        bool checkAppleEat(int xApple, int yApple);
        
        void move();
        void grow();

        bool updateSnake(int xBoundary, int yBoundary, int xApple, int yApple);
};

#endif
