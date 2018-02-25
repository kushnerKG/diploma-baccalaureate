#pragma once
#include "PointVector3D.h"
class Ray
{
public:
	Ray(const PointVector3D& rayStart, const PointVector3D& rayFinish, bool isInner);//вызывается если извесны только стартовая точка и точка лежащая на луче
	
	Ray(bool isRefracted = false);

	//Ray(Ray& ray);
	
	~Ray();

	const PointVector3D& getRayStart() const;
	const PointVector3D& getRayDirection() const;

	void setRayStart(const PointVector3D rayStart);
	void setRayDirection(const PointVector3D rayDirection);

	bool isInner() const;

private:
	PointVector3D rayStart;
	PointVector3D rayDirection;

//	PointVector3D parallelPolarizationVector;
//	PointVector3D normalPolarizationVector;
	bool isIn = false;

	void createRayDirection(const PointVector3D& vector);
};

