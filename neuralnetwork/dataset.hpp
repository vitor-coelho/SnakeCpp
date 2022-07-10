#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "matrix.hpp"

#define DELIMITER ','

void shuffleMatrix(Matrix<float>* input, Matrix<float>* output);

class Dataset{
    private:
        Matrix<float> trainInput;
        Matrix<float> trainOutput;
        Matrix<float> testInput;
        Matrix<float> testOutput;
        Matrix<float> validationInput;
        Matrix<float> validationOutput;
        size_t batchSize;
        size_t dataIdx;

    public:
        Dataset(size_t batchSize_);
        Dataset(Matrix<float> input, Matrix<float> output, size_t batchSize_, float trainPct, float testPct, float validationPct);

        bool nextBatch();       // Update data index to the next batch - returns true if all data has been done
        void shuffle();         // Shuffle train set
        void writeToCsv(std::string path);
        static Matrix<float> getMatrixFromCsv(std::string path);
        static Dataset getDatasetFromCsv(std::string path);
        void print();   // Print dataset information (num samples; pct of train, test, val; first five samples of train)
        size_t getNumSamples(){return trainInput.numRows();};

        Matrix<float> getBatchInput();
        Matrix<float> getBatchOutput();

        Matrix<float> getTrainInput(){return trainInput;};
        Matrix<float> getTrainOutput(){return trainOutput;};
        Matrix<float> getTestInput(){return testInput;};
        Matrix<float> getTestOutput(){return testOutput;};
        Matrix<float> getValidationInput(){return validationInput;};
        Matrix<float> getValidationOutput(){return validationOutput;};

        void setBatchSize(size_t batchSize_){batchSize = batchSize_;};
        void setTrainInput(Matrix<float> data);
        void setTrainOutput(Matrix<float> data);
        void setTestInput(Matrix<float> data);
        void setTestOutput(Matrix<float> data);
        void setValidationInput(Matrix<float> data);
        void setValidationOutput(Matrix<float> data);

        void operator=(Dataset toCopy);
};

inline Dataset::Dataset(size_t batchSize_){
    batchSize = batchSize_;
    dataIdx = 0;
}

inline Dataset::Dataset(Matrix<float> input, Matrix<float> output, size_t batchSize_, float trainPct, float testPct, float validationPct){
    if(abs(trainPct + testPct + validationPct - 1) > 0.001)
            throw std::invalid_argument("Percentages must sum to 1");

    float trainIndex = input.numRows()*trainPct;
    float testIndex  = input.numRows()*testPct + trainIndex;
    float validationIndex = input.numRows()*validationPct + testIndex;

    size_t trainIdx      = (size_t) trainIndex == std::floor(trainIndex) ? trainIndex - 1 : trainIndex;
    size_t testIdx       = (size_t) testIndex == std::floor(testIndex) ? testIndex - 1 : testIndex;
    size_t validationIdx = (size_t) validationIndex == std::floor(validationIndex) ? validationIndex - 1 : validationIndex;

    batchSize = batchSize_;
    dataIdx = 0;
    shuffleMatrix(&input, &output);

    trainInput  =  input(0, trainIdx, 0, input.numCols()-1);
    trainOutput = output(0, trainIdx, 0, output.numCols()-1);

    if(testIdx != trainIdx){
        testInput   =  input(trainIdx+1, testIdx, 0, input.numCols()-1);
        testOutput  = output(trainIdx+1, testIdx, 0, output.numCols()-1);
    }

    if(validationIdx != testIdx){
        validationInput  =  input(testIdx+1, validationIdx, 0, input.numCols()-1);
        validationOutput = output(testIdx+1, validationIdx, 0, output.numCols()-1);
    }
}

inline bool Dataset::nextBatch(){
    dataIdx += batchSize;
    if(dataIdx >= trainInput.numRows()){
        dataIdx = 0;
        return false;   // Data is over
    }
    return true;    // There is more data
}

inline void Dataset::shuffle(){
    size_t size = trainInput.numRows();
    std::vector<size_t> idxList(size);
    Matrix<float> oldInput = trainInput, oldOutput = trainOutput;

    std::generate(idxList.begin(), idxList.end(), [n=0]()mutable{return n++;});
    std::shuffle(idxList.begin(), idxList.end(), mt());

    for(size_t i = 0; i < size; i++){
        trainInput.set(oldInput(idxList.at(i), ROW), i, ROW);
        trainOutput.set(oldOutput(idxList.at(i), ROW), i, ROW);
    }
}

inline void shuffleMatrix(Matrix<float>* input, Matrix<float>* output){
    size_t size = input->numRows();
    std::vector<size_t> idxList(size);
    Matrix<float> oldInput = *input, oldOutput = *output;

    std::generate(idxList.begin(), idxList.end(), [n=0]()mutable{return n++;});
    std::shuffle(idxList.begin(), idxList.end(), mt());

    for(size_t i = 0; i < size; i++){
        input->set(oldInput(idxList.at(i), ROW), i, ROW);
        output->set(oldOutput(idxList.at(i), ROW), i, ROW);
    }
}

