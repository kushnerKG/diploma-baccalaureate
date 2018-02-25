#pragma once
#include <memory>
class Color;

class OpticProperties
{
public:

	OpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power);
	virtual ~OpticProperties();

	Color& getDiffuse() const;
	Color& getSpecular() const;

	OpticProperties(const OpticProperties& other) = delete;
	OpticProperties(OpticProperties&& other);

	OpticProperties& operator=(const OpticProperties& other) = delete;
	OpticProperties& operator=(OpticProperties&& other);
	double getPower() const;


private:
	std::unique_ptr<Color> diffuse = nullptr;
	std::unique_ptr<Color> specular = nullptr;
	double power;
};