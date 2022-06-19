#include "matrix.hpp"

std::mt19937 mt(time(NULL));
std::uniform_real_distribution<float> dist(-1, 1);


Matrix::Matrix(){
    this->rows = 1;
    this->cols = 1;
    this->values.resize(1, 0);    
}

Matrix::Matrix(size_t numRows, size_t numCols, std::string fill){
    this->rows = numRows;
    this->cols = numCols;

    if(fill == "zeros"){
        this->values.resize(numCols*numRows, 0);
    }else if(fill == "ones"){
        this->values.resize(numCols*numRows, 1);
    }else if(fill == "random"){
        this->values.resize(numCols*numRows, 0);
        for(size_t i = 0; i < this->rows; i++){
            for(size_t j = 0; j < this->cols; j++){
                this->set(dist(mt), i, j);
            }
        }
    }
}

Matrix::Matrix(std::vector<float> vec, size_t numRows, size_t numCols){
    this->rows = numRows;
    this->cols = numCols;
    this->values.assign(vec.begin(), vec.begin() + vec.size());
}

Matrix::~Matrix(){}

void Matrix::print(){
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

void Matrix::set(float value, size_t row, size_t col){
    this->values.at(col + this->cols*row) = value;
}

void Matrix::set(std::vector<float> vec, size_t idx, bool row){
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

void Matrix::set(Matrix matrix){
    this->rows = matrix.rows;
    this->cols = matrix.cols;
    this->values.assign(matrix.values.begin(), matrix.values.begin() + matrix.values.size());
}

bool Matrix::equalTo(Matrix matrix){
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

size_t Matrix::numRows(){
    return this->rows;
}

size_t Matrix::numCols(){
    return this->cols;
}

std::vector<float>* Matrix::getValues(){
    return &values;
}

void Matrix::setRows(size_t newRows){
    rows = newRows;
}

void Matrix::setCols(size_t newCols){
    cols = newCols;
}


Matrix Matrix::add(float value){
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i,j) + value, i, j);
        }
    }
    return (*this);
}

Matrix Matrix::add(Matrix matrix){
    if(this->rows != matrix.rows || this->cols != matrix.cols)
        throw std::invalid_argument("Matrices must have the same format");

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i,j) + matrix(i,j), i, j);
        }
    }
    return (*this);
}

Matrix Matrix::subtract(float value){
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i,j) - value, i, j);
        }
    }
    return (*this);
}

Matrix Matrix::subtract(Matrix matrix){
    if(this->rows != matrix.rows || this->cols != matrix.cols)
        throw std::invalid_argument("Matrices must have the same format");

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i,j) - matrix(i,j), i, j);
        }
    }
    return (*this);
}

Matrix Matrix::multiply(float value){
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i, j)*value, i, j);
        }
    }
    return (*this);
}

Matrix Matrix::divide(float value){
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set((*this)(i, j)/value, i, j);
        }
    }
    return (*this);
}

float Matrix::dotProduct(Matrix matrix){
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

float Matrix::sum(){
    float sum = 0;
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            sum += (*this)(i,j);
        }
    }
    return sum;
}

Matrix Matrix::transpose(){
    Matrix result(this->cols, this->rows, "zeros");

    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(i,j), j, i);
        }
    }

    return result;
}

Matrix Matrix::applyFunction(float function(float)){
    for(size_t i = 0; i < this->rows; i++){
        for(size_t j = 0; j < this->cols; j++){
            this->set(function((*this)(i,j)), i, j);
        }
    }
    return (*this);
}

Matrix Matrix::insert(Matrix toAppend, size_t idx, bool row){
    if(row){
        if(cols != toAppend.cols)
            throw std::invalid_argument("Matrices must have the same number of columns");

        (*this->getValues()).insert((*this->getValues()).begin()+this->numCols()*idx, 
                                 (*toAppend.getValues()).begin(),
                                 (*toAppend.getValues()).end());

        this->rows = rows + toAppend.numRows();

    }else{
        if(rows != toAppend.rows)
            throw std::invalid_argument("Matrices must have the same number of rows");

        size_t end = (*this->getValues()).size();
        size_t numInserted = 0;
        for(size_t i = 0; i < end; i += cols){
            (*this->getValues()).insert((*this->getValues()).begin() + i + numInserted + idx, (*toAppend.getValues()).at(i/cols));
            numInserted++;
        }

        this->cols = cols + toAppend.numCols();
    }
    
    return (*this);
}

