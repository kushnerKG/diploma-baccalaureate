#pragma once
#include "crt.h"
#include "Intensity.h"
class SpectrumIntensity : public Intensity
{
public:
	SpectrumIntensity(double* spectr);
	explicit SpectrumIntensity(Color& color);
	explicit SpectrumIntensity(double value);
	SpectrumIntensity();
	~SpectrumIntensity();

	void pow(double power) override;
	bool isBackground() override;
	double getMaxIntensity() override;
	QRgb getColor() override;
	Intensity& operator/=(double val) override;
	Intensity& operator+=(Intensity& right) override;
	Intensity& operator+=(const Color& right) override;
	Intensity& operator*=(const Color& right) override;
	Intensity& operator*=(double value) override;
	Intensity& operator*=(Transmission& transmission) override;
	Intensity& operator=(const Intensity& right) override;
	double& operator[](int index) override;
	double getComponent(int index) const override;
private:
	double intensities[SPECTRUM_SIZE];
};

