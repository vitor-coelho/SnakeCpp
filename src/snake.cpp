#include "..\include\snake.hpp"


Snake::Snake(int xBoundary, int yBoundary){
    pos initPos = {.x = xBoundary/2, .y = yBoundary/2};
    for(int i = 0; i < 3; i++){
        body.push_back(initPos);
        initPos.x -= 1;
    }
}

Snake::~Snake(){}

int Snake::getDir(){
    return direction;
}

void Snake::setDir(int newDir){
    direction = newDir;
}

bool Snake::getAlive(){
    return alive;
}

int Snake::getScore(){
    return size-3;
}

std::vector<pos> Snake::getBody(){
    return body;
}

void Snake::move(){
    for(auto position = body.rbegin(); position != body.rend()-1; position++){
        *position = *(position+1);
    }

    if(direction == 0)
        body.front().x += 1;
    else if(direction == 2)
        body.front().x -= 1;
    else if(direction == 1)
        body.front().y -= 1;
    else if(direction == 3)
        body.front().y += 1;
}

void Snake::grow(){
    size += 1;
    pos newPos = body.front();

    if(direction == 0)
        newPos.x += 1;
    else if(direction == 2)
        newPos.x -= 1;
    else if(direction == 1)
        newPos.y -= 1;
    else if(direction == 3)
        newPos.y += 1;

    body.insert(body.begin(), newPos);
}

bool Snake::checkPosInBody(int x, int y){
    for (auto position = body.cbegin(); position != body.cend(); ++position){
        if(x == (*position).x && y == (*position).y)
            return true;
    }
    return false;
}

bool Snake::checkWallCollision(int xBoundary, int yBoundary){
    if(direction == 0 && body.front().x == xBoundary-1)
        return true;
    else if(direction == 2 && body.front().x == 0)
        return true;
    else if(direction == 1 && body.front().y == 0)
        return true;
    else if(direction == 3 && body.front().y == yBoundary-1)
        return true;
    else
        return false;
}

bool Snake::checkBodyCollision(){
    pos nextPos = body.front();
    if(direction == 0)
        nextPos.x += 1;
    else if(direction == 2)
        nextPos.x -= 1;
    else if(direction == 1)
        nextPos.y -= 1;
    else if(direction == 3)
        nextPos.y += 1;

    return checkPosInBody(nextPos.x, nextPos.y);
}

bool Snake::checkAppleEat(int xApple, int yApple){
    pos nextPos = body.front();
    if(direction == 0)
        nextPos.x += 1;
    else if(direction == 2)
        nextPos.x -= 1;
    else if(direction == 1)
        nextPos.y -= 1;
    else if(direction == 3)
        nextPos.y += 1;

    if(nextPos.x == xApple && nextPos.y == yApple)
        return true;
    else
        return false;
}

bool Snake::updateSnake(int xBoundary, int yBoundary, int xApple, int yApple){
    if(this->checkAppleEat(xApple, yApple)){
        this->grow();
        return true;
    }else{
        if(checkBodyCollision())
            alive = false;

        if(checkWallCollision(xBoundary, yBoundary))
            alive = false;
        
        if(alive)
            move();

        return false;
    }
}
