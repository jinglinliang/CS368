//////////////////////////////////////////////
// 
// Your Name:  Manyi Zhu
// 
// CS 368, Spring 2018
// Matrix.hpp
//
//////////////////////////////////////////////

#ifndef LECTURE9_MATRIX_HPP
#define LECTURE9_MATRIX_HPP

#include <iostream>
#include <vector>
#include <stdexcept> // includes runtime_error, which is a subclass of exception

/**
 * @brief This class derives from runtime_error
 *        It is thrown in the case of Non-postive matrix dimensions
 */
class NonPositiveDimensionException : public std::runtime_error {
    public:
        // constructor that calls the base class constructor in its initializer list
        NonPositiveDimensionException() : std::runtime_error("dimensions must be positive")
    {// no other code in the constructor
    }
};

/**
 * @brief This class derives from runtime_error
 *        It is thrown in the case of any dimension mismatch
 */
class DimensionMismatchException : public std::runtime_error {
    public:
        DimensionMismatchException() : std::runtime_error("dimensions do not match")
    {// no other code in the constructor
    }
};

class RowIndexOutOfBoundsException : public std::runtime_error {
    public:
        // constructor that calls the base class constructor in its initializer list
        RowIndexOutOfBoundsException() : std::runtime_error("row index out of bounds")
    {// no other code in the constructor
    }
};

///////////////////////////////////////////////////////
// DECLARATION OF THE MATRIX TEMPLATED CLASS
// YOU WILL NEED TO ADD MORE PROTOTYPES HERE
///////////////////////////////////////////////////////
template<typename T>
class Matrix {
    private:
        int rows;
        int cols;
        std::vector<std::vector<T>> data;

    public:
        Matrix();
        Matrix(int r, int c);
        void print() const;

        // we need operator[] to return a modifiable L-value, that is, a non-const reference
        // so that we can store a value in a cell of a Matrix
        std::vector<T> & operator[](const int index); 

        // we need a second operator[] in order to make our operator+ below work
        // the operator+ function below needs a const Matrix as a parameter
        // thus this second opertor[] function returns a const reference
        const std::vector<T> & operator[](const int index) const; 

        // operator+ has a const reference parameter, promises not to modify this object
        // and returns a const value
        const Matrix<T> operator+(const Matrix<T> &rhs) const;

        //return row and col number
        int getRows() const;
        int getCols() const;

        // operator==,!=,-,* have a const reference parameter, promises not to modify this object
        // and returns a const value
        const bool operator==(const Matrix<T> &rhs) const;
        const bool operator!=(const Matrix<T> &rhs) const;
        const Matrix<T> operator-(const Matrix<T> &rhs) const;
        const Matrix<T> operator*(const Matrix<T> &rhs) const;


        //Declare scalar * as friend function since not sure which object it's called on
        friend const Matrix<T> operator*(T scalar, const Matrix<T> &rhs){
            int rows = rhs.rows;
            int cols = rhs.cols;
            Matrix<T> result(rows, cols);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result[i][j] = scalar * rhs[i][j];
                }
            }
            return result;
        }
        friend const Matrix<T> operator*(const Matrix<T> &rhs, T scalar){
            return scalar * rhs;
        }

        //Compound operators referencing the original operators
        Matrix<T> & operator+=(const Matrix<T> &rhs);
        Matrix<T> & operator-=(const Matrix<T> &rhs);
        Matrix<T> & operator*=(const Matrix<T> &rhs);
        Matrix<T> & operator*=(T scalar);


        /**
         * @brief overrides the << operator for Matrix<T>
         *        because this is a friend function, and because we have a templated class
         *        we need to define the function inside our class declaration
         * @param os the stream that we are using << with
         * @param obj  the Matrix<T> we are trying to insert into the stream
         * @return a non-const reference, which allows us to chain << operators
         */
        friend std::ostream& operator<<(std::ostream& os, const Matrix<T> &obj) {
            for (auto rowIt = obj.data.begin(); rowIt != obj.data.end(); ++rowIt) {
                for (auto colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt) {
                    if (colIt != rowIt->end() - 1) {
                        os << *colIt << " ";
                    } else {
                        os << *colIt;
                    }
                }
                os << std::endl;
            }
            return os;
        }
};

///////////////////////////////////////////////////////
// IMPLEMENTATION OF THE MATRIX TEMPLATED CLASS
// YOU WILL NEED TO ADD MORE FUNCTION DEFINITIONS HERE
///////////////////////////////////////////////////////

