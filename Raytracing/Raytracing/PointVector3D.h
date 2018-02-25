#pragma once

#include <iostream>

class PointVector3D
{
public:
	PointVector3D(double x = 0, double y = 0, double z = 0, double u = 1.0);
	explicit PointVector3D(double* coordsArray);
	~PointVector3D();

	void normalizePointVector();
	PointVector3D getNormolizeVector() const;

	double getX() const;
	double getY() const;
	double getZ() const;
	double getU() const;
	double getValue(int index) const;
	double getLen() const;

	double* toArray() const;

	double &operator[](int index);

	PointVector3D operator+ (const PointVector3D& right) const;
	PointVector3D& operator+= (const PointVector3D& right);
	PointVector3D operator* (const double scalar) const;
	PointVector3D& operator*= (const double scalar);
	PointVector3D operator/ (const double scalar) const;
	PointVector3D& operator/= (const double scalar);
	PointVector3D operator* (const PointVector3D& right) const;//левая система координат
	PointVector3D& operator*= (const PointVector3D& right);//левая система координат
	PointVector3D operator- (const PointVector3D& right) const;
	PointVector3D& operator-= (const PointVector3D& right);
	double operator|(const PointVector3D& right) const;//скалярное произведение
	bool operator==(const PointVector3D& right) const;
	bool operator!=(const PointVector3D& right) const;

	static double calculateNorma(PointVector3D vector1, PointVector3D vector2);

	friend std::ostream& operator<<(std::ostream &outputStream, const PointVector3D& outputData);
	friend PointVector3D operator*(double scalar, const PointVector3D& right);

private:
	const static int VECTOR_DIMENSION = 4;

	double vector[VECTOR_DIMENSION];
};

