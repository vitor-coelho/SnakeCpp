#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <stdexcept>
#include <string>
#include <time.h>
#include <algorithm>


#define ROW true
#define COLUMN false



/* Class definitions */
template <typename T> class Matrix{
    private:
        size_t rows, cols;
        std::vector<T> values;

    public:
        Matrix();
        Matrix(size_t numRows, size_t numCols, std::string fill="zeros");
        template<typename dist_t> Matrix(size_t numRows, size_t numCols, dist_t dist);
        Matrix(std::vector<T> vec, size_t numRows, size_t numCols);
        ~Matrix();

        void print();

        void set(T value, size_t row, size_t col);
        void set(std::vector<T> vec, size_t idx, bool row=true);
        void set(Matrix<T> matrix);

        bool equalTo(Matrix matrix);

        size_t numRows();
        size_t numCols();
        std::vector<T>* getValues();

        void setRows(size_t newRows);
        void setCols(size_t newCols);

        Matrix add(T value);
        Matrix add(Matrix matrix);
        Matrix subtract(T value);
        Matrix subtract(Matrix matrix);
        Matrix multiply(T value);
        Matrix multiply(Matrix matrix);
        Matrix divide(T value);
        T dotProduct(Matrix matrix);
        T sum();
        Matrix transpose();

        Matrix applyFunction(T function(T));
        Matrix insert(Matrix toAppend, size_t idx, bool row=true);
        Matrix append(Matrix toAppend, bool row=true);
        Matrix del(size_t startIdx, size_t endIdx, bool row=true);

        T max();
        T min();
        T mean();
        T median();

        size_t maxIdx();
        size_t minIdx();

        //Matrix crossProduct(Matrix matrix);   //TODO

        // Overloaded operators
        T operator()(size_t row, size_t col);
        Matrix operator()(size_t idx, bool row=true);
        Matrix operator()(size_t startRow, size_t endRow, size_t startCol, size_t endCol);

        void operator=(Matrix matrix);
        bool operator==(Matrix matrix);

        Matrix operator+(Matrix matrix);
        Matrix operator+(T value);
        Matrix operator-(Matrix matrix);
        Matrix operator-(T value);
        Matrix operator*(Matrix matrix);
        Matrix operator*(T value);
        Matrix operator/(T value);

        void operator++(int);
        void operator--(int);

        void operator+=(Matrix matrix);
        void operator+=(T value);
        void operator-=(Matrix matrix);
        void operator-=(T value);
        void operator*=(Matrix<T> matrix);
        void operator*=(T value);
        void operator/=(T value);
};



/* Random number generator utility */
inline std::mt19937& mt(){static std::mt19937 mt_gen(time(NULL)); return mt_gen;};


/* Constructors and destructor */
template <typename T>
Matrix<T>::Matrix(){
    this->rows = 1;
    this->cols = 1;
    this->values.resize(1, 0);    
}

template <typename T>
Matrix<T>::Matrix(size_t numRows, size_t numCols, std::string fill){
    this->rows = numRows;
    this->cols = numCols;

    if(fill == "zeros"){
        this->values.resize(numCols*numRows, 0);
    }else if(fill == "ones"){
        this->values.resize(numCols*numRows, 1);
    }else{
        throw std::invalid_argument("Invalid fill argument.");
    }
}

template <typename T>
template <typename dist_t>
Matrix<T>::Matrix(size_t numRows, size_t numCols, dist_t dist){
    this->rows = numRows;
    this->cols = numCols;

    this->values.resize(numCols*numRows, 0);
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set(dist(mt()), i, j);
        }
    }
}

template <typename T>
Matrix<T>::Matrix(std::vector<T> vec, size_t numRows, size_t numCols){
    this->rows = numRows;
    this->cols = numCols;
    this->values.assign(vec.begin(), vec.end());
}

template <typename T>
Matrix<T>::~Matrix(){}



/* Getters and Setters */
template <typename T>
size_t Matrix<T>::numRows(){
    return this->rows;
}

template <typename T>
size_t Matrix<T>::numCols(){
    return this->cols;
}

template <typename T>
std::vector<T>* Matrix<T>::getValues(){
    return &values;
}

template <typename T>
void Matrix<T>::setRows(size_t newRows){
    rows = newRows;
}

template <typename T>
void Matrix<T>::setCols(size_t newCols){
    cols = newCols;
}

template <typename T>
void Matrix<T>::set(T value, size_t row, size_t col){
    this->values.at(col + this->cols*row) = value;
}

template <typename T>
void Matrix<T>::set(std::vector<T> vec, size_t idx, bool row){
    if(row){
        for(size_t j = 0; j < this->cols; j++){
            this->set(vec.at(j), idx, j);
        }
    }else{
        for(size_t i = 0; i < this->rows; i++){
            this->set(vec.at(i), i, idx);
        }
    }  
}

