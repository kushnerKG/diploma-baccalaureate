#include "LightSource.h"
#include "RgbColor.h"

LightSource::LightSource(PointVector3D &coordinates, Color* color) : lightSourceCoordinates(coordinates), lightSourceColor(color->clone())
{
} 

LightSource::~LightSource()
{
	delete lightSourceColor;
}

LightSource::LightSource(const LightSource& other): lightSourceCoordinates(other.lightSourceCoordinates),
                                                    lightSourceColor(other.lightSourceColor->clone())
{
}

LightSource::LightSource(LightSource&& other): lightSourceCoordinates(std::move(other.lightSourceCoordinates)),
                                               lightSourceColor(other.lightSourceColor->clone())
{
}

LightSource& LightSource::operator=(const LightSource& other)
{
	if (this == &other)
		return *this;
	lightSourceCoordinates = other.lightSourceCoordinates;
	lightSourceColor = other.lightSourceColor->clone();
	return *this;
}

LightSource& LightSource::operator=(LightSource&& other)
{
	if (this == &other)
		return *this;
	lightSourceCoordinates = std::move(other.lightSourceCoordinates);
	lightSourceColor = other.lightSourceColor->clone();
	return *this;
}

const PointVector3D& LightSource::getLightSourceCoordinates() const
{
	return lightSourceCoordinates;
}

const Color& LightSource::getLightSourceColor() const
{
	return *lightSourceColor;
}

