#include "..\include\nn.hpp"

Matrix sigmoid(Matrix x){
    auto func = [](float x){return 1/(1 + (float)exp(-x));};
    return x.applyFunction(func);
}

Matrix relu(Matrix x){
    auto func = [](float x){return x > 0 ? x : 0;};
    return x.applyFunction(func);
}

Matrix softmax(Matrix x){
    auto func = [](float x){return (float) exp(x);};
    x.applyFunction(func);
    float expSum = x.sum();
    return x / expSum;
}

NeuralNetwork::NeuralNetwork(int layers, std::vector<size_t> sizes, bool addBias){
    if(layers != (int) sizes.size())
        throw std::invalid_argument("Number of layers must match the vector of sizes");

    numLayers = layers;
    layerSizes = sizes;

    for(auto idx=layerSizes.cbegin(); idx != layerSizes.cend()-1; ++idx){
        if(addBias)
            weights.push_back(new Matrix((*idx)+1, *(idx+1), "random"));
        else
            weights.push_back(new Matrix(*idx, *(idx+1), "random"));
        activations.push_back(relu);
    }
}


NeuralNetwork::NeuralNetwork(int layers, std::vector<size_t> sizes, std::vector<Matrix(*)(Matrix)> act, bool addBias){
    if(layers != (int) sizes.size() || layers != (int) act.size() + 1)
        throw std::invalid_argument("Number of layers must match the vectors' sizes");

    numLayers = layers;
    layerSizes = sizes;

    for(auto idx=layerSizes.cbegin(); idx != layerSizes.cend()-1; ++idx){
        if(addBias)
            weights.push_back(new Matrix((*idx)+1, *(idx+1), "random"));
        else
            weights.push_back(new Matrix(*idx, *(idx+1), "random"));
    }   

    for(auto activation=act.begin(); activation != act.end(); ++activation){
        activations.push_back(*activation);
    }
}


NeuralNetwork::NeuralNetwork(int layers, std::vector<size_t> sizes, std::vector<Matrix> w, std::vector<Matrix(*)(Matrix)> act){
    if(layers != (int) sizes.size() || layers != (int) act.size() + 1 || layers != (int) w.size() + 1)
        throw std::invalid_argument("Number of layers must match the vectors' sizes");

    numLayers = layers;
    layerSizes = sizes;

    for(auto weight=w.begin(); weight != w.end(); ++weight){
        weights.push_back(new Matrix(*(weight->getValues()), weight->numRows(), weight->numCols()));
    }   

    for(auto activation=act.begin(); activation != act.end(); ++activation){
        activations.push_back(*activation);
    }
}

NeuralNetwork::~NeuralNetwork(){}


void NeuralNetwork::printWeights(){
    std::cout << std::endl << std::endl << "Number of layers: " << numLayers << std::endl << "Layer sizes: ";
    for(auto size = layerSizes.begin(); size != layerSizes.end(); ++size){
        if(size != layerSizes.end()-1)
            std::cout << *size << ", ";
        else
            std::cout << *size << std::endl;
    }
    for(auto weight = weights.begin(); weight != weights.end(); ++weight){
        (*weight)->print();
    }
}

Matrix NeuralNetwork::feedforward(Matrix input, bool addBias){
    for(size_t idx = 0; idx < weights.size(); idx++){
        if(addBias)
            input = input.append(Matrix({1}, 1, 1), false);
        input *= *(weights.at(idx));
        input = activations[idx](input);
    }
    return input;
}

void NeuralNetwork::saveToFile(std::string path){} // TODO

void NeuralNetwork::assignNN(NeuralNetwork nn){
    numLayers = nn.getNumLayers();
    layerSizes = nn.getLayers();
    activations = nn.getActivations();

    for(auto i=0; i < numLayers - 1; i++){
        *(weights[i]) = *(nn.getWeights()[i]);
    }
}

void NeuralNetwork::operator=(NeuralNetwork nn){
    this->assignNN(nn);
}

int NeuralNetwork::getNumLayers(){return numLayers;}
std::vector<size_t> NeuralNetwork::getLayers(){return layerSizes;}
std::vector<Matrix*> NeuralNetwork::getWeights(){return weights;}
std::vector<Matrix(*)(Matrix)> NeuralNetwork::getActivations(){return activations;}
