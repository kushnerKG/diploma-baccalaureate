#include "RgbColor.h"
#include "RgbIntensity.h"
#include "QDebug"

RgbColor::RgbColor(double red, double green, double blue) : red(red), green(green), blue(blue)
{

}

RgbColor::RgbColor(int color)
{
	this->red += (color >> 16) & 0xFF;
	this->green += (color >> 8) & 0xFF;
	this->blue += color & 0xFF;
}

RgbColor::~RgbColor()
{
}

Color* RgbColor::clone()
{
	return new RgbColor(red, green, blue);
}

QRgb RgbColor::getRGB()
{
	return qRgb(red * 255, green * 255, blue * 255);
}

void RgbColor::power(double power)
{
	red = pow(red, power);
	green = pow(green, power);
	blue = pow(blue, power);
}

Color& RgbColor::operator/(double val)
{
	return *(new RgbColor(red/val, green/val, blue/val));
}

Color& RgbColor::operator/=(double val)
{
	red /= val;
	green /= val;
	blue /= val;
	return *this;
}

Intensity& RgbColor::operator*(const Color& right) const
{
	double redIntensity = this->red * right[0];
	double greenIntensity = this->green * right[1];
	double blueIntensity = this->blue * right[2];

	return *(new RgbIntensity(redIntensity, greenIntensity, blueIntensity));
}

double RgbColor::operator[](int index) const
{
	switch (index)
	{
	case 0:
		return red;
	case 1:
		return green;
	case 2:
		return blue;
	default: 
		throw "type of color - RGB, index should be 0 <= index <=2";
	}
}

Intensity& RgbColor::operator*(double val) const
{
	return *(new RgbIntensity(red * val, green * val, blue * val));
}

int RgbColor::getSize() const
{
	return 3;
}

int RgbColor::getRed() const
{
	return red;
}

int RgbColor::getGreen() const
{
	return green;
}

int RgbColor::getBlue() const
{
	return blue;
}

void RgbColor::setRed(int red)
{
	this->red = red;
}

void RgbColor::setGreen(int green)
{
	this->green = green;
}

void RgbColor::setBlue(int blue)
{
	this->blue = blue;
}

std::ostream& operator<<(std::ostream& os, const RgbColor& obj)
{
	return os
		<< "red: " << obj.red
		<< " green: " << obj.green
		<< " color1: " << obj.blue << std::endl;
}