Matrix Matrix::append(Matrix toAppend, bool row){
    if(row)
        (*this) = this->insert(toAppend, rows, true);
    else
        (*this) = this->insert(toAppend, cols, false);
    
    return (*this);
}

Matrix Matrix::del(size_t startIdx, size_t endIdx, bool row){
    // Start idx and end idx are inclusive [startIdx, endIdx]

    if(row){
        if(startIdx < 0 || startIdx >= this->rows || endIdx < 0 || endIdx >= this->rows || endIdx < startIdx)
            throw std::invalid_argument("Indexes out of range");

        this->values.erase(this->values.begin() + startIdx*this->cols, this->values.begin() + endIdx*this->cols);

        this->rows = rows - (endIdx - startIdx + 1);

    }else{
        if(startIdx < 0 || startIdx >= this->cols || endIdx < 0 || endIdx >= this->cols || endIdx < startIdx)
            throw std::invalid_argument("Indexes out of range");

        size_t end = this->values.size();
        size_t numDeleted = 0;
        for(size_t i = 0; i < end; i += cols){
            for(size_t col = startIdx; col <= endIdx; col++){
                this->values.erase(this->values.begin() + i + col - numDeleted);
                numDeleted++;
            }
        }

        this->cols = cols - (endIdx - startIdx + 1);
    }
    
    return (*this);
}


// Static operators

Matrix Matrix::multiply(Matrix leftMatrix, Matrix rightMatrix){
    
    if(leftMatrix.cols != rightMatrix.rows)
        throw std::invalid_argument("First matrix column size must match second matrix row size");

    Matrix result(leftMatrix.rows, rightMatrix.cols, "zeros");

    for(size_t i = 0; i < leftMatrix.rows; i++){
        for(size_t j = 0; j < rightMatrix.cols; j++){
            float dotProd = dotProduct(leftMatrix(i, true), rightMatrix(j, false).transpose());
            result.set(dotProd, i, j);
        }
    }

    return result;
}

Matrix Matrix::multiply(Matrix matrix, float value){ 
    Matrix result(matrix.rows, matrix.cols, "zeros");
    result.multiply(value);
    return result;
}

float Matrix::dotProduct(Matrix matrix1, Matrix matrix2){
    if(matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols)
        throw std::invalid_argument("Matrices must have the same format");

    float dotProd = matrix1.dotProduct(matrix2);
    return dotProd;
}


// TODO: develop the generalized cross product between matrixs
// Matrix Matrix::crossProduct(Matrix matrix){}
// Matrix Matrix::crossProduct(Matrix matrix1, Matrix matrix2){}


// Overloaded operators

float Matrix::operator()(size_t row, size_t col){
    return this->values.at(col + this->cols*row);
}

Matrix Matrix::operator()(size_t idx, bool row){
    
    if(row){
        Matrix result(1, this->cols, "zeros");
        for(size_t j = 0; j < this->cols; j++){
            result.set((*this)(idx, j), 0, j);
        }
        return result;
    }else{
        Matrix result(this->rows, 1, "zeros");
        for(size_t i = 0; i < this->rows; i++){
            result.set((*this)(i, idx), i, 0);
        }
        return result;
    }
}

void Matrix::operator=(Matrix matrix){this->set(matrix);}

bool Matrix::operator==(Matrix matrix){return this->equalTo(matrix);}

Matrix Matrix::operator+(Matrix matrix){return this->add(matrix);}
Matrix Matrix::operator+(float value){return this->add(value);}
Matrix Matrix::operator-(Matrix matrix){return this->subtract(matrix);}
Matrix Matrix::operator-(float value){return this->subtract(value);}
Matrix Matrix::operator*(Matrix matrix){return multiply((*this), matrix);}
Matrix Matrix::operator*(float value){return this->multiply(value);}
Matrix Matrix::operator/(float value){return this->divide(value);}

void Matrix::operator++(){this->add(1);}
void Matrix::operator--(){this->subtract(1);}

void Matrix::operator+=(Matrix matrix){this->add(matrix);}
void Matrix::operator+=(float value){this->add(value);}
void Matrix::operator-=(Matrix matrix){this->subtract(matrix);}
void Matrix::operator-=(float value){this->subtract(value);}
void Matrix::operator*=(Matrix matrix){(*this) = multiply((*this), matrix);}
void Matrix::operator*=(float value){this->multiply(value);}
void Matrix::operator/=(float value){this->divide(value);}
