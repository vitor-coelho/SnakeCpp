#ifndef GEN_ALGO_H
#define GEN_ALGO_H

#include <vector>
#include <algorithm>
#include "..\neuralnetwork\matrix.hpp"
#include "..\neuralnetwork\nn.hpp"

#define NUM_LAYERS  3
#define LAYER1      (size_t) 8, (size_t)  16, sigmoid, sigmoidDerivative
#define LAYER2      (size_t) 16, (size_t)  4, softmax, softmaxDerivative

#define NUM_GENERATIONS 1000
#define POPULATION_SIZE 200
#define MAX_TIME_WO_APPLE 15*15 + 1

#define MUTATE_CHANCE 0.2
#define CROSSOVER_CHANCE 0.1


typedef struct individual_t{
    NeuralNetwork* genome;
    float fitness;

    //~individual_t(){delete genome;}
} individual_t;


class GenAlgo{
    private:
        int populationSize;
        std::vector<individual_t> population;

    public:
        GenAlgo(int popSize);
        GenAlgo(std::vector<individual_t> initPopulation);
        ~GenAlgo();

        std::vector<individual_t> getPopulation();

        void setFitness(std::vector<float> fitness);
        void resetPopulationList();
        void appendRandomIndividuals(int numNewIndividuals);
        void appendIndividuals(std::vector<individual_t> newIndividuals);
        std::vector<individual_t> crossover(individual_t individual1, individual_t individual2, float chance);
        void mutate(individual_t* individual);
        std::vector<individual_t> getBestIndividuals(int numIndividuals);

        float runGeneration(std::vector<float> newFitness);
};

#endif
