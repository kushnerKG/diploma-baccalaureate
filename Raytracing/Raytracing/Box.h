#pragma once

#include "Shape.h"
#include "PointVector3D.h"
#include "OpticProperties.h"
#include <ostream>
#include <vector>
#include "LineSegment.h"
class Box : public Shape
{

public:
	Box(PointVector3D minPoint, PointVector3D maxPoint, std::shared_ptr<OpticProperties> opticProperties);
	~Box();


	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	const OpticProperties& getOpticProperties() override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;

	friend std::ostream& operator<<(std::ostream& os, const Box& obj);

private:
	const PointVector3D minPoint;
	const PointVector3D maxPoint;
	std::shared_ptr<OpticProperties> opticProperties;
	
	double xMin = 0, xMax = 0, yMin = 0, yMax = 0, zMin = 0, zMax = 0;
	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformBox = nullptr;


	void calculateLineSegment();

	PointVector3D calculateNormal(PointVector3D& intersection) const;
};

