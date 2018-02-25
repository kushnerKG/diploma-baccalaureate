#include "AdvancedOpticProperties.h"
#include "Color.h"
#include <vector>
#include "crt.h"


AdvancedOpticProperties::AdvancedOpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power,
	std::unique_ptr<Color> transmitted, double* refractionIndex, RenderType renderType):
							OpticProperties(std::move(diffuse), std::move(specular), power), transmitted(std::move(transmitted))
{
	countRefraction = renderType == RGB_RENDER ? 1 : SPECTRUM_SIZE;
	this->refractionIndex = std::vector<double>(countRefraction);

	for (int i = 0; i < countRefraction; i++)
	{
		this->refractionIndex[i] = refractionIndex[i];
	}
}

AdvancedOpticProperties::AdvancedOpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power,
	std::unique_ptr<Color> transmitted, double refractionIndex, RenderType renderType):
							OpticProperties(std::move(diffuse), std::move(specular), power), transmitted(std::move(transmitted))
{
	countRefraction = renderType == RGB_RENDER ? 1 : SPECTRUM_SIZE;
	this->refractionIndex = std::vector<double>(countRefraction);

	for (int i = 0; i < countRefraction; i++)
	{
		this->refractionIndex[i] = refractionIndex;
	}
}


AdvancedOpticProperties::~AdvancedOpticProperties()
{
}

AdvancedOpticProperties::AdvancedOpticProperties(AdvancedOpticProperties&& other): OpticProperties(std::move(other)),
                                                                                   transmitted(std::move(other.transmitted))
{
	other.transmitted = nullptr;
	for (int i = 0; i < countRefraction; i++)
	{
		this->refractionIndex[i] = other.refractionIndex[i];
	}
}

AdvancedOpticProperties& AdvancedOpticProperties::operator=(AdvancedOpticProperties&& other)
{
	if (this == &other)
		return *this;
	OpticProperties::operator =(std::move(other));
	transmitted = std::move(other.transmitted);
	for (int i = 0; i < countRefraction; i++)
	{
		this->refractionIndex[i] = other.refractionIndex[i];
	}
	other.transmitted = nullptr;
	return *this;
}

const Color& AdvancedOpticProperties::getTransmitted() const
{
	return *transmitted;
}

const std::vector<double> AdvancedOpticProperties::getN() const
{
	return refractionIndex;
}