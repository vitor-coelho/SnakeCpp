#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <stdexcept>
#include <string>
#include <time.h>


class Matrix{
    private:
        size_t rows, cols;
        std::vector<float> values;

    public:
        Matrix();
        Matrix(size_t numRows, size_t numCols, std::string fill="zeros");
        Matrix(std::vector<float> vec, size_t numRows, size_t numCols);
        ~Matrix();

        void print();

        void set(float value, size_t row, size_t col);
        void set(std::vector<float> vec, size_t idx, bool row=true);
        void set(Matrix matrix);

        bool equalTo(Matrix matrix);

        size_t numRows();
        size_t numCols();
        std::vector<float>* getValues();

        void setRows(size_t newRows);
        void setCols(size_t newCols);

        Matrix add(float value);
        Matrix add(Matrix matrix);
        Matrix subtract(float value);
        Matrix subtract(Matrix matrix);
        Matrix multiply(float value);
        Matrix divide(float value);
        float dotProduct(Matrix matrix);
        float sum();
        Matrix transpose();

        Matrix applyFunction(float function(float));
        Matrix insert(Matrix toAppend, size_t idx, bool row=true);
        Matrix append(Matrix toAppend, bool row=true);
        Matrix del(size_t startIdx, size_t endIdx, bool row=true);

        // Static operators
        static Matrix multiply(Matrix matrix1, Matrix matrix2);
        static Matrix multiply(Matrix matrix, float value);
        static float dotProduct(Matrix matrix1, Matrix matrix2);

        //Matrix crossProduct(Matrix matrix);   //TODO
        //static Matrix crossProduct(Matrix matrix1, Matrix matrix2);   //TODO

        // Overloaded operators
        float operator()(size_t row, size_t col);
        Matrix operator()(size_t idx, bool row=true);

        void operator=(Matrix matrix);
        bool operator==(Matrix matrix);

        Matrix operator+(Matrix matrix);
        Matrix operator+(float value);
        Matrix operator-(Matrix matrix);
        Matrix operator-(float value);
        Matrix operator*(Matrix matrix);
        Matrix operator*(float value);
        Matrix operator/(float value);

        void operator++();
        void operator--();

        void operator+=(Matrix matrix);
        void operator+=(float value);
        void operator-=(Matrix matrix);
        void operator-=(float value);
        void operator*=(Matrix matrix);
        void operator*=(float value);
        void operator/=(float value);
};

#endif
