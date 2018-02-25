#pragma once
#include <vector>
#include "PointVector3D.h"

template <typename T> class Matrix
{
	std::vector<std::vector<T> > mat;
	unsigned int rows;
	unsigned int cols;

public:
	Matrix(unsigned int rows, unsigned int cols, const T& _initial);
	Matrix(const Matrix<T>& rhs);
	~Matrix();

	// Operator overloading, for "standard" mathematical matrix operations                                                                                                                                                          
	Matrix<T>& operator=(const Matrix<T>& rhs);

	// Matrix mathematical operations                                                                                                                                                                                               
	Matrix<T> operator+(const Matrix<T>& rhs) const;
	Matrix<T>& operator+=(const Matrix<T>& rhs);
	Matrix<T> operator-(const Matrix<T>& rhs) const;
	Matrix<T>& operator-=(const Matrix<T>& rhs);
	Matrix<T> operator*(const Matrix<T>& rhs) const;
	Matrix<T>& operator*=(const Matrix<T>& rhs);

	PointVector3D operator*(const PointVector3D& vector) const;

	Matrix<T> transpose();

	// Matrix/scalar operations                                                                                                                                                                                                     
	Matrix<T> operator+(const T& rhs) const;
	Matrix<T> operator-(const T& rhs) const;
	Matrix<T> operator*(const T& rhs) const;
	Matrix<T> operator/(const T& rhs) const;

	// Matrix/vector operations                                                                                                                                                                                                     
	std::vector<T> operator*(const std::vector<T>& rhs);

	// Access the individual elements                                                                                                                                                                                               
	T& operator()(unsigned int row, unsigned int col);
	const T& operator()(unsigned int row, unsigned int col) const;

	std::vector<T>& operator[](unsigned int row);

	// Access the row and column sizes                                                                                                                                                                                              
	unsigned int getRows() const;
	unsigned int getCols() const;

};

#include "Matrix.cpp"