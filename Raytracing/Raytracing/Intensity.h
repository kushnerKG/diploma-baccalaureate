#pragma once
#include <QColor>
class Transmission;
class Color;

class Intensity
{

public:
	virtual ~Intensity()
	{
	}

	virtual void pow(double power) = 0;
	virtual bool isBackground() = 0;
	virtual double getMaxIntensity() = 0;
	virtual QRgb getColor() = 0;
	virtual Intensity& operator/=(double val) = 0;
	virtual Intensity& operator+=(Intensity& right) = 0;
	virtual Intensity& operator+=(const Color& right) = 0;
	virtual Intensity& operator*=(const Color& right) = 0;
	virtual Intensity& operator*=(double value) = 0;
	virtual Intensity& operator*=(Transmission& transmission) = 0;
	virtual Intensity& operator=(const Intensity& right) = 0;
	
	virtual double& operator[](int index) = 0;
	virtual double getComponent(int index) const = 0;
};
