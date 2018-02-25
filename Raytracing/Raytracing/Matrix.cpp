#ifndef __MATRIX_CPP
#define __MATRIX_CPP

#include <Matrix.h>

template<typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const T& initial) :rows(rows), cols(cols)
{
	mat.resize(rows);
	for (unsigned int i = 0; i<mat.size(); i++) 
	{
		mat[i].resize(cols, initial);
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
	mat = rhs.mat;
	rows = rhs.getRows();
	cols = rhs.getCols();
}
template<typename T>
Matrix<T>::~Matrix()
{
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs) 
{
	if (&rhs == this)
		return *this;

	unsigned int newRows = rhs.getRows();
	unsigned int newCols = rhs.getCols();

	mat.resize(newRows);
	for (unsigned int i = 0; i < this->mat.size(); i++) 
	{
		mat[i].resize(newCols);
	}

	for (unsigned int i = 0; i<newRows; i++) 
	{
		for (unsigned int j = 0; j<newCols; j++) 
		{
			mat[i][j] = rhs(i, j);
		}
	}

	this->rows = newRows;
	this->cols = newCols;

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const
{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i<rows; i++) 
	{
		for (unsigned int j = 0; j<cols; j++) 
		{
			result(i, j) = mat[i][j] + rhs(i, j);
		}
	}

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs) 
{
	unsigned int rows = rhs.getRows();
	unsigned int cols = rhs.getCols();

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j<cols; j++) 
		{
			this->mat[i][j] += rhs(i, j);
		}
	}

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const
{
	unsigned int rows = rhs.getRows();
	unsigned int cols = rhs.getCols();
	Matrix result(rows, cols, 0);

	for (unsigned i = 0; i < rows; i++) 
	{
		for (unsigned j = 0; j < cols; j++) 
		{
			result(i, j) = this->mat[i][j] - rhs(i, j);
		}
	}

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs) 
{
	unsigned int rows = rhs.getRows();
	unsigned int  cols = rhs.getCols();

	for (unsigned int i = 0; i < rows; i++) 
	{
		for (unsigned int j = 0; j < cols; j++) 
		{
			mat[i][j] -= rhs(i, j);
		}
	}

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const
{
	unsigned int rows = rhs.getRows();
	unsigned int cols = rhs.getCols();
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i<rows; i++) 
	{
		for (unsigned int j = 0; j<cols; j++) 
		{
			for (unsigned int k = 0; k<rows; k++) 
			{
				result(i, j) += mat[i][k] * rhs(k, j);
			}
		}
	}

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& rhs) 
{
	Matrix result = (*this) * rhs;

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			this->mat[i][j] = result(i, j);
		}
	}

	(*this) = result;
	return *this;
}

template<typename T>
PointVector3D Matrix<T>::operator*(const PointVector3D& vector) const
{
	double tmpVector[4];

	for (int i = 0; i < 4; i++)
	{
		tmpVector[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			tmpVector[i] += mat[i][j] * vector.getValue(j);
		}
	}

	return PointVector3D(tmpVector);
}

template<typename T>
Matrix<T> Matrix<T>::transpose() 
{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			result(i, j) = mat[j][i];
		}
	}

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T& rhs) const
{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			result(i, j) = mat[i][j] - rhs;
		}
	}

	return result;
}


template<typename T>
Matrix<T> Matrix<T>::operator*(const T& rhs) const{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			result(i, j) = mat[i][j] * rhs;
		}
	}

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T& rhs) const
{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			result(i, j) = mat[i][j] + rhs;
		}
	}

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T& rhs) const
{
	Matrix result(rows, cols, 0);

	for (unsigned int i = 0; i< rows; i++) 
	{
		for (unsigned int j = 0; j< cols; j++) 
		{
			result(i, j) = mat[i][j] / rhs;
		}
	}

	return result;
}

template<typename T>
std::vector<T> Matrix<T>::operator*(const std::vector<T>& rhs) 
{
	std::vector<T> result(rhs.size(), 0);

	for (unsigned i = 0; i< rows; i++) 
	{
		for (unsigned j = 0; j< cols; j++) 
		{
			result[i] = mat[i][j] * rhs[j];
		}
	}

	return result;
}

template<typename T>
T& Matrix<T>::operator()(const unsigned row, const unsigned col) 
{
	return this->mat[row][col];
}

template<typename T>
const T& Matrix<T>::operator()(const unsigned row, const unsigned col) const 
{
	return this->mat[row][col];
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](unsigned row)
{
	return mat[row];
}

template<typename T>
unsigned int Matrix<T>::getRows() const 
{
	return this->rows;
}


template<typename T>
unsigned int Matrix<T>::getCols() const 
{
	return this->cols;
}

#endif