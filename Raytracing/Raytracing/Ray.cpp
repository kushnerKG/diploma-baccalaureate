#include "Ray.h"


Ray::Ray(const PointVector3D& rayStart, const PointVector3D& rayFinish, bool isInner) : rayStart(rayStart), isIn(isInner)
{
	createRayDirection(rayFinish);
}

Ray::Ray(bool isRefracted) : isIn(isRefracted)
{
}

Ray::~Ray()
{
}

void Ray::createRayDirection(const PointVector3D& vector)
{
	rayDirection = (vector - rayStart).getNormolizeVector();
}

const PointVector3D& Ray::getRayStart() const
{
	return rayStart;
}

const PointVector3D& Ray::getRayDirection() const
{
	return rayDirection;
}

void Ray::setRayStart(const PointVector3D rayStart)
{
	this->rayStart = rayStart;
}

void Ray::setRayDirection(const PointVector3D rayDirection)
{
	this->rayDirection = rayDirection;
}

bool Ray::isInner() const
{
	return isIn;
}
