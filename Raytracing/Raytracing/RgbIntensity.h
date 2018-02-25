#pragma once
#include "Intensity.h"
#include "RgbColor.h"

static int count = 0;

class RgbIntensity : public Intensity
{
public:
	RgbIntensity();
	RgbIntensity(double redIntensity, double greenIntensity, double blueIntensity);
	explicit RgbIntensity(Color& color);
	explicit RgbIntensity(RgbIntensity& right);
	~RgbIntensity();

	void pow(double power) override;
	bool isBackground() override;
	double getMaxIntensity() override;
	QRgb getColor() override;

	Intensity& operator/=(double val) override;
	Intensity& operator+=(Intensity& right) override;//по-хорошему нужно бросать исключение если не того типа, но пока пофиг
	Intensity& operator*=(const Color& right) override;
	Intensity& operator*=(double value) override;
	Intensity& operator*=(Transmission& transmission) override;
	Intensity& operator+=(const Color& right) override;
	Intensity& operator=(const Intensity& right) override;
	double& operator[](int index) override;
	double getComponent(int index) const override;

	
private:
	double intensities[3];
};
