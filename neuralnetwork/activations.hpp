#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include <cmath>
#include "matrix.hpp"

#define DEFINED_FUNCTIONS 3

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
    x = x.applyFunction(exp_);
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

typedef struct func_n {activation_t func; std::string funcName;} func_n;
typedef struct derv_n {act_deriv_t func;  std::string dervName;} derv_n;

static struct func_n listFunc[DEFINED_FUNCTIONS] = {{sigmoid, "sigmoid"},
                                                    {relu,    "relu"},
                                                    {softmax, "softmax"}};

static struct derv_n listDerv[DEFINED_FUNCTIONS] = {{sigmoidDerivative, "sigmoid"},
                                                    {reluDerivative,    "relu"},
                                                    {softmaxDerivative, "softmax"}};

inline std::string funcToStr(activation_t func){
    for(int i = 0; i < DEFINED_FUNCTIONS; i++){
        if(func == listFunc[i].func){
            return listFunc[i].funcName;
        }
    }
    return "None";
}

inline activation_t strToFunc(std::string funcStr){
    for(int i = 0; i < DEFINED_FUNCTIONS; i++){
        if(funcStr == listFunc[i].funcName){
            return listFunc[i].func;
        }
    }
    return nullptr;
}

inline act_deriv_t strToDerivative(std::string funcStr){
    for(int i = 0; i < DEFINED_FUNCTIONS; i++){
        if(funcStr == listFunc[i].funcName){
            return listDerv[i].func;
        }
    }
    return nullptr;
}

#endif
