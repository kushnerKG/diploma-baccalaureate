#include "SpectrumIntensityMap.h"
SpectrumIntensityMap::SpectrumIntensityMap(int columns, int rows) : columns(columns), rows(rows)
{
	array = new SpectrumIntensity[columns * rows];
}

SpectrumIntensityMap::~SpectrumIntensityMap()
{
	delete[] array;
}

Intensity& SpectrumIntensityMap::getIntensity(int i, int j)
{
	return  array[i * columns + j];
}

int SpectrumIntensityMap::getRows() const
{
	return  rows;
}

int SpectrumIntensityMap::getColumns() const
{
	return columns;
}