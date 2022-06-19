#include "gen-algo.hpp"

#define NUM_LAYERS  3
#define LAYERS      5,10,4
#define ACTIVATIONS relu,softmax

// TODO: permitir criar populações com redes neurais diferentes (número de layers, ativações)

std::mt19937 generator(time(NULL));
std::uniform_real_distribution<float> distribution(0, 1);

GenAlgo::GenAlgo(int popSize){
    populationSize = popSize;

    for(int i = 0; i < populationSize; i++){
        population.push_back({new NeuralNetwork(NUM_LAYERS, {LAYERS}, {ACTIVATIONS}, true), 0});
    }
}

GenAlgo::GenAlgo(std::vector<individual_t> initPopulation){
    populationSize = initPopulation.size();
    population.assign(initPopulation.begin(), initPopulation.end());
}

GenAlgo::~GenAlgo(){}

void GenAlgo::resetPopulationList(){
    population.clear();
}

void GenAlgo::appendRandomIndividuals(int numNewIndividuals){
    for(int i = 0; i < numNewIndividuals; i++){
        individual_t newIndividual = {new NeuralNetwork(NUM_LAYERS, {LAYERS}, {ACTIVATIONS}), 0};
        population.push_back(newIndividual);
    }
}

void GenAlgo::appendIndividuals(std::vector<individual_t> newIndividuals){
    population.insert(population.end(), newIndividuals.begin(), newIndividuals.end());
}

std::vector<individual_t> GenAlgo::crossover(individual_t individual1, individual_t individual2, float chance){
    // Realizar a troca de um ou mais neurônios (a troca deve ser em coluna para realizar a ativação de um neurônio)
    // Criar vetor de 0 ou 1 (um para cada coluna), para decidir se haverá a troca de genes ou não

    individual_t offspring1, offspring2;

    offspring1.genome = new NeuralNetwork(NUM_LAYERS, {LAYERS}, {ACTIVATIONS}, true);
    offspring2.genome = new NeuralNetwork(NUM_LAYERS, {LAYERS}, {ACTIVATIONS}, true);

    *(offspring1.genome) = *(individual1.genome);
    *(offspring2.genome) = *(individual2.genome);

    for(int nnIdx = 0; nnIdx < individual1.genome->getNumLayers()-1; nnIdx++){
        for(size_t col = 0; col < individual1.genome->getWeights().at(nnIdx)->numCols(); col++){
            
            if(true){ // distribution(generator) < chance
                // Crossover
                offspring1.genome->getWeights().at(nnIdx)->del(col, col, false);
                offspring2.genome->getWeights().at(nnIdx)->del(col, col, false);

                offspring1.genome->getWeights().at(nnIdx)->insert((*individual2.genome->getWeights().at(nnIdx))(col,false), col, false);
                offspring2.genome->getWeights().at(nnIdx)->insert((*individual1.genome->getWeights().at(nnIdx))(col,false), col, false);
            }

        }
    }
    return std::vector<individual_t> {offspring1, offspring2};
}

void GenAlgo::mutate(individual_t* individual){
    // Pode ser de adição (como está abaixo)
    int numLayers = individual->genome->getNumLayers() - 1;
    size_t numRows, numCols;

    for(int i = 0; i < numLayers; i++){
        numRows = individual->genome->getWeights().at(i)->numRows();
        numCols = individual->genome->getWeights().at(i)->numCols();
        Matrix rndMatrix(numRows, numCols, "random");

        individual->genome->getWeights().at(i)->add(rndMatrix);
    }

    // Pode ser de multiplicação (TODO)
    // Pode ser de setar um valor aleatório novo (TODO)
    // Pode ser de mutar apenas partes dos valores (pode ser colunas) (TODO)
}

std::vector<individual_t> GenAlgo::getBestIndividuals(int numIndividuals){
    std::sort(population.begin(), population.end(),[](auto ind1, auto ind2){return ind1.fitness < ind2.fitness;});

    std::vector<individual_t> selected(numIndividuals);
    std::copy(population.begin(), population.begin()+numIndividuals, selected.begin());
    return selected;
}

void GenAlgo::setFitness(std::vector<float> newFitness){
    int idx = 0;
    for(auto individual = population.begin(); individual != population.end(); ++individual)
        individual->fitness = newFitness[idx++];
}

void GenAlgo::runGeneration(std::vector<float> newFitness){
    this->setFitness(newFitness);

    std::vector<individual_t> newGeneration = this->getBestIndividuals(10*populationSize/100);

    this->resetPopulationList();

    // Add offspring resulting from crossover of the most fit
    for(size_t i = 0; i < newGeneration.size(); i++){
        for(size_t j = 0; j < newGeneration.size(); j++){
            if(i >= j)
                continue;

            std::vector<individual_t> offspring = this->crossover(newGeneration[i], newGeneration[j], 0.2);
            this->appendIndividuals(offspring);

            if(population.size() >= 0.5*populationSize)
                break; // Half of the pop. size is the maximum
        }
        if(population.size() >= 0.5*populationSize)
                break;
    }

    this->appendRandomIndividuals(populationSize - population.size());
    
    for(auto ind = population.begin(); ind != population.end(); ++ind){
        if(true){ //distribution(generator) < 0.2
            this->mutate(&*ind);
        }
    }
}
