#pragma once
#include "RgbIntensity.h"
#include "IntensityMap.h"

class RgbIntensityMap : public IntensityMap
{
public:
	RgbIntensityMap(int rows, int columns);
	~RgbIntensityMap();

	Intensity& getIntensity(int i, int j) override;
	int getRows() const override;
	int getColumns() const override;
private:
	RgbIntensity* array;

	int rows;
	int columns;
};