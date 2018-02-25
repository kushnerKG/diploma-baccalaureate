#pragma once
#include "Intensity.h"
class IntensityMap
{

public:
	virtual ~IntensityMap()
	{
	}
	virtual Intensity& getIntensity(int i, int j) = 0;
	virtual int getRows() const = 0;
	virtual int getColumns() const = 0;
};
