#include "Plane.h"
#define _USE_MATH_DEFINES

Plane::Plane(double A, double B, double C, double D) : A(A), B(B), C(C), D(D)
{
}

Plane::Plane(PointVector3D& direction, PointVector3D& point)
{
	PointVector3D normal = direction.getNormolizeVector();
	A = normal.getX();
	B = normal.getY();
	C = normal.getZ();

	D = -1.0 * (A * point.getX() + B * point.getY() + C * point.getZ());
}

double Plane::distanceFrom(PointVector3D& point) const
{
	return  (A * point.getX() + B * point.getY() + C * point.getZ() + D) / (sqrt(A*A + B*B + C*C));
}

PointVector3D Plane::getNormal() const
{
	return PointVector3D(A, B, C, 1.0);
}

double Plane::getA() const
{
	return A;
}

double Plane::getB() const
{
	return B;
}

double Plane::getC() const
{
	return C;
}

double Plane::getD() const
{
	return D;
}

Plane::~Plane()
{
}
