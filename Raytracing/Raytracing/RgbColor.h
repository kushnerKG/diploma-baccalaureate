#pragma once
#include <ostream>
#include "Color.h"

class RgbColor : public Color
{
public:
	RgbColor(double red = 0, double green = 0, double blue = 0);
	
	explicit RgbColor(int color);
	~RgbColor();
	
	Color* clone() override;
	QRgb getRGB() override;
	void power(double power) override;
	Color& operator/(double val) override;
	Color& operator/=(double val) override;
	Intensity& operator*(const Color& right) const override;
	double operator[](int index) const override;
	Intensity& operator*(double val) const override;
	int getSize() const override;
	int getRed() const;
	int getGreen() const;
	int getBlue() const;

	void setRed(int red);
	void setGreen(int green);
	void setBlue(int blue);

	friend std::ostream& operator<<(std::ostream& os, const RgbColor& obj);

private:
	double red;
	double green;
	double blue;
};

