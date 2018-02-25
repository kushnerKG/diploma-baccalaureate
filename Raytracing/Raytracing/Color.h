#pragma once
#include <qrgb.h>
//#include "Intensity.h"

class Intensity;

class Color
{

public:
	virtual ~Color()
	{
	}

	virtual Color* clone() = 0;
	virtual QRgb getRGB() = 0;
	virtual void power(double power) = 0;
	virtual Color& operator/(double val) = 0;
	virtual Color& operator/=(double val) = 0;
	virtual Intensity& operator*(const Color& right) const = 0;
	virtual Intensity& operator*(double val) const = 0;
	virtual double operator[](int index) const = 0;
	virtual int getSize() const = 0;
};
