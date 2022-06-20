#ifndef GEN_ALGO_H
#define GEN_ALGO_H

#include <vector>
#include <algorithm>
#include "matrix.hpp"
#include "nn.hpp"

#define NUM_LAYERS  3
#define LAYERS      15,4,4
#define ACTIVATIONS relu,softmax

#define NUM_GENERATIONS 500
#define POPULATION_SIZE 1000
#define MAX_TIME_WO_APPLE 15*15 + 1

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
