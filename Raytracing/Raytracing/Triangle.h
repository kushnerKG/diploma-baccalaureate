#pragma once
#include "Shape.h"
#include "OpticProperties.h"
#include <ostream>

class Triangle : public Shape
{
public:
	Triangle(PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, std::shared_ptr<OpticProperties> opticProperties);
	~Triangle();

	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	const OpticProperties& getOpticProperties() override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;
	friend std::ostream& operator<<(std::ostream& os, const Triangle& obj);

private:

	const PointVector3D vertex1;
	const PointVector3D vertex2;
	const PointVector3D vertex3;
	std::shared_ptr<OpticProperties> opticProperties;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformTriangle = nullptr;

	PointVector3D normal;
	double D;

	PointVector3D projVertex1;
	PointVector3D projVertex2;
	PointVector3D projVertex3;
	int mainCoord;

	void calculateLineSegment();
	double calculateSquare(const PointVector3D& a, const PointVector3D& b, const PointVector3D& c) const;
	void calcauateD();
	void calculateNormal();
};

