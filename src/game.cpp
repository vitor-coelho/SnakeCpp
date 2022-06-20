#include "..\include\game.hpp"

std::mt19937 gameGen(time(NULL));

Game::Game(int w, int h, bool toShow, bool human){
    width = w;
    height = h;
    show = toShow;
    humanControl = human;
    if(human)
        started = false;

    snake = new Snake(w, h);

    newApple();

    if(toShow){
        SDL_Init(SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow("Snake", WIN_INIT_X, WIN_INIT_Y, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_Surface * image = SDL_LoadBMP("../img/snake-1.bmp");
        SDL_SetWindowIcon(window, image);
        SDL_FreeSurface(image);

        TTF_Init();
        updateScore(snake->getScore());
    }
}

Game::~Game(){}

bool Game::isRunning(){
    return running;
}

void Game::newApple(){
    std::uniform_int_distribution<int> xDist(0, width-1);
    std::uniform_int_distribution<int> yDist(0, height-1);

    while(true){
        int newX = xDist(gameGen);
        int newY = yDist(gameGen);

        if(snake->checkPosInBody(newX, newY) == false){
            appleX = newX;
            appleY = newY;
            break;
        }
    }
}

void Game::changeSnakeDir(int newDir){
    if(newDir == 0 && snake->getDir() != 2)
        snake->setDir(0);
    else if(newDir == 1 && snake->getDir() != 3)
        snake->setDir(1);
    else if(newDir == 2 && snake->getDir() != 0)
        snake->setDir(2);
    else if(newDir == 3 && snake->getDir() != 1)
        snake->setDir(3);
}

int Game::getSnakeScore(){
    return snake->getScore();
}

Matrix Game::getSnakeInputs(){
    pos head = snake->getHead();

    float dir0, dir1, dir2, dir3;
    float snakeFront, snakeLeft, snakeRight;
    if(snake->getDir() == 0){
        dir0 = 1;
        if(snake->checkPosInBody(head.x+1, head.y))
            snakeFront = 1;
        else if(snake->checkPosInBody(head.x, head.y+1))
            snakeRight = 1;
        else if(snake->checkPosInBody(head.x, head.y-1))
            snakeLeft = 1;

    }else if(snake->getDir() == 2){
        dir2 = 1;
        if(snake->checkPosInBody(head.x-1, head.y))
            snakeFront = 1;
        else if(snake->checkPosInBody(head.x, head.y-1))
            snakeRight = 1;
        else if(snake->checkPosInBody(head.x, head.y+1))
            snakeLeft = 1;

    }else if(snake->getDir() == 1){
        dir1 = 1;
        if(snake->checkPosInBody(head.x, head.y-1))
            snakeFront = 1;
        else if(snake->checkPosInBody(head.x+1, head.y))
            snakeRight = 1;
        else if(snake->checkPosInBody(head.x-1, head.y))
            snakeLeft = 1;

    }else if(snake->getDir() == 3){
        dir3 = 1;
        if(snake->checkPosInBody(head.x, head.y+1))
            snakeFront = 1;
        else if(snake->checkPosInBody(head.x-1, head.y))
            snakeRight = 1;
        else if(snake->checkPosInBody(head.x+1, head.y))
            snakeLeft = 1;
    }

    
    float normHeadX = head.x / width, normHeadY = head.y / height;
    float normAppleX = appleX / width, normAppleY = appleY / height;

    float appleAbove, appleBelow, appleLeft, appleRight;

    if(head.x > appleX)
        appleLeft = 1;
    else if(head.x < appleX)
        appleRight = 1;

    if(head.y > appleY)
        appleAbove = 1;
    else if(head.y < appleY)
        appleBelow = 1;

    return Matrix({dir0, dir1, dir2, dir3, normHeadX, normHeadY, normAppleX, normAppleY, 
                   snakeFront, snakeLeft, snakeRight, appleAbove, appleBelow, appleLeft, appleRight}, 1, 15);
}

void Game::update(){
    if(started){
        if(snake->updateSnake(width, height, appleX, appleY)){
            newApple();
            updateScore(snake->getScore());
        }

        if(snake->getAlive() == false)
            running = false;
    }
}

void Game::updateScore(int score){
    TTF_Font *font = TTF_OpenFont("../font/PublicPixel.ttf", TEXT_SIZE);
    const char* textString = (std::string("Score: ") + std::to_string(score)).c_str();
    SDL_Surface *surface = TTF_RenderText_Solid(font, textString, {TEXT_COLOR});
    text = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void Game::pollEvents(){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT)
            running = false;
        if(event.type == SDL_KEYDOWN && started == false)
            started = true;
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_RIGHT]){
        changeSnakeDir(0);
    }else if(state[SDL_SCANCODE_UP]){
        changeSnakeDir(1);
    }else if(state[SDL_SCANCODE_LEFT]){
        changeSnakeDir(2);
    }else if(state[SDL_SCANCODE_DOWN]){
        changeSnakeDir(3);
    }
}

void Game::render(){
    SDL_SetRenderDrawColor(renderer, BG_COLOR);
    SDL_RenderClear(renderer);

    int xSize = WIN_WIDTH/width;
    int ySize = WIN_HEIGHT/height;

    SDL_SetRenderDrawColor(renderer, GRID_COLOR);
    for(int i = 0; i <= WIN_WIDTH; i += xSize){
        SDL_RenderDrawLine(renderer, i, 0, i, WIN_HEIGHT);
    }
    for(int i = 0; i <= WIN_HEIGHT; i += ySize){
        SDL_RenderDrawLine(renderer, 0, i, WIN_WIDTH, i);
    }

    SDL_SetRenderDrawColor(renderer, SNAKE_COLOR);
    std::vector<pos> body = snake->getBody();
    for(auto position = body.cbegin(); position != body.cend(); ++position){
        SDL_Rect rect{.x=(*position).x*xSize+1, .y=(*position).y*ySize+1, .w=xSize-1, .h=ySize-1};
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, APPLE_COLOR);
    SDL_Rect rect{.x=appleX*xSize+1, .y=appleY*ySize+1, .w=xSize-1, .h=ySize-1};
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

    rect = {.x=0, .y=ySize/4, .w=10*TEXT_SIZE, .h=TEXT_SIZE};
    SDL_RenderCopy(renderer, text, NULL, &rect);

    SDL_RenderPresent(renderer);
}

void Game::quit(){
    delete snake;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(text);
    TTF_Quit();
    SDL_Quit();
}
