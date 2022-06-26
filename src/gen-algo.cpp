#include "..\include\gen-algo.hpp"

// TODO: permitir criar populações com redes neurais diferentes (número de layers, ativações)

std::mt19937 generator(time(NULL));
std::normal_distribution<float> distribution(0, 1);
std::uniform_real_distribution<float> chance_dist(0, 1);

GenAlgo::GenAlgo(int popSize){
    populationSize = popSize;

    for(int i = 0; i < populationSize; i++){
        population.push_back({new NeuralNetwork(NUM_LAYERS, {new FCLayer(LAYER1), new FCLayer(LAYER2)}), 0});
    }
}

GenAlgo::GenAlgo(std::vector<individual_t> initPopulation){
    populationSize = initPopulation.size();
    population.assign(initPopulation.begin(), initPopulation.end());
}

GenAlgo::~GenAlgo(){}

std::vector<individual_t> GenAlgo::getPopulation(){
    return population;
}

void GenAlgo::resetPopulationList(){
    population.clear();
}

void GenAlgo::appendRandomIndividuals(int numNewIndividuals){
    for(int i = 0; i < numNewIndividuals; i++){
        individual_t newIndividual = {new NeuralNetwork(NUM_LAYERS, {new FCLayer(LAYER1), new FCLayer(LAYER2)}), 0};
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

    offspring1.genome = new NeuralNetwork(NUM_LAYERS, {new FCLayer(LAYER1), new FCLayer(LAYER2)});
    offspring2.genome = new NeuralNetwork(NUM_LAYERS, {new FCLayer(LAYER1), new FCLayer(LAYER2)});

    *(offspring1.genome) = *(individual1.genome);
    *(offspring2.genome) = *(individual2.genome);

    auto layers1 = offspring1.genome->getLayers();
    auto layers2 = offspring2.genome->getLayers();
    
    for(auto layerIdx = 0; layerIdx < NUM_LAYERS - 1; layerIdx++){
        for(size_t colIdx = 0; colIdx < layers1.at(layerIdx)->getOutputSize(); colIdx++){
            if(chance_dist(generator) < chance){
                auto col1 = layers1.at(layerIdx)->getWeights()(colIdx, COLUMN);
                auto col2 = layers2.at(layerIdx)->getWeights()(colIdx, COLUMN);

                layers1.at(layerIdx)->setWeights(layers1.at(layerIdx)->getWeights().del(colIdx, colIdx, COLUMN));
                layers2.at(layerIdx)->setWeights(layers2.at(layerIdx)->getWeights().del(colIdx, colIdx, COLUMN));

                layers1.at(layerIdx)->setWeights(layers1.at(layerIdx)->getWeights().insert(col2, colIdx, COLUMN));
                layers2.at(layerIdx)->setWeights(layers2.at(layerIdx)->getWeights().insert(col1, colIdx, COLUMN));
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
        numRows = individual->genome->getLayers().at(i)->getInputSize();
        numCols = individual->genome->getLayers().at(i)->getOutputSize();
        Matrix<float> rndMatrix(numRows+1, numCols, distribution);

        individual->genome->getLayers().at(i)->getWeights() += rndMatrix; // test
    }

    // Pode ser de multiplicação (TODO)
    // Pode ser de setar um valor aleatório novo (TODO)
    // Pode ser de mutar apenas partes dos valores (pode ser colunas) (TODO)
}

std::vector<individual_t> GenAlgo::getBestIndividuals(int numIndividuals){
    std::sort(population.begin(), population.end(),[](auto ind1, auto ind2){return ind1.fitness > ind2.fitness;});

    std::vector<individual_t> selected(numIndividuals);
    std::copy(population.begin(), population.begin()+numIndividuals, selected.begin());
    return selected;
}

void GenAlgo::setFitness(std::vector<float> newFitness){
    int idx = 0;
    for(auto individual = population.begin(); individual != population.end(); ++individual)
        individual->fitness = newFitness[idx++];
}

float GenAlgo::runGeneration(std::vector<float> newFitness){
    this->setFitness(newFitness);

    std::vector<individual_t> newGeneration = this->getBestIndividuals(10*populationSize/100);

    float maxFitness = newGeneration.at(0).fitness;

    this->resetPopulationList();

    this->appendIndividuals(newGeneration);

    // Add offspring resulting from crossover of the most fit
    for(size_t i = 0; i < newGeneration.size(); i++){
        for(size_t j = 0; j < newGeneration.size(); j++){
            if(i >= j)
                continue;

            std::vector<individual_t> offspring = this->crossover(newGeneration[i], newGeneration[j], CROSSOVER_CHANCE);
            this->appendIndividuals(offspring);

            if(population.size() >= 0.8*populationSize)
                break; // 80% of the pop. size is the maximum
        }
        if(population.size() >= 0.8*populationSize)
                break;
    }

    this->appendRandomIndividuals(populationSize - population.size());
    
    for(auto ind = population.begin(); ind != population.end(); ++ind){
        if(chance_dist(generator) < MUTATE_CHANCE){
            this->mutate(&*ind);
        }
    }

    return maxFitness;
}
