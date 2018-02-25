#include "Transmission.h"
#include "Color.h"

RenderType CURRENT_RENDER_MODE = RGB_RENDER;

Transmission::Transmission(double value)
{
	len = CURRENT_RENDER_MODE == RGB_RENDER ? 3 : SPECTRUM_SIZE;
	this->transmission = std::vector<double>(len);
	for (double& val : transmission)
	{
		val = value;
	}

}

Transmission::Transmission(RenderType renderType, double value)
{
	len = renderType == RGB_RENDER ? 3 : SPECTRUM_SIZE;
	this->transmission = std::vector<double>(len);
	for (double& val : transmission)
	{
		val = value;
	}
}

Transmission::Transmission(double* transmission, RenderType renderType)
{
	len = renderType == RGB_RENDER ? 3 : SPECTRUM_SIZE;
	this->transmission = std::vector<double>(len);
	for (int i = 0; i < len; i++)
	{
		this->transmission[i] = transmission[i];
	}
}

Transmission::Transmission(const Color& color)
{
	len = color.getSize();
	this->transmission = std::vector<double>(len);
	for (int i = 0; i < len; i++)
	{
		transmission[i] = color[i];
	}
}

Transmission::Transmission(const Color& color1, const Color& color2)
{
	len = color1.getSize();
	this->transmission = std::vector<double>(len);
	for (int i = 0; i < len; i++)
	{
		transmission[i] = color1[i] * color2[i];
	}
}

Transmission& Transmission::operator*=(Transmission& rTransmission)
{
	for (int i = 0; i < len; i++)
	{
		this->transmission[i] *= rTransmission.transmission[i];
	}

	return *this;
}

double& Transmission::operator[](int index)
{
	return transmission[index];
}

Transmission::~Transmission()
{
}
