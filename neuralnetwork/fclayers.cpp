#include "layers.hpp"
#include "activations.hpp"

std::normal_distribution<float> dist(0,1);

FCLayer::FCLayer(size_t inputLen, size_t outputLen, activation_t act, bool addBias){
    inputSize = inputLen;
    outputSize = outputLen;
    activation = act;
    bias = addBias;

    size_t rows = 0;
    bias ? rows = inputSize + 1 : rows = inputSize;

    weights = Matrix<float>(rows, outputSize, dist);
}

Matrix<float> FCLayer::feedforward(Matrix<float> input){
    if(bias)
        input = input.insert(Matrix<float>(1, 1, (std::string) "ones"), 0, COLUMN);  // Add bias
    return activation(input * weights);
}

void FCLayer::print(){std::cout << ">> Fully connected layer"; weights.print();}

void FCLayer::saveToFile(){}

size_t FCLayer::getInputSize(){return inputSize;}

size_t FCLayer::getOutputSize(){return outputSize;}

Matrix<float> FCLayer::getWeights(){return weights;}

activation_t FCLayer::getActivation(){return activation;}

void FCLayer::setWeights(Matrix<float> newWeights){weights = newWeights;}

void FCLayer::setActivation(activation_t act){activation = act;}
