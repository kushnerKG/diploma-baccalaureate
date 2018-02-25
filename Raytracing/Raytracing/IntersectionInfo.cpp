#include "IntersectionInfo.h"
#include "AdvancedOpticProperties.h"
#include "QDebug"
IntersectionInfo::IntersectionInfo(PointVector3D intersectionPoints, std::shared_ptr<OpticProperties> opticProperties, 
	double len, PointVector3D normal, std::shared_ptr<Transmission> transmission, bool isTras) :
	intersectionPoints(intersectionPoints), opticProperties(opticProperties), len(len), normal(normal), isTras(isTras), transmission(transmission)
{
}

IntersectionInfo::~IntersectionInfo()
{
}

const PointVector3D& IntersectionInfo::getIntersectionPoints() const
{
	return intersectionPoints;
}

std::shared_ptr<OpticProperties> IntersectionInfo::getOpticProperties() const
{
	return opticProperties;
}

void IntersectionInfo::setOpticProperties(std::shared_ptr<OpticProperties> properties)
{
	opticProperties = std::move(properties);
}

double IntersectionInfo::getLen() const
{
	return len;
}

const PointVector3D& IntersectionInfo::getNormal() const
{
	return normal;
}

Transmission& IntersectionInfo::getTransmission() const
{
	return *transmission;
}

bool IntersectionInfo::isTrasasparency() const
{
	return isTras;
}

void IntersectionInfo::setTransmission(std::shared_ptr<Transmission> transmission)
{
	this->transmission = transmission;

}

bool IntersectionInfo::isIntesctionWithLightSource() const
{
	return intesectionWithLightSource;
}

void IntersectionInfo::setLightSourceIntesection(bool status)
{
	intesectionWithLightSource = status;
}

void IntersectionInfo::setIntersectionWithCylinder(bool isIntersected)
{
	this->intersectedWithCylinder = isIntersected;
}

bool IntersectionInfo::getIntersectionWithCylinder()
{
	return this->intersectedWithCylinder;
}
