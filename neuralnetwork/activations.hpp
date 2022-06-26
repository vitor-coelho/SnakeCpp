#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <cmath>
#include "matrix.hpp"

typedef Matrix<float> (*activation_t) (Matrix<float>);

inline Matrix<float> sigmoid(Matrix<float> x){
    auto func = [](float x){return 1/(1 + (float)exp(-x));};
    return x.applyFunction(func);
}

inline Matrix<float> relu(Matrix<float> x){
    auto func = [](float x){return x > 0 ? x : 0;};
    return x.applyFunction(func);
}

inline Matrix<float> softmax(Matrix<float> x){
    auto func = [](float x){return (float) exp(x);};
    x = x.applyFunction(func);
    float expSum = x.sum();
    return x / expSum;
}

#endif
