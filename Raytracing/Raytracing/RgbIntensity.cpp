#include "RgbIntensity.h"
#include "Color.h"
#include "Transmission.h"
#include "QDebug"

RgbIntensity::RgbIntensity()
{
	intensities[0] = 0.0;
	intensities[1] = 0.0;
	intensities[2] = 0.0;
}

RgbIntensity::RgbIntensity(double redIntensity, double greenIntensity, double blueIntensity)
{
	intensities[0] = redIntensity;
	intensities[1] = greenIntensity;
	intensities[2] = blueIntensity;
}

RgbIntensity::RgbIntensity(Color& color)
{
	intensities[0] = color[0];
	intensities[1] = color[1];
	intensities[2] = color[2];
}

RgbIntensity::RgbIntensity(RgbIntensity& right)
{
	intensities[0] = right.intensities[0];
	intensities[1] = right.intensities[1];
	intensities[2] = right.intensities[2];
}

RgbIntensity::~RgbIntensity()
{
}

void RgbIntensity::pow(double power)
{
	intensities[0] = std::pow(intensities[0], power);
	intensities[1] = std::pow(intensities[1], power);
	intensities[2] = std::pow(intensities[2], power);
}

bool RgbIntensity::isBackground()
{
	if (intensities[0] == -1.0 && intensities[1] == -1.0 && intensities[2] == -1.0)
	{
		return true;
	}
	return false;
}


Intensity& RgbIntensity::operator/=(double val)
{
	intensities[0] /= val;
	intensities[1] /= val;
	intensities[2] /= val;
	return *this;
}

Intensity& RgbIntensity::operator+=(Intensity& right)//по-хорошему нужно бросать исключение если не того типа, но пока пофиг
{
	intensities[0] += right[0];
	intensities[1] += right[1];
	intensities[2] += right[2];

	return *this;
}

Intensity& RgbIntensity::operator*=(const Color& right)
{
	intensities[0] *= right[0];
	intensities[1] *= right[2];
	intensities[2] *= right[2];
	return *this;
}

Intensity& RgbIntensity::operator*=(double value)
{
	intensities[0] *= value;
	intensities[1] *= value;
	intensities[2] *= value;
	return *this;
}

Intensity& RgbIntensity::operator*=(Transmission& transmission)
{
	intensities[0] *= transmission[0];
	intensities[1] *= transmission[1];
	intensities[2] *= transmission[2];

	return *this;
}

Intensity& RgbIntensity::operator+=(const Color& right)
{
	intensities[0] += right[0];
	intensities[1] += right[1];
	intensities[2] += right[2];

	return *this;
}

Intensity& RgbIntensity::operator=(const Intensity& right)
{
	if (this == &right)
	{
		return *this;
	}
	
	intensities[0] = right.getComponent(0);
	intensities[1] = right.getComponent(1);
	intensities[2] = right.getComponent(2);

	return *this;
}

double& RgbIntensity::operator[](int index)
{
	return intensities[index];
}

double RgbIntensity::getComponent(int index) const
{
	return intensities[index];
}

double RgbIntensity::getMaxIntensity()
{
	double max = intensities[0];

	max = intensities[1] > max ? intensities[1] : max;
	max = intensities[2] > max ? intensities[2] : max;

	return max;
}

QRgb RgbIntensity::getColor()
{
	return qRgb(intensities[0] * 255.0, intensities[1] * 255.0, intensities[2] * 255.0);
}
