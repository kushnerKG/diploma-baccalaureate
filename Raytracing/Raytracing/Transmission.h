#pragma once
#include "RenderSetting.h"
#include "crt.h"
#include "Intensity.h"
#include <vector>
class Color;

extern RenderType CURRENT_RENDER_MODE;

class Transmission
{
public:
	explicit Transmission(double value = 0);
	Transmission(RenderType renderType, double value = 0);
	explicit Transmission(double* transmission, RenderType renderType);
	Transmission(const Color& color);
	Transmission(const Color& color1, const Color& color2);

	Transmission& operator*=(Transmission& rTransmission);
	double& operator[](int index);

	~Transmission();
private:
	int len;
	std::vector<double> transmission;
};

