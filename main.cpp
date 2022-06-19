#include <SDL2/SDL.h>
#include "include/game.hpp"
#include "include/gen-algo.hpp"

#define GAME_WIDTH  15
#define GAME_HEIGHT 15

#define FPS 15

#define SHOW false

using namespace std;

void optimizeAI(bool show);
int  gameAI(individual_t individual, bool show);
void gameHuman();


int main(int argv, char** args){
    
    optimizeAI(SHOW);

    return 0;    
}

void optimizeAI(bool show){
    GenAlgo optimizer(POPULATION_SIZE);
    int generation = NUM_GENERATIONS;

    std::vector<individual_t> population;
    std::vector<float> fitness;

    while(generation--){
        cout << "Generation: " << generation + 1 << endl;

        population = optimizer.getPopulation();
        fitness.clear();

        for(auto individual = population.begin(); individual != population.end(); ++individual){
            fitness.push_back(gameAI(*individual, show));
        }

        optimizer.runGeneration(fitness);
    }

    for(auto individual = population.begin(); individual != population.end(); ++individual){
        fitness.push_back(gameAI(*individual, true));
    }
}

int gameAI(individual_t individual, bool show){
    Game game(GAME_WIDTH, GAME_HEIGHT, show, false);

    while(game.isRunning()){
        Matrix input = game.getSnakeInputs();

        Matrix output = individual.genome->feedforward(input);

        int newDir = individual.genome->maxOutput(output);

        game.changeSnakeDir(newDir);
        
        game.update();

        if(show){
            game.render();
            SDL_Delay(200);
        }
    }

    game.quit();

    return game.getSnakeScore();
}

void gameHuman(){
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
}




