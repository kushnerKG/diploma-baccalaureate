#include "SpectrumColor.h"
#include "SpectrumIntensity.h"
#include "RgbIntensity.h"


SpectrumColor::SpectrumColor(double* spectr)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		this->spectr[i] = spectr[i];
	}
}

SpectrumColor::SpectrumColor(float* spectr)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		this->spectr[i] = spectr[i];
	}
}

SpectrumColor::SpectrumColor(double val)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		this->spectr[i] = val;
	}
}

SpectrumColor::~SpectrumColor()
{
}

Color* SpectrumColor::clone()
{
	return new SpectrumColor(this->spectr);
}

QRgb SpectrumColor::getRGB()
{	
	double red, green, blue;
	convert_spectrum_to_rgb(std::string(sourceSpectrFileName).c_str(), this->spectr, SPECTRUM_SIZE, &red, &green, &blue);
	return qRgb(255.0 * red, 255.0 * green, 255.0 * blue);
}

void SpectrumColor::power(double power)
{
	for (double& val : this->spectr)
	{
		val = pow(val, power);
	}
}

Color& SpectrumColor::operator/(double val)
{
	double tmp[SPECTRUM_SIZE];

	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		tmp[i] = this->spectr[i] / val;
	}

	return *(new SpectrumColor(tmp));
}

Color& SpectrumColor::operator/=(double val)
{
	for (double& element : this->spectr)
	{
		element /= val;
	}

	return *this;
}

Intensity& SpectrumColor::operator*(const Color& right) const
{
	double spectr[SPECTRUM_SIZE];
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		spectr[i] = this->spectr[i] * right[i];
	}
	Intensity* intensity = new SpectrumIntensity(spectr);
	return *intensity;
}

Intensity& SpectrumColor::operator*(double val) const
{
	double spectr[SPECTRUM_SIZE];
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		spectr[i] = this->spectr[i] * val;
	}
	Intensity* intensity = new SpectrumIntensity(spectr);
	return *intensity;
}

double SpectrumColor::operator[](int index) const
{
	return this->spectr[index];
}

int SpectrumColor::getSize() const
{
	return SPECTRUM_SIZE;
}
