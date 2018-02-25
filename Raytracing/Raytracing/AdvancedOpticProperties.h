#pragma once
#include "OpticProperties.h"
#include "RenderSetting.h"
#include <vector>

class AdvancedOpticProperties :
	public OpticProperties
{
public:
	AdvancedOpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power,
		std::unique_ptr<Color>  transmitted, double* refractionIndex, RenderType renderType);
	AdvancedOpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power,
		std::unique_ptr<Color>  transmitted, double refractionIndex, RenderType renderType);
	AdvancedOpticProperties(const AdvancedOpticProperties& other) = delete;
	AdvancedOpticProperties(AdvancedOpticProperties&& other);

	~AdvancedOpticProperties();

	AdvancedOpticProperties& operator=(const AdvancedOpticProperties& other) = delete;
	AdvancedOpticProperties& operator=(AdvancedOpticProperties&& other);

	const Color& getTransmitted() const;
	const std::vector<double> getN() const;
private:

	std::unique_ptr<Color> transmitted = nullptr;
	std::vector<double> refractionIndex;
	int countRefraction = 0;
};