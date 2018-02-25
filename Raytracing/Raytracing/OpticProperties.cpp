#include "OpticProperties.h"
#include "Color.h"

OpticProperties::OpticProperties(std::unique_ptr<Color> diffuse, std::unique_ptr<Color> specular, double power) :
									diffuse(std::move(diffuse)), specular(std::move(specular)), power(power) 
{
}

OpticProperties::~OpticProperties()
{

}

Color& OpticProperties::getDiffuse() const
{
	return *diffuse;
}

Color& OpticProperties::getSpecular() const
{
	return *specular;
}

OpticProperties::OpticProperties(OpticProperties&& other): diffuse(std::move(other.diffuse)),
                                                           specular(std::move(other.specular)),
                                                           power(other.power)
{
	other.diffuse = nullptr;
	other.specular = nullptr;
}

OpticProperties& OpticProperties::operator=(OpticProperties&& other)
{
	if (this == &other)
		return *this;
	diffuse = std::move(other.diffuse);
	specular = std::move(other.specular);
	power = other.power;

	return *this;
}

double OpticProperties::getPower() const
{
	return power;
}