#include <cmath>
#include "PointVector3D.h"
#include "QDebug"

PointVector3D::PointVector3D(double x, double y, double z, double u)
{
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
	this->vector[3] = u;
}

PointVector3D::PointVector3D(double* coordsArray)
{
	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] = coordsArray[i];
	}
}

PointVector3D::~PointVector3D()
{
}

void PointVector3D::normalizePointVector()
{
	double lenVector = getLen();
	
	for (int i = 0; i < VECTOR_DIMENSION - 1; i++)
	{
		vector[i] /= lenVector;
	}
}

PointVector3D PointVector3D::getNormolizeVector() const
{
	double lenVector = getLen();

	double tmpVector[3];

	for (int i = 0; i < VECTOR_DIMENSION - 1; i++)
	{
		tmpVector[i] = vector[i] / lenVector;
	}

	return PointVector3D(tmpVector[0], tmpVector[1], tmpVector[2], 1.0);
}

double PointVector3D::getX() const
{
	return vector[0];
}

double PointVector3D::getY() const
{
	return vector[1];
}

double PointVector3D::getZ() const
{
	return vector[2];
}

double PointVector3D::getU() const
{
	return vector[3];
}

double PointVector3D::getValue(int index) const
{
	return vector[index];
}

double PointVector3D::getLen() const
{
	double lenVector = 0;

	for (int i = 0; i < VECTOR_DIMENSION - 1; i++)
	{
		lenVector += vector[i] * vector[i];
	}

	return sqrt(lenVector);
}

double* PointVector3D::toArray() const
{
	double returnVector[VECTOR_DIMENSION];
	
	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		returnVector[i] = vector[i];
	}

	return returnVector;
}

double& PointVector3D::operator[](int index)
{
	return vector[index];
}

PointVector3D PointVector3D::operator+(const PointVector3D& right) const
{
	double returnVector[VECTOR_DIMENSION];

	for (int i = 0; i < VECTOR_DIMENSION - 1; i++)
	{
		returnVector[i] = this->vector[i] + right.getValue(i);
	}

	returnVector[VECTOR_DIMENSION - 1] = 1.0;

	return PointVector3D(returnVector);
}

PointVector3D& PointVector3D::operator+=(const PointVector3D& right)
{
	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] += right.getValue(i);
	}

	return *this;
}

PointVector3D& PointVector3D::operator*=(double scalar)
{
	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] *= scalar;
	}

	return *this;
}

PointVector3D PointVector3D::operator/(const double scalar) const
{
	double tmpVector[VECTOR_DIMENSION];

	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		tmpVector[i] = this->vector[i] / scalar;
	}

	return PointVector3D(tmpVector);
}

PointVector3D& PointVector3D::operator/=(const double scalar)
{
	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] /= scalar;
	}

	return *this;
}

PointVector3D PointVector3D::operator*(double scalar) const
{
	double tmpVector[VECTOR_DIMENSION];

	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		tmpVector[i] = this->vector[i] * scalar;
	}

	return PointVector3D(tmpVector);
}

PointVector3D PointVector3D::operator-(const PointVector3D& right) const
{
	double returnVector[VECTOR_DIMENSION];

	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		returnVector[i] = this->vector[i] - right.getValue(i);
	}

	return PointVector3D(returnVector);
}

PointVector3D& PointVector3D::operator-=(const PointVector3D& right)
{

	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] -= right.getValue(i);
	}

	return *this;
}

double PointVector3D::operator|(const PointVector3D& right) const
{
	double value = 0;

	for (int i = 0; i < 3; i++){
		value += vector[i] * right.getValue(i);
	}

	return value;
}

bool PointVector3D::operator==(const PointVector3D& right) const
{
	if (std::abs(vector[0] - right.getValue(0)) < 0.000001 && std::abs(vector[1] - right.getValue(1)) < 0.000001 &&
		std::abs(vector[2] - right.getValue(2)) < 0.000001){
		return true;
	}
	
	return false;
	
}

bool PointVector3D::operator!=(const PointVector3D& right) const
{
	return !(*this == right);
}

PointVector3D PointVector3D::operator*(const PointVector3D& right) const
{
	double tmpVector[VECTOR_DIMENSION];
	
//	tmpVector[0] = this->vector[2] * right.getValue(1) - this->vector[1] * right.getValue(2);
//	tmpVector[1] = this->vector[0] * right.getValue(2) - this->vector[2] * right.getValue(0);
//	tmpVector[2] = this->vector[1] * right.getValue(0) - this->vector[0] * right.getValue(1);
//	tmpVector[3] = 1.0;

	tmpVector[0] = this->vector[1] * right.getValue(2) - this->vector[2] * right.getValue(1);
	tmpVector[1] = this->vector[2] * right.getValue(0) - this->vector[0] * right.getValue(2);
	tmpVector[2] = this->vector[0] * right.getValue(1) - this->vector[1] * right.getValue(0);
	tmpVector[3] = 1.0;

	return PointVector3D(tmpVector);
}

PointVector3D& PointVector3D::operator*=(const PointVector3D& right)
{
	double tmpVector[VECTOR_DIMENSION];

//	tmpVector[0] = this->vector[2] * right.getValue(1) - this->vector[1] * right.getValue(2);
//	tmpVector[1] = this->vector[0] * right.getValue(2) - this->vector[2] * right.getValue(0);
//	tmpVector[2] = this->vector[1] * right.getValue(0) - this->vector[0] * right.getValue(1);
//	tmpVector[3] = 1.0;

	tmpVector[0] = this->vector[1] * right.getValue(2) - this->vector[2] * right.getValue(1);
	tmpVector[1] = this->vector[2] * right.getValue(0) - this->vector[0] * right.getValue(2);
	tmpVector[2] = this->vector[0] * right.getValue(1) - this->vector[1] * right.getValue(0);
	tmpVector[3] = 1.0;

	for (int i = 0; i < VECTOR_DIMENSION; i++)
	{
		this->vector[i] = tmpVector[i];
	}

	return *this;
}

double PointVector3D::calculateNorma(PointVector3D vector1, PointVector3D vector2)
{
	double acc = 0;
	for (int i = 0; i < 3; i++){
		acc += (vector1.getValue(i) - vector2.getValue(i)) * (vector1.getValue(i) - vector2.getValue(i));
	}

	return sqrt(acc);
}

std::ostream& operator << (std::ostream &outputStream, const PointVector3D& outputData)
{
	outputStream << outputData.getX() << ' ' << outputData.getY() << ' ' << outputData.getZ() << ' ' << outputData.getU() << std::endl;
	return outputStream;
}

PointVector3D operator*(double scalar, const PointVector3D& right)
{
	double tmpVector[PointVector3D::VECTOR_DIMENSION];
	
	for (int i = 0; i < PointVector3D::VECTOR_DIMENSION; i++)
	{
		tmpVector[i] = scalar * right.vector[i];
	}

	return PointVector3D(tmpVector);
}