/**
 * @brief default Constructor for Matrix<T>
 *        calls the other constructor through an initializer list
 */

template<typename T>
Matrix<T>::Matrix() : Matrix(1,1)
{
}


/**
 * @brief Constructor for Matrix<T>
 *        Matrices store their data in a single vector called data
 *        We assume the matrix will use row-major ordering
 * @param r the number of rows (non-negative)
 * @param c the number of cols (non-negative)
 */
template<typename T>
Matrix<T>::Matrix(int r, int c) {
    if (r <= 0 || c <= 0) {
        throw NonPositiveDimensionException();
    }
    rows = r;
    cols = c;
    data.resize(rows);
    for (int r = 0; r < rows; ++r) {
        data[r].resize(cols);
    }
}

/**
 * @brief prints out to the terminal the elements of this matrix
 *        we assume row-major ordering of the elements
 */
template<typename T>
void Matrix<T>::print() const {
    for (auto rowit = data.begin(); rowit != data.end(); ++rowit) {
        std::vector<T> rowData = *rowit;
        for (auto colit = rowData.begin(); colit != rowData.end(); ++colit) {
            if (colit != rowData.end() - 1)
                std::cout << *colit << " ";
            else
                std::cout << *colit ;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}




/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 *        this non-const function returns a non-const reference
 * @param index     the index which corresponds to a vector (row) in the matrix
 * @return a non-const vector reference that is suitable for a Left-value
 */
template<typename T>
std::vector<T> & Matrix<T>::operator[](const int index) {
    //std::cout << " calling non-const operator[] with index = " << index << std::endl;
    if (index < 0 || index >= rows){
        throw RowIndexOutOfBoundsException();
    }
    return data[index];
}


/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 * @param       the index which corresponds to a vector (row) in the matrix
 * @return      a const vector referene that is suitable for a Right-value
 */
template<typename T>
const std::vector<T> & Matrix<T>::operator[](const int index) const {
    //std::cout << "     calling const operator[] with index = " << index << std::endl;
    if (index < 0 || index >= rows){
        throw RowIndexOutOfBoundsException();
    }
    return data[index];
}

/**
 * @brief adds the Matrix<T> on the right side of the + operator to the matrix on the left
 *        is called on the Matrix on the left
 * @param rhs a const reference to the Matrix on the right of the + operator
 * @return a const Matrix that represents the sum
 */
template<typename T>
const Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    if (lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
        throw DimensionMismatchException();
    }
    int rows = rhs.rows;
    int cols = rhs.cols;
    Matrix<T> result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            //std::cout << "adding elements at [" << i << "][" << j << "]" << std::endl;
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}



//return row number
template<typename T>
int Matrix<T>::getRows() const {
    return rows;
}

//return col number
template<typename T>
int Matrix<T>::getCols() const {
    return cols;
}


template<typename T>
const Matrix<T> Matrix<T>::operator-(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    if (lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
        throw DimensionMismatchException();
    }
    int rows = rhs.rows;
    int cols = rhs.cols;
    Matrix<T> result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}


template<typename T>
const bool Matrix<T>::operator==(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    if (lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
        return false;
    }
    int rows = rhs.rows;
    int cols = rhs.cols;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (lhs[i][j] != rhs[i][j])
                //found unequal, return immediately
                return false;
        }
    }
    return true;
}


template<typename T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    if (lhs.cols != rhs.rows) {
        throw DimensionMismatchException();
    }

    Matrix<T> result(lhs.rows, rhs.cols);
    for (int i = 0; i < lhs.rows; i++){
        for (int j = 0; j < rhs.cols; j++){
            for (int k = 0; k < lhs.cols; k++){
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return result;
}


template<typename T>
const bool Matrix<T>::operator!=(const Matrix<T> &rhs) const {
    if (*this == rhs) //call operator==
        return false;
    return true;
}

template<typename T>
Matrix<T> & Matrix<T>::operator+=(const Matrix<T> &rhs){
    *this = *this + rhs; // calls operator+
    return *this;
}
template<typename T>
Matrix<T> & Matrix<T>::operator-=(const Matrix<T> &rhs){
    *this = *this - rhs; // calls operator-
    return *this;
}
template<typename T>
Matrix<T> & Matrix<T>::operator*=(const Matrix<T> &rhs){
    *this = *this * rhs; // calls operator*
    return *this;
}
template<typename T>
Matrix<T> & Matrix<T>::operator*=(T scalar){
    *this = *this * scalar; // calls operator*
    return *this;
}

#endif //LECTURE9_MATRIX_HPP
