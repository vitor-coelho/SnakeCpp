#ifndef NN_H
#define NN_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include "matrix.hpp"
#include "activations.hpp"
#include "layers.hpp"

// Entrada: vetor linha (Matrix(1,n))


class NeuralNetwork{

    private:
        size_t numLayers, inputSize, outputSize;
        std::vector<Layer*> layers;

    public:
        NeuralNetwork(size_t numberOfLayers, std::vector<Layer*> NNLayers);
        ~NeuralNetwork();

        void printInfo();

        int getNumLayers();
        size_t getInputSize();
        size_t getOutputSize();
        std::vector<Layer*> getLayers();

        void assignNN(NeuralNetwork nn);
        void operator=(NeuralNetwork nn);

        Matrix<float> feedforward(Matrix<float> input);
        void saveToFile(std::string path);
};

#endif
