#pragma once
#include "OpticProperties.h"
#include "PointVector3D.h"
#include "Transmission.h"
class Transmission;

class IntersectionInfo
{
public:
	IntersectionInfo(PointVector3D intersectionPoints, std::shared_ptr<OpticProperties> opticProperties, double len, PointVector3D normal,
		std::shared_ptr<Transmission> transmission, bool isTras);

	~IntersectionInfo();

	const PointVector3D& getIntersectionPoints() const;
	std::shared_ptr<OpticProperties> getOpticProperties() const;
	void setOpticProperties(std::shared_ptr<OpticProperties> opticProperties);
	double getLen() const;
	const PointVector3D& getNormal() const;
	Transmission& getTransmission() const;
	bool isTrasasparency() const;
	void setTransmission(std::shared_ptr<Transmission> transmission);
	bool isIntesctionWithLightSource() const;
	void setLightSourceIntesection(bool status);


	void setIntersectionWithCylinder(bool isIntersected);
	bool getIntersectionWithCylinder();

private:

	const PointVector3D intersectionPoints;
	std::shared_ptr<OpticProperties> opticProperties;
	const double len;
	const PointVector3D normal;
	const bool isTras;
	std::shared_ptr<Transmission> transmission;

	bool intesectionWithLightSource = false;

	bool intersectedWithCylinder = false;
};