template <typename T>
void Matrix<T>::set(Matrix matrix){
    this->rows = matrix.rows;
    this->cols = matrix.cols;
    this->values.assign(matrix.values.begin(), matrix.values.begin() + matrix.values.size());
}



/* Print matrix */
template <typename T>
void Matrix<T>::print(){
    std::cout << std::setprecision(5);
    std::cout << std::fixed;
    std::cout << "\n";
    for(size_t i = 0; i < this->rows; i++){
        std::cout << "|";
        for(size_t j = 0; j < this->cols; j++){
            std::cout << std::setw(10) << (*this)(i,j) << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "\n";
}



/* Equality comparison */
template <typename T>
bool Matrix<T>::equalTo(Matrix matrix){
    if(this->rows == matrix.rows && this->cols == matrix.cols){
        for(size_t i = 0; i < matrix.rows; i++){
            for(size_t j = 0; j < matrix.cols; j++){
                if((*this)(i,j) != matrix(i,j))
                    return false;
            }
        }
        return true;
    }else{
        return false;
    }
}



/* Matrix operations */
template <typename T>
Matrix<T> Matrix<T>::add(T value){
    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j) + value, i, j);
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::add(Matrix matrix){
    if(this->rows != matrix.rows || this->cols != matrix.cols)
        throw std::invalid_argument("Matrices must have the same format");

    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j) + matrix(i,j), i, j);
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::subtract(T value){
    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j) - value, i, j);
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::subtract(Matrix matrix){
    if(this->rows != matrix.rows || this->cols != matrix.cols)
        throw std::invalid_argument("Matrices must have the same format");

    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j) - matrix(i,j), i, j);
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::multiply(T value){
    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i, j)*value, i, j);
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::multiply(Matrix matrix){
    if(this->cols != matrix.rows)
        throw std::invalid_argument("First matrix column size must match second matrix row size");

    Matrix result(this->rows, matrix.cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < matrix.cols; j++){
            float dotProd = (*this)(i,true).dotProduct(matrix(j, false).transpose());
            result.set(dotProd, i, j);
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::divide(T value){
    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i, j)/value, i, j);
        }
    }
    return result;
}

template <typename T>
T Matrix<T>::dotProduct(Matrix matrix){
    if(this->rows != matrix.rows || this->cols != matrix.cols)
        throw std::invalid_argument("Matrices must have the same format");

    float dotProd = 0;
    for(size_t i = 0; i < matrix.rows; i++){
        for(size_t j = 0; j < matrix.cols; j++){
            dotProd += (*this)(i,j) * matrix(i,j);
        }
    }

    return dotProd;
}

template <typename T>
T Matrix<T>::sum(){
    float sum = 0;
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            sum += (*this)(i,j);
        }
    }
    return sum;
}

template <typename T>
Matrix<T> Matrix<T>::transpose(){
    Matrix result(this->cols, this->rows);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j), j, i);
        }
    }

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::applyFunction(T function(T)){
    Matrix result(this->rows, this->cols);

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set(function((*this)(i,j)), i, j);
        }
    }
    return result;
}



/* Inserting, appending and deleting */
template <typename T>
Matrix<T> Matrix<T>::insert(Matrix toAppend, size_t idx, bool row){
    Matrix result(*(this->getValues()), this->rows, this->cols);

    if(row){
        if(cols != toAppend.cols)
            throw std::invalid_argument("Matrices must have the same number of columns");

        result.getValues()->insert(result.getValues()->begin()+result.numCols()*idx, 
                                  (*toAppend.getValues()).begin(),
                                  (*toAppend.getValues()).end());

        result.rows = result.rows + toAppend.numRows();

    }else{
        if(rows != toAppend.rows)
            throw std::invalid_argument("Matrices must have the same number of rows");

        size_t end = result.getValues()->size();
        size_t numInserted = 0;

        for(size_t i = 0; i < end; i += cols){
            result.getValues()->insert(result.getValues()->begin() + i + numInserted + idx, toAppend.getValues()->at(i/cols));
            numInserted++;
        }

        result.cols = cols + toAppend.numCols();
    }
    
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::append(Matrix toAppend, bool row){
    Matrix result(*(this->getValues()), this->rows, this->cols);

    if(row)
        result = result.insert(toAppend, rows, true);
    else
        result = result.insert(toAppend, cols, false);
    
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::del(size_t startIdx, size_t endIdx, bool row){
    // Start idx and end idx are inclusive [startIdx, endIdx]
    Matrix result(*(this->getValues()), this->rows, this->cols);

    if(row){
        if(startIdx < 0 || startIdx >= result.rows || endIdx < 0 || endIdx >= result.rows || endIdx < startIdx)
            throw std::invalid_argument("Indexes out of range");

        result.values.erase(result.values.begin() + startIdx*result.cols, result.values.begin() + (endIdx+1)*result.cols);

        result.rows = rows - (endIdx - startIdx + 1);

    }else{
        if(startIdx < 0 || startIdx >= this->cols || endIdx < 0 || endIdx >= this->cols || endIdx < startIdx)
            throw std::invalid_argument("Indexes out of range");

        size_t end = result.values.size();
        size_t numDeleted = 0;
        for(size_t i = 0; i < end; i += cols){
            for(size_t col = startIdx; col <= endIdx; col++){
                result.values.erase(result.values.begin() + i + col - numDeleted);
                numDeleted++;
            }
        }

        result.cols = cols - (endIdx - startIdx + 1);
    }
    
    return result;
}



/* Statistics methods */
template <typename T>
T Matrix<T>::max(){
    T maxValue = (*this)((size_t) 0, (size_t) 0);
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            if(maxValue < (*this)(i,j))
                maxValue = (*this)(i,j);
        }
    }
    return maxValue;
}

