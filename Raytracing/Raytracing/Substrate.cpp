#include "Substrate.h"
#include "Color.h"
#include "RenderSetting.h"
#include "RgbColor.h"
#include "SpectrumColor.h"
#include "AdvancedOpticProperties.h"
#include <QDebug>
#define WIDTH 8
#define HEIGHT 8

double xStart = -9;
double xFinish = 9;
double yStart = 9;
double yFinish = -9;

//задание цветов произвольных
void generateCustomBoard(std::vector<char>& textureMap)//изначально размерность из файла менялась, поэтому динамический массив
{
	char tmp[HEIGHT][WIDTH] ={
		{0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
		{ 0, 1, 0, 1, 0, 1, 0, 1},
	};

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < HEIGHT; x++)
		{
			textureMap[y * HEIGHT + x] = tmp[y][x];
		}
	}
}

void generateChessBoard(std::vector<char>& textureMap)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			if ((x + y) % 2 == 0)
			{
				textureMap[y * HEIGHT + x] = 0;
			}
			else
			{
				textureMap[y * HEIGHT + x] = 1;
			}
		}
	}
}

void generateLabyrinth(std::vector<char>& textureMap)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			if ((y % 2 == 0 && x >= (y - 2) && x <= (HEIGHT - 1 - y)) || (y % 2 == 1 && x >= (HEIGHT - 1 - y) && x <= y))
			{
				textureMap[y * HEIGHT + x] = 0;
			}
			else if (((x % 2) == 0 && y >= (x + 3) && y <= (WIDTH - 2 - x)) || ((x % 2) == 1 && y >= (WIDTH - x) && y <= (x - 1)))
			{
				textureMap[y * HEIGHT + x] = 0;
			}
			else
			{
				textureMap[y * HEIGHT + x] = 1;
			}
		}
	}
}

Substrate::Substrate(RenderType renderType, SUBSTRATE_TYPE type, PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, PointVector3D vertex4, std::shared_ptr<OpticProperties> opticProperties) :
				Quadrangle(vertex1, vertex2, vertex3, vertex4, opticProperties), renderType(renderType), type(type), vertex1(vertex1), vertex2(vertex2), vertex3(vertex3), vertex4(vertex4), opticProperties(opticProperties),
				triangle1(Triangle(vertex1, vertex2, vertex4, opticProperties)), triangle2(Triangle(vertex4, vertex2, vertex3, opticProperties))
{
	///////////////////////////////
	//задание цветов в RGB формате
	std::vector<RgbColor> rgbColors;
	rgbColors.push_back(RgbColor(0, 0, 1));
	rgbColors.push_back(RgbColor(1, 1, 0));
	//rgbColors.push_back(RgbColor(0, 1, 0));
	//rgbColors.push_back(RgbColor(1, 0, 1));
	//rgbColors.push_back(RgbColor(1, 1, 0));
	//rgbColors.push_back(RgbColor(0, 0, 1));
	//rgbColors.push_back(RgbColor(1, 0, 0));
	//задание цветов в RGB формате
	///////////////////////////////

	generateOpticPropertiesForColor(rgbColors);

	textureMap = std::vector<char>(WIDTH * HEIGHT);

	if (type == CHESS_BOARD)
	{
		generateChessBoard(textureMap);
	}
	else if (type == LABIRINTH)
	{
		generateLabyrinth(textureMap);
	} else
	{
		generateCustomBoard(textureMap);
	}
}

void Substrate::generateOpticPropertiesForColor(std::vector<RgbColor>& colors)
{
	float spectrum[SPECTRUM_SIZE];
	if (renderType == RGB_RENDER)
	{
		for (auto rgbColor : colors)
		{
			colorsOpticProprties.push_back(std::make_shared<OpticProperties>(
				std::move(std::make_unique<RgbColor>(rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue())),
				std::move(std::make_unique<RgbColor>(0, 0, 0)),
				Quadrangle::getOpticProperties().getPower()));
		}
	}
	else
	{
		for (auto rgbColor : colors)
		{
			convert_rgb_to_spectrum(sourceSpectrFileName, rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue(), spectrum, SPECTRUM_SIZE);
			std::unique_ptr<Color> blue = std::make_unique<SpectrumColor>(spectrum);

			convert_rgb_to_spectrum(sourceSpectrFileName, 0, 0, 0, spectrum, SPECTRUM_SIZE);
			std::unique_ptr<Color> specular = std::make_unique<SpectrumColor>(spectrum);

			colorsOpticProprties.push_back(std::make_shared<OpticProperties>(
				std::move(blue),
				std::move(specular),
				Quadrangle::getOpticProperties().getPower()));
		}
	}
}

Substrate::~Substrate()
{
}

Shape* Substrate::clone()
{
	return new Substrate(renderType, type, vertex1, vertex2, vertex3, vertex4, opticProperties);
}

std::unique_ptr<IntersectionInfo> Substrate::checkRay(Ray& ray)
{
	std::unique_ptr<IntersectionInfo> c1 = triangle1.checkRay(ray);
	std::unique_ptr<IntersectionInfo> c2 = triangle2.checkRay(ray);

	if (c1 != nullptr){
		c1->setLightSourceIntesection(true);
		const PointVector3D& intersection = c1->getIntersectionPoints();
		c1->setOpticProperties(getOpticalPropertiesByPoint(intersection.getX(), intersection.getY()));
		return c1;
	}
	if (c2 != nullptr){
		c2->setLightSourceIntesection(true);
		const PointVector3D& intersection = c2->getIntersectionPoints();
		c2->setOpticProperties(getOpticalPropertiesByPoint(intersection.getX(), intersection.getY()));
		return c2;
	}

	return nullptr;
}

std::shared_ptr<OpticProperties> Substrate::getOpticalPropertiesByPoint(double x, double y)
{
	double deltaX = (xFinish - xStart) / WIDTH;
	double deltaY = (yFinish - yStart) / HEIGHT;

	int _X = static_cast<int>((x - xStart) / deltaX);
 	int _Y = static_cast<int>((y - yStart) / deltaY);

	return colorsOpticProprties[textureMap[_Y * HEIGHT + _X]];
}