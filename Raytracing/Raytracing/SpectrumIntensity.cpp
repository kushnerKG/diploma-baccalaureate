#include "SpectrumIntensity.h"
#include "Color.h"
#include "Transmission.h"
#include "crt.h"

SpectrumIntensity::SpectrumIntensity(double* spectr)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = spectr[i];
	}
}

SpectrumIntensity::SpectrumIntensity(Color& color)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = color[i];
	}
}

SpectrumIntensity::SpectrumIntensity(double value)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = value;
	}
}

SpectrumIntensity::SpectrumIntensity()
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = 0.0;
	}
}

SpectrumIntensity::~SpectrumIntensity()
{
}

void SpectrumIntensity::pow(double power)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = std::pow(intensities[i], power);
	}
}

bool SpectrumIntensity::isBackground()
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		if (intensities[i] != -1.0)
		{
			return false;
		}
	}
	return true;
}

double SpectrumIntensity::getMaxIntensity()
{
	double max = intensities[0];
	for (int i = 1; i < SPECTRUM_SIZE; i++)
	{
		max = intensities[i] > max ? intensities[i] : max;
	}
	return max;
}

QRgb SpectrumIntensity::getColor()
{
	double red = 0, green = 0, blue = 0;
	convert_spectrum_to_rgb(sourceSpectrFileName, intensities, SPECTRUM_SIZE, &red, &green, &blue);

	return qRgb(255.0 * red, 255.0 * green, 255.0 * blue);
}

Intensity& SpectrumIntensity::operator/=(double val)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] /= val;
	}

	return *this;
}

Intensity& SpectrumIntensity::operator+=(Intensity& right)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] += right[i];
	}

	return *this;
}

Intensity& SpectrumIntensity::operator+=(const Color& right)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] += right[i];
	}

	return *this;
}

Intensity& SpectrumIntensity::operator*=(const Color& right)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] *= right[i];
	}

	return *this;
}

Intensity& SpectrumIntensity::operator*=(double value)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] *= value;
	}

	return *this;
}

Intensity& SpectrumIntensity::operator*=(Transmission& transmission)
{
	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] *= transmission[i];
	}

	return *this;
}

Intensity& SpectrumIntensity::operator=(const Intensity& right)
{
	if (this == &right)
	{
		return *this;
	}

	for (int i = 0; i < SPECTRUM_SIZE; i++)
	{
		intensities[i] = right.getComponent(i);
	}

	return *this;
}

double& SpectrumIntensity::operator[](int index)
{
	return intensities[index];
}

double SpectrumIntensity::getComponent(int index) const
{
	return intensities[index];
}
