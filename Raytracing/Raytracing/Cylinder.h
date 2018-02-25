#pragma once
#include "Shape.h"
#include "AdvancedOpticProperties.h"
class Cylinder :
	public Shape
{
public:
	Cylinder(PointVector3D p1, PointVector3D p2, double radius, std::shared_ptr<OpticProperties> advancedOpticProperties, bool isTransparency);
	~Cylinder();


	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	const OpticProperties& getOpticProperties() override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;

private:
	const double radius;
	const PointVector3D p1;
	const PointVector3D p2;
	PointVector3D modelP1;
	PointVector3D modelP2;
	std::shared_ptr<OpticProperties> opticProperties;
	AdvancedOpticProperties* advancedOpticProperties = nullptr;

	bool isTransparency;

	const static int M = 30;
	const static int N = 10;


	Matrix<double> fromWorldToModelMatrix;
	Matrix<double> fromModelToWorldMatrix;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformCylinder = nullptr;

	void calculateLineSegment();

	void buildChangeCoordinateSystemMatrices();
};

