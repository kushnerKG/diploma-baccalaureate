#include "Triangle.h"
#include <cmath>
#include "Color.h"

Triangle::Triangle(PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, std::shared_ptr<OpticProperties> opticProperties) :
					vertex1(vertex1), vertex2(vertex2), vertex3(vertex3), opticProperties(opticProperties)
{
	calculateNormal();
	calcauateD();

	if ((abs(normal.getValue(0)) >= abs(normal.getValue(1))) &&
		(abs(normal.getValue(0)) >= abs(normal.getValue(2)))){
		projVertex1 = PointVector3D(vertex1.getY(), vertex1.getZ(), 0.0);
		projVertex2 = PointVector3D(vertex2.getY(), vertex2.getZ(), 0.0);
		projVertex3 = PointVector3D(vertex3.getY(), vertex3.getZ(), 0.0);
		mainCoord = 0;
	}
	else if ((abs(normal.getValue(1)) >= abs(normal.getValue(0))) &&
		(abs(normal.getValue(1)) >= abs(normal.getValue(2)))){
		projVertex1 = PointVector3D(vertex1.getX(), vertex1.getZ(), 0);
		projVertex2 = PointVector3D(vertex2.getX(), vertex2.getZ(), 0);
		projVertex3 = PointVector3D(vertex3.getX(), vertex3.getZ(), 0);
		mainCoord = 1;
	}
	else{
		projVertex1 = PointVector3D(vertex1.getX(), vertex1.getY(), 0);
		projVertex2 = PointVector3D(vertex2.getX(), vertex2.getY(), 0);
		projVertex3 = PointVector3D(vertex3.getX(), vertex3.getY(), 0);
		mainCoord = 2;
	}
}

Triangle::~Triangle()
{
}

Shape* Triangle::clone()
{
	return new Triangle(vertex1, vertex2, vertex3, opticProperties);
}

void Triangle::calculateLineSegment()
{
	lineSegments.push_back(LineSegment(vertex1, vertex2));
	lineSegments.push_back(LineSegment(vertex2, vertex3));
	lineSegments.push_back(LineSegment(vertex3, vertex1));
}

std::unique_ptr<IntersectionInfo> Triangle::checkRay(Ray& ray)
{
	const PointVector3D& rayStart = ray.getRayStart();
	const PointVector3D& rayirection = ray.getRayDirection();


	double vD = normal | rayirection;

	if (vD >= 0){
		return nullptr;
	}

	double v0 = -(normal | rayStart) - D;
	
	double t = v0 / vD;
	
	if (t < 0){
		return nullptr;
	}

	PointVector3D intersection = rayStart + t * rayirection;

	PointVector3D sepIntersection;

	if (mainCoord == 0){
		sepIntersection = PointVector3D(intersection.getValue(1), intersection.getValue(2), 0);
	}
	else if (mainCoord == 1){
		sepIntersection = PointVector3D(intersection.getValue(0), intersection.getValue(2), 0);
	}
	else{
		sepIntersection = PointVector3D(intersection.getValue(0), intersection.getValue(1), 0);
	}

	double s = calculateSquare(projVertex1, projVertex2, projVertex3);
	
	double alpha = calculateSquare(sepIntersection, projVertex2, projVertex3) / s;
	double betta = calculateSquare(projVertex1, sepIntersection, projVertex3) / s;
	double gamma = calculateSquare(projVertex1, projVertex2, sepIntersection) / s;

	if (abs(alpha + betta + gamma - 1.0) < 0.0000000001 && alpha >= 0 && betta >= 0 && gamma >= 0 &&
		alpha <= 1 && betta <= 1 && gamma <= 1) {
		double len = (rayStart -  intersection).getLen();
		return std::make_unique<IntersectionInfo>(intersection, opticProperties, len, normal, std::make_shared<Transmission>(), isTrasparency());
	}
	return nullptr;
}

double Triangle::calculateSquare(const PointVector3D& a, const PointVector3D& b, const PointVector3D& c) const
{
	return 0.5*((b.getValue(0) - a.getValue(0)) * (c.getValue(1) - a.getValue(1)) -
		(c.getValue(0) - a.getValue(0))*(b.getValue(1) - a.getValue(1)));
}

void Triangle::calcauateD()
{
	double A = normal.getValue(0);
	double B = normal.getValue(1);
	double C = normal.getValue(2);

	D = -A * vertex1.getX() - B * vertex1.getY() - C * vertex1.getZ();
}

void Triangle::calculateNormal()
{
	PointVector3D v1 = vertex2 - vertex1;
	PointVector3D v2 = vertex3 - vertex1;
	
	normal = v1*v2;
	normal.normalizePointVector();
}

std::shared_ptr<std::vector<LineSegment>> Triangle::getTransformFigure(Matrix<double> matrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

	transformTriangle = std::make_shared<std::vector<LineSegment>>();
	
	for (LineSegment segment: lineSegments)
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

		transformTriangle->push_back(LineSegment(transformP1, transformP2));
	}

	return transformTriangle;
}

std::shared_ptr<std::vector<LineSegment>> Triangle::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformTriangle == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformTriangle;
}

QRgb Triangle::getColor()
{
	return opticProperties->getDiffuse().getRGB();
}

bool Triangle::isTrasparency()
{
	return false;
}

bool Triangle::isLightSource()
{
	return false;
}

const OpticProperties& Triangle::getOpticProperties()
{
	return *opticProperties;
}

std::ostream& operator<<(std::ostream& os, const Triangle& obj)
{
	return os
		<< "vertex1: " << obj.vertex1
		<< " vertex2: " << obj.vertex2
		<< " vertex3: " << obj.vertex3
		<< " opticProperties: " << obj.opticProperties;
}
