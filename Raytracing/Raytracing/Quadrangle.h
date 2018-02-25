#pragma once
#include "Shape.h"
#include "PointVector3D.h"
#include "OpticProperties.h"
#include <ostream>
#include "Triangle.h"

class Quadrangle : public Shape
{
public:
	Quadrangle(PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, PointVector3D vertex4, std::shared_ptr<OpticProperties> opticProperties);
	~Quadrangle();

	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	const OpticProperties& getOpticProperties() override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;

	friend std::ostream& operator<<(std::ostream& os, const Quadrangle& obj);

protected:
	Triangle triangle1;
	Triangle triangle2;

private:

	const PointVector3D vertex1;
	const PointVector3D vertex2;
	const PointVector3D vertex3;
	const PointVector3D vertex4;
	std::shared_ptr<OpticProperties> opticProperties;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformQuadrangle = nullptr;

	void calculateLineSegment();
};