inline static void writeMatrixToCsv(Matrix<float> matrix, std::string path){
    std::fstream fout;

    fout.open(path, std::ios::out | std::ios::trunc);
    for(size_t i = 0; i < matrix.numRows(); i++){
        for(size_t j = 0; j < matrix.numCols(); j++){
            fout << std::setprecision(15);
            fout << std::fixed;
            fout << std::setw(20) << matrix(i,j);
            if(j != matrix.numCols()-1)
                fout << DELIMITER;
        }
        fout << "\n";
    }
    fout.close();
    fout.clear();
}

inline void Dataset::writeToCsv(std::string path){
    std::fstream fout;

    writeMatrixToCsv(trainInput, path + "_intrain.csv");
    writeMatrixToCsv(trainOutput, path + "_outtrain.csv");
    writeMatrixToCsv(testInput, path + "_intest.csv");
    writeMatrixToCsv(testOutput, path + "_outtest.csv");
    writeMatrixToCsv(validationInput, path + "_inval.csv");
    writeMatrixToCsv(validationOutput, path + "_outval.csv");
}

inline static Matrix<float> getMatrixFromCsv(std::string path){
    std::ifstream fin;

    Matrix<float> data;
    std::vector<float> row;
  
    fin.open(path, std::ios::in);

    std::string line, word, temp;
  
    while(std::getline(fin, line)){
        row.clear();
        std::stringstream s(line);

        while(std::getline(s, word, DELIMITER)){
            row.push_back(std::stof(word));
        }

        data = data.append(Matrix<float>(row, (size_t) 1, row.size()), ROW);
    }

    return data;
}

inline static Dataset getDatasetFromCsv(std::string path){
    Dataset dataset(1);
    
    dataset.setTrainInput(getMatrixFromCsv(path + "_intrain.csv"));
    dataset.setTrainOutput(getMatrixFromCsv(path + "_outtrain.csv"));
    dataset.setTestInput(getMatrixFromCsv(path + "_intest.csv"));
    dataset.setTestOutput(getMatrixFromCsv(path + "_outtest.csv"));
    dataset.setValidationInput(getMatrixFromCsv(path + "_inval.csv"));
    dataset.setValidationOutput(getMatrixFromCsv(path + "_outval.csv"));

    return dataset;
}

inline void Dataset::print(){
    size_t trainSize, testSize, valSize, totalSize;
    trainSize = trainInput.numRows();
    testSize = testInput.numRows();
    valSize = validationInput.numRows();
    totalSize = trainSize + testSize + valSize;

    std::cout << std::endl << "Number of samples: " << totalSize << std::endl;
    std::cout << std::setprecision(2);
    std::cout << std::fixed;
    std::cout << "Train samples: " << trainSize << " (" << std::setw(5) << trainSize*100.0/totalSize << "%)" << std::endl;
    std::cout << "Test samples: " << testSize << " (" << std::setw(5) << testSize*100.0/totalSize << "%)" << std::endl;
    std::cout << "Validation samples: " << valSize << " (" << std::setw(5) << valSize*100.0/totalSize << "%)" << std::endl;

    size_t printIdx = trainSize > 4 ? 4 : trainSize-1;
    trainInput(0, printIdx, 0, trainInput.numCols()-1).print();
    trainOutput(0, printIdx, 0, trainOutput.numCols()-1).print();
}

inline Matrix<float> Dataset::getBatchInput(){
    size_t dataEnd = dataIdx + batchSize - 1 < trainInput.numRows() ? dataIdx + batchSize - 1 : trainInput.numRows() - 1;
    return trainInput(dataIdx, dataEnd, 0, trainInput.numCols()-1);
}

inline Matrix<float> Dataset::getBatchOutput(){
    size_t dataEnd = dataIdx + batchSize - 1 < trainOutput.numRows() ? dataIdx + batchSize - 1 : trainOutput.numRows() - 1;
    return trainOutput(dataIdx, dataEnd, 0, trainOutput.numCols()-1);
}

inline void Dataset::setTrainInput(Matrix<float> data){
    trainInput = data;
}

inline void Dataset::setTrainOutput(Matrix<float> data){
    if(data.numRows() != trainInput.numRows())
        throw std::invalid_argument("Number of samples different from input data");
    trainOutput = data;
}

inline void Dataset::setTestInput(Matrix<float> data){
    testInput = data;
}

inline void Dataset::setTestOutput(Matrix<float> data){
    if(data.numRows() != testInput.numRows())
        throw std::invalid_argument("Number of samples different from input data");
    testOutput = data;
}

inline void Dataset::setValidationInput(Matrix<float> data){
    validationInput = data;
}

inline void Dataset::setValidationOutput(Matrix<float> data){
    if(data.numRows() != validationInput.numRows())
        throw std::invalid_argument("Number of samples different from input data");
    validationOutput = data;
}

inline void Dataset::operator=(Dataset toCopy){
    this->trainInput = toCopy.trainInput;
    this->trainOutput = toCopy.trainOutput;
    this->testInput = toCopy.testInput;
    this->testOutput = toCopy.testOutput;
    this->validationInput = toCopy.validationInput;
    this->validationOutput = toCopy.validationOutput;
}

#endif
