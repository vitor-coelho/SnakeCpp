#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2\SDL_ttf.h>
#include <random>
#include <time.h>
//#include <string>
#include "snake.hpp"

#define WIN_INIT_X 100
#define WIN_INIT_Y 50
#define WIN_WIDTH  600
#define WIN_HEIGHT 600

#define TEXT_SIZE 12

#define GRID_COLOR  255,255,255,255
#define BG_COLOR      0,  0,  0,255
#define SNAKE_COLOR   0,255,  0,255
#define APPLE_COLOR 255,  0,  0,255
#define TEXT_COLOR  255,255,255,255


class Game{
    private:
        SDL_Window   * window;
        SDL_Renderer * renderer;
        SDL_Texture  * text;

        Snake * snake;

        int width, height;

        int appleX, appleY;

        bool show;
        bool running = true;
        bool humanControl;
        bool started = true;
    
    public:
        Game(int w, int h, bool toShow, bool human);
        ~Game();

        bool isRunning();

        void newApple();

        void changeSnakeDir(int newDir);

        void pollEvents();
        void update();
        void updateScore(int score);
        void render();
        void quit();
};


#endif