template <typename T>
T Matrix<T>::min(){
    T minValue = (*this)((size_t) 0, (size_t) 0);
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            if(minValue > (*this)(i,j))
                minValue = (*this)(i,j);
        }
    }
    return minValue;
}

template <typename T>
T Matrix<T>::mean(){
    T meanValue = 0;
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            meanValue += (*this)(i,j);
        }
    }
    return meanValue / (this->rows*this->cols);
}

template <typename T>
T Matrix<T>::median(){
    std::vector<T> aux = *(this->getValues());
    std::sort(aux.begin(), aux.end());
    if(aux.size() % 2){
        return aux.at(aux.size()/2);
    }else{
        return (aux.at(aux.size()/2) + aux.at(aux.size()/2 - 1)) / 2;
    }
}

template <typename T>
size_t Matrix<T>::maxIdx(){
    T maxValue = (*this)((size_t) 0, (size_t) 0);
    size_t idx = 0;
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            if(maxValue < (*this)(i,j)){
                maxValue = (*this)(i,j);
                idx = i*this->cols + j;
            }
        }
    }
    return idx;
}

template <typename T>
size_t Matrix<T>::minIdx(){
    T minValue = (*this)((size_t) 0, (size_t) 0);
    size_t idx = 0;
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            if(minValue > (*this)(i,j)){
                minValue = (*this)(i,j);
                idx = i*this->cols + j;
            }
        }
    }
    return idx;
}



/* Overloaded operators */
template <typename T>
T Matrix<T>::operator()(size_t row, size_t col){
    return this->values.at(col + this->cols*row);
}

template <typename T>
Matrix<T> Matrix<T>::operator()(size_t idx, bool row){
    if(row){
        Matrix result(1, this->cols);
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(idx, j), 0, j);
        }
        return result;
    }else{
        Matrix result(this->rows, 1);
        for(size_t i = 0; i < this->rows; i++){
            result.set((*this)(i, idx), i, 0);
        }
        return result;
    }
}

template <typename T>
Matrix<T> Matrix<T>::operator()(size_t startRow, size_t endRow, size_t startCol, size_t endCol){
    Matrix result(endRow - startRow + 1, endCol - startCol + 1);

    for(size_t i = startRow; i < endRow + 1; i++){
        for(size_t j = startCol; j < endCol + 1; j++){
            result.set((*this)(i,j), i - startRow, j - startCol);
        }
    }
    
    return result;
}

template <typename T>
void Matrix<T>::operator=(Matrix matrix){this->set(matrix);}

template <typename T>
bool Matrix<T>::operator==(Matrix matrix){return this->equalTo(matrix);}

template <typename T>
Matrix<T> Matrix<T>::operator+(Matrix matrix){return this->add(matrix);}

template <typename T>
Matrix<T> Matrix<T>::operator+(T value)  {return this->add(value);}

template <typename T>
Matrix<T> Matrix<T>::operator-(Matrix matrix){return this->subtract(matrix);}

template <typename T>
Matrix<T> Matrix<T>::operator-(T value)  {return this->subtract(value);}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix matrix){return this->multiply(matrix);}

template <typename T>
Matrix<T> Matrix<T>::operator*(T value)  {return this->multiply(value);}

template <typename T>
Matrix<T> Matrix<T>::operator/(T value)  {return this->divide(value);}

template <typename T>
void Matrix<T>::operator++(int){(*this) = this->add(1);}

template <typename T>
void Matrix<T>::operator--(int){(*this) = this->subtract(1);}

template <typename T>
void Matrix<T>::operator+=(Matrix matrix){(*this) = this->add(matrix);}

template <typename T>
void Matrix<T>::operator+=(T value)  {(*this) = this->add(value);}

template <typename T>
void Matrix<T>::operator-=(Matrix matrix){(*this) = this->subtract(matrix);}

template <typename T>
void Matrix<T>::operator-=(T value)  {(*this) = this->subtract(value);}

template <typename T>
void Matrix<T>::operator*=(Matrix<T> matrix){(*this) = this->multiply(matrix);}

template <typename T>
void Matrix<T>::operator*=(T value)  {(*this) = this->multiply(value);}

template <typename T>
void Matrix<T>::operator/=(T value)  {(*this) = this->divide(value);}

#endif
