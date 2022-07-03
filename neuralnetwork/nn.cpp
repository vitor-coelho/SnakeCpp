#include "nn.hpp"

NeuralNetwork::NeuralNetwork(size_t numberOfLayers, std::vector<Layer*> NNLayers){
    if(numberOfLayers != NNLayers.size() + 1)
        throw std::invalid_argument("Number of layers do not match");

    numLayers = numberOfLayers;
    inputSize = NNLayers.at(0)->getInputSize();
    outputSize = NNLayers.at(numLayers-2)->getOutputSize();
    layers.assign(NNLayers.begin(), NNLayers.end());
}

NeuralNetwork::~NeuralNetwork(){}


void NeuralNetwork::printInfo(){
    std::cout << std::endl << std::endl << "Number of layers: " << numLayers << "\n\n*** Layers ***\n\n";
    for(auto layer = layers.begin(); layer != layers.end(); ++layer)
        (*layer)->print();
}

int NeuralNetwork::getNumLayers(){return numLayers;}
size_t NeuralNetwork::getInputSize(){return inputSize;}
size_t NeuralNetwork::getOutputSize(){return outputSize;}
std::vector<Layer*> NeuralNetwork::getLayers(){return layers;}

void NeuralNetwork::assignNN(NeuralNetwork nn){ // Test references
    numLayers = nn.getNumLayers();
    inputSize = nn.inputSize;
    outputSize = nn.outputSize;

    layers.resize(nn.getLayers().size());
    for(size_t idx = 0; idx < layers.size(); idx++){
        layers.at(idx)->setWeights(nn.getLayers().at(idx)->getWeights());
    }
}

void NeuralNetwork::operator=(NeuralNetwork nn){
    this->assignNN(nn);
}

Matrix<float> NeuralNetwork::feedforward(Matrix<float> input){
    for(auto layer = layers.begin(); layer != layers.end(); ++layer){
        input = (*layer)->feedforward(input);
    }
    return input;
}

float NeuralNetwork::trainBatch(Matrix<float> input, Matrix<float> target, float learningRate, cost_t costFunc, cost_deriv_t costDer){
    if(input.numRows() != target.numRows())
        throw std::invalid_argument("Input and targets have different number of rows");

    for(auto layer = layers.begin(); layer != layers.end(); ++layer){
        input = (*layer)->train(input);
    }

    float error = costFunc(input, target);
    Matrix<float> errorMatrix = costDer(input, target);

    for(auto layer = layers.rbegin(); layer != layers.rend(); ++layer){
        errorMatrix = (*layer)->backpropagate(errorMatrix, learningRate);
    }

    return error;
}

void NeuralNetwork::saveToFile(std::string path){} // TODO
