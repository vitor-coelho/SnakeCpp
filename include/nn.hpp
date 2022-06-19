#ifndef NN_H
#define NN_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include "matrix.hpp"

// Entrada: vetor linha (Matrix(1,n))

Matrix sigmoid(Matrix x);
Matrix relu(Matrix x);
Matrix softmax(Matrix x);

class NeuralNetwork{

    private:
        int numLayers;
        std::vector<size_t> layerSizes;
        std::vector<Matrix*> weights;
        std::vector<Matrix(*)(Matrix)> activations;

    public:
        NeuralNetwork(int layers, std::vector<size_t> sizes, bool addBias=true);
        NeuralNetwork(int layers, std::vector<size_t> sizes, std::vector<Matrix(*)(Matrix)> act, bool addBias=true);
        NeuralNetwork(int layers, std::vector<size_t> sizes, std::vector<Matrix> w,
                      std::vector<Matrix(*)(Matrix)> act);
        ~NeuralNetwork();

        void printWeights();

        int getNumLayers();
        std::vector<size_t> getLayers();
        std::vector<Matrix*> getWeights();
        std::vector<Matrix(*)(Matrix)> getActivations();

        void assignNN(NeuralNetwork nn);

        Matrix feedforward(Matrix input, bool addBias=true);
        int maxOutput(Matrix output);
        void saveToFile(std::string path);

        void operator=(NeuralNetwork nn);

};

#endif
