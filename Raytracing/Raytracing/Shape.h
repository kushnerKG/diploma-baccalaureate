#pragma once
#include <memory>
#include "LineSegment.h"
#include "Matrix.h"
#include "OpticProperties.h"
#include "IntersectionInfo.h"
#include "Ray.h"

#define INTERSECTION_DELTA 0.0000000005

class Shape
{

public:
	virtual ~Shape()
	{
	}

	virtual Shape* clone() = 0;
	virtual std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) = 0;
	virtual std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) = 0;
	virtual std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) = 0;
	virtual const OpticProperties& getOpticProperties() = 0;
	virtual QRgb getColor() = 0;
	virtual bool isTrasparency() = 0;
	virtual bool isLightSource() = 0;
};
