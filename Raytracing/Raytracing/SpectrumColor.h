#pragma once
#include "Color.h"
#include "crt.h"

class SpectrumColor :
	public Color
{
public:
	explicit SpectrumColor(double* spectr);
	explicit SpectrumColor(float* spectr);
	explicit SpectrumColor(double val);
	~SpectrumColor();

	Color* clone() override;
	QRgb getRGB() override;
	void power(double power) override;
	Color& operator/(double val) override;
	Color& operator/=(double val) override;
	Intensity& operator*(const Color& right) const override;
	Intensity& operator*(double val) const override;
	double operator[](int index) const override;
	int getSize() const override;
private:

	double spectr[SPECTRUM_SIZE];

};

