#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <cmath>
#include "matrix.hpp"


typedef Matrix<float> (*activation_t) (Matrix<float>);
typedef Matrix<float> (*act_deriv_t)  (Matrix<float>, Matrix<float>);


/* Activation functions */
inline Matrix<float> sigmoid(Matrix<float> x){
    auto func = [](float x){return 1/(1 + (float)exp(-x));};
    return x.applyFunction(func);
}

inline Matrix<float> relu(Matrix<float> x){
    auto func = [](float x){return x > 0 ? x : 0;};
    return x.applyFunction(func);
}

inline Matrix<float> softmax(Matrix<float> x){
    auto exp_ = [](float x){return (float) exp(x);};
    x = x.applyFunction(exp_);                           // VERIFICAR SE ESTÁ FUNCIONANDO
    auto func = [](Matrix<float> x){return x/x.sum();};
    return x.applyFunction(func, ROW_WISE);
}


/* Activation functions derivatives (error propagators) */
inline Matrix<float> sigmoidDerivative(Matrix<float> x, Matrix<float> error){
    auto func = [](float x){return 1/(1 + (float)exp(-x));};
    Matrix<float> sig = x.applyFunction(func);
    return sig.multiply(sig*(-1) + 1).multiply(error);
}

inline Matrix<float> reluDerivative(Matrix<float> x, Matrix<float> error){
    auto func = [](float x){return x >= 0 ? (float) 1 : 0;};
    return x.applyFunction(func).multiply(error);
}

inline Matrix<float> softmaxDerivative(Matrix<float> x, Matrix<float> error){
    Matrix<float> y = softmax(x), aux, result;
    aux = y.multiply(error);
    result = aux - y.multiply(aux.sum(ROW_WISE));
    return result;
}

#endif
