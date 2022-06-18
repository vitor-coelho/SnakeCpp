#include <SDL2/SDL.h>
#include "include/game.hpp"

#define GAME_WIDTH  15
#define GAME_HEIGHT 15

#define FPS 15

using namespace std;


int main(int argv, char** args){
    
    Game game(GAME_WIDTH, GAME_HEIGHT, true, true);
    
    Uint32 timing;

    while(game.isRunning()){
        timing = SDL_GetTicks();
        game.pollEvents();
        game.update();
        game.render();
        timing = SDL_GetTicks() - timing;

        if(1000/FPS > timing)
            SDL_Delay(1000/FPS - timing);
    }

    SDL_Delay(3000);
    game.quit();    

    return 0;    
}
