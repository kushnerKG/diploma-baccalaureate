#include "RgbIntensityMap.h"
#include <QDebug>
RgbIntensityMap::RgbIntensityMap(int rows, int columns) : rows(rows), columns(columns)
{
	array = new RgbIntensity[rows * columns];
}

RgbIntensityMap::~RgbIntensityMap()
{
	delete[] array;
}

Intensity& RgbIntensityMap::getIntensity(int i, int j)
{
	return  array[i * columns + j];
}

int RgbIntensityMap::getRows() const
{
	return rows;
}

int RgbIntensityMap::getColumns() const
{
	return columns;
}