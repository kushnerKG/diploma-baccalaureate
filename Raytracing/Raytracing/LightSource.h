#pragma once
#include "PointVector3D.h"
#include <ostream>
#include "RgbColor.h"
class LightSource
{
public:
	LightSource(PointVector3D& coordinates, Color* color);
	~LightSource();

	LightSource(const LightSource& other);

	LightSource(LightSource&& other);

	LightSource& operator=(const LightSource& other);

	LightSource& operator=(LightSource&& other);

	const PointVector3D& getLightSourceCoordinates() const;
	const Color& getLightSourceColor() const;
private:
	PointVector3D lightSourceCoordinates;
	Color* lightSourceColor;

};

