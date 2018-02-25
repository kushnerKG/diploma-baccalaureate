#pragma once
#include "PointVector3D.h"

class Plane
{
public:
	Plane(double A, double B, double C, double D);
	Plane(PointVector3D& direction, PointVector3D& point);

	~Plane();


	double distanceFrom(PointVector3D& point) const;

	PointVector3D getNormal() const;


	double getA() const;
	double getB() const;
	double getC() const;
	double getD() const;
private:
	double A;
	double B;
	double C;
	double D;
};

