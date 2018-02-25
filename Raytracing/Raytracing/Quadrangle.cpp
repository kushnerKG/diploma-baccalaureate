#include "Quadrangle.h"
#include "QDebug.h"
#include "Color.h"

Quadrangle::Quadrangle(PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, PointVector3D vertex4, std::shared_ptr<OpticProperties> opticProperties) :
			vertex1(vertex1), vertex2(vertex2), vertex3(vertex3), vertex4(vertex4), opticProperties(opticProperties),
			triangle1(Triangle(vertex1, vertex2, vertex4, opticProperties)), triangle2(Triangle(vertex4, vertex2, vertex3, opticProperties))
{
}

Quadrangle::~Quadrangle()
{
}

Shape* Quadrangle::clone()
{
	return new Quadrangle(vertex1, vertex2, vertex3, vertex4, opticProperties);
}

void Quadrangle::calculateLineSegment()
{
	lineSegments.push_back(LineSegment(vertex1, vertex2));
	lineSegments.push_back(LineSegment(vertex2, vertex3));
	lineSegments.push_back(LineSegment(vertex3, vertex4));
	lineSegments.push_back(LineSegment(vertex4, vertex1));
}

std::unique_ptr<IntersectionInfo> Quadrangle::checkRay(Ray& ray)
{
	std::unique_ptr<IntersectionInfo> c1 = triangle1.checkRay(ray);
	std::unique_ptr<IntersectionInfo> c2 = triangle2.checkRay(ray);

	if (c1 != nullptr){
		c1->setLightSourceIntesection(true);
		return c1;
	}
	if (c2 != nullptr){
		c2->setLightSourceIntesection(true);
		return c2;
	}
	return nullptr;
}

std::shared_ptr<std::vector<LineSegment>> Quadrangle::getTransformFigure(Matrix<double> matrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

	transformQuadrangle = std::make_shared<std::vector<LineSegment>>();

	for (LineSegment segment : lineSegments)
	{
		PointVector3D p1 = segment.getStart();
		PointVector3D p2 = segment.getFinish();

		PointVector3D v1 = matrix * p1;
		PointVector3D v2 = matrix * p2;

		double x1 = v1[0] / v1[3];
		double y1 = v1[1] / v1[3];
		double z1 = v1[2];
		double u1 = v1[3];

		double x2 = v2[0] / v2[3];
		double y2 = v2[1] / v2[3];
		double z2 = v2[2];
		double u2 = v2[3];

		PointVector3D transformP1 = PointVector3D(x1, y1, z1, u1);
		PointVector3D transformP2 = PointVector3D(x2, y2, z2, u2);

		transformQuadrangle->push_back(LineSegment(transformP1, transformP2));
	}

	return transformQuadrangle;
}

std::shared_ptr<std::vector<LineSegment>> Quadrangle::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformQuadrangle == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformQuadrangle;
}

QRgb Quadrangle::getColor()
{
	return opticProperties->getDiffuse().getRGB();
}

bool Quadrangle::isTrasparency()
{
	return false;
}

bool Quadrangle::isLightSource()
{
	return true;
}

const OpticProperties& Quadrangle::getOpticProperties()
{
	return *opticProperties;
}

std::ostream& operator<<(std::ostream& os, const Quadrangle& obj)
{
	return os
		<< " vertex1: " << obj.vertex1
		<< " vertex2: " << obj.vertex2
		<< " vertex3: " << obj.vertex3
		<< " vertex4: " << obj.vertex4
		<< " opticProperties: " << obj.opticProperties << std::endl;
}