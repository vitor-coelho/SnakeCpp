#include <SDL2/SDL.h>
#include "include/game.hpp"
#include "include/gen-algo.hpp"
#include "neuralnetwork\dataset.hpp"

#define GAME_WIDTH  12
#define GAME_HEIGHT 12

#define FPS 15

#define SHOW true

using namespace std;

void optimizeAI(bool show);
float gameAI(individual_t individual, bool show);
void gameHuman();
void perfectGame();


int main(int argv, char** args){
    
    //optimizeAI(SHOW);
    //gameHuman();
    perfectGame();

    return 0;
}

void optimizeAI(bool show){
    GenAlgo optimizer(POPULATION_SIZE);
    int generation = NUM_GENERATIONS;

    std::vector<individual_t> population;
    std::vector<float> fitness;

    float maxFitness;

    while(generation--){
        cout << "Generation " << generation + 1 << " | ";

        population = optimizer.getPopulation();
        fitness.clear();

        for(auto individual = population.begin(); individual != population.end(); ++individual){
            fitness.push_back(gameAI(*individual, show));
        }

        maxFitness = optimizer.runGeneration(fitness);
        cout << "Max fitness = " << maxFitness << endl;
    }

    population = optimizer.getBestIndividuals(10);

    for(auto individual = population.begin(); individual != population.end(); ++individual){
        gameAI(*individual, true);
    }
}

float gameAI(individual_t individual, bool show){
    Game game(GAME_WIDTH, GAME_HEIGHT, show, false);

    int snakeScore = game.getSnakeScore(), iterWithoutApple = 0;

    while(game.isRunning()){
        Matrix<float> input = game.getSnakeInputs();

        Matrix<float> output = individual.genome->feedforward(input);

        int newDir = output.maxIdx();

        game.changeSnakeDir(newDir);
        
        game.update();

        if(snakeScore < game.getSnakeScore()){
            snakeScore = game.getSnakeScore();
            iterWithoutApple = 0;
        }else{
            iterWithoutApple++;
        }

        if(iterWithoutApple > MAX_TIME_WO_APPLE){
            break;
        }

        if(show){
            game.render();
            SDL_Delay(50);
        }
    }

    float fitness = game.getSnakeFitness();

    //cout << "Fitness: " << fitness << endl;

    game.quit();

    return fitness;
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

void perfectGame(){
    Game game(GAME_WIDTH, GAME_HEIGHT, SHOW, false);
    Matrix<float> inputData, outputData;

    while(game.isRunning()){
        Matrix<float> input = game.getSnakeInputs();

        int newDir = game.getSnakePerfectMove();
        game.changeSnakeDir(newDir);

        Matrix<float> move(1,4);
        move.set(1, 0, newDir);

        inputData = inputData.append(input, ROW);
        outputData = outputData.append(move, ROW);

        game.update();

        if(SHOW){
            game.render();
            SDL_Delay(1);
        }
    }

    game.quit();

    writeMatrixToCsv(inputData, "input.csv");
    writeMatrixToCsv(outputData, "output.csv");
}
