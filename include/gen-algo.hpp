#ifndef GEN_ALGO_H
#define GEN_ALGO_H

#include <vector>
#include <algorithm>
#include "matrix.hpp"
#include "nn.hpp"

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

        void setFitness(std::vector<float> fitness);
        void resetPopulationList();
        void appendRandomIndividuals(int numNewIndividuals);
        void appendIndividuals(std::vector<individual_t> newIndividuals);
        std::vector<individual_t> crossover(individual_t individual1, individual_t individual2, float chance);
        void mutate(individual_t* individual);
        std::vector<individual_t> getBestIndividuals(int numIndividuals);

        void runGeneration(std::vector<float> newFitness);
};

#endif
