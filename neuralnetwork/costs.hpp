#ifndef COSTS_H
#define COSTS_H

#include <cmath>
#include "matrix.hpp"


typedef float (*cost_t) (Matrix<float>, Matrix<float>);
typedef Matrix<float> (*cost_deriv_t) (Matrix<float>, Matrix<float>);


/* Cost functions */
inline float crossEntropy(Matrix<float> y, Matrix<float> target){
    auto log_ = [](float x){return (float) log(x);};
    return target.multiply(y.applyFunction(log_)).sum() * -1;
}

inline float MSE(Matrix<float> y, Matrix<float> target){
    auto sqr_ = [](float x){return (float) pow(x, 2);};
    return (target - y).applyFunction(sqr_).sum()/y.numRows();
}


/* Cost functions derivatives (error propagators) */
inline Matrix<float> crossEntropyDerivative(Matrix<float> y, Matrix<float> target){
    auto _x = [](float x){return 1/x;};
    return target.multiply(y.applyFunction(_x)) * -1;
}

inline Matrix<float> MSEDerivative(Matrix<float> y, Matrix<float> target){
    return ((target - y)*2).multiply(y)*(-1/y.numRows());
}

#endif
