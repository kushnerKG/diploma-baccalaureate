#pragma once
#include "Ray.h"

class PolarizedRay :
	public Ray
{
public:
	PolarizedRay(const PointVector3D& rayStart, const PointVector3D& rayFinish, bool isInner, PointVector3D polarizationVector, double intensity);
	virtual ~PolarizedRay();

	PointVector3D getPolarizedVector() const;
	void setPolarizedVector(PointVector3D& ray);

	Ray* getNotPolarizedRay();

	double& getIntensity();
private:
	PointVector3D polarizationDirection;

	double intensity = 0;
};

