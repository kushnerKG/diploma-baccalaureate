#include "PolarizedRay.h"


PolarizedRay::PolarizedRay(const PointVector3D& rayStart, const PointVector3D& rayFinish, 
	bool isInner, PointVector3D polarizationVector, double intensity) : Ray(rayStart, rayFinish, isInner), polarizationDirection(polarizationVector), intensity(intensity)
{
}

PolarizedRay::~PolarizedRay()
{
}

PointVector3D PolarizedRay::getPolarizedVector() const
{
	return polarizationDirection;
}

void PolarizedRay::setPolarizedVector(PointVector3D& ray)
{
	polarizationDirection = ray;
}

Ray* PolarizedRay::getNotPolarizedRay()
{
	return static_cast<Ray*>(this);
}

double& PolarizedRay::getIntensity()
{
	return intensity;
}
