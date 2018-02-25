#pragma once
#include "SpectrumIntensity.h"
#include "IntensityMap.h"
class SpectrumIntensityMap : IntensityMap
{
public:
	SpectrumIntensityMap(int columns, int rows);
	~SpectrumIntensityMap();

	Intensity& getIntensity(int i, int j) override;
	int getRows() const override;
	int getColumns() const override;
private:
	int columns;
	int rows;
	SpectrumIntensity* array;
};

