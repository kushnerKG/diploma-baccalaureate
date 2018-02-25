#include "Box.h"
#include "MatrixFactory.h"
#include <QDebug>
#include "Color.h"

Box::Box(PointVector3D minPoint, PointVector3D maxPoint, std::shared_ptr<OpticProperties> opticProperties) :
		minPoint(minPoint), maxPoint(maxPoint), opticProperties(opticProperties)
{
}

Box::~Box()
{

}

Shape* Box::clone()
{
	return new Box(minPoint, maxPoint, opticProperties);
}

std::unique_ptr<IntersectionInfo> Box::checkRay(Ray& ray)
{
	double tNear = std::numeric_limits<double>::min();
	double tFar = std::numeric_limits<double>::max();
	double t1, t2;

	// для x=const плоскостей 
	if (ray.getRayDirection().getValue(0) == 0.0){
		double x = ray.getRayStart().getValue(0);
		if (x < minPoint.getX() || x > maxPoint.getX()){
			return nullptr;
		}
	}

	t1 = (minPoint.getX() - ray.getRayStart().getValue(0)) / ray.getRayDirection().getValue(0);
	t2 = (maxPoint.getX() - ray.getRayStart().getValue(0)) / ray.getRayDirection().getValue(0);

	if (t2 < t1){
		double tmp;
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	if (t1 > tNear){
		tNear = t1;
	}

	if (t2 < tFar){
		tFar = t2;
	}

	if (tNear > tFar){
		return nullptr;
	}

	if (tFar < 0.0000000001){
		return nullptr;
	}


	//для y=const плоскостей

	if (ray.getRayDirection().getValue(1) == 0.0){
		double x = ray.getRayStart().getValue(1);
		if (x < minPoint.getY() || x > maxPoint.getY()){
			return nullptr;
		}
	}

	t1 = (minPoint.getY() - ray.getRayStart().getValue(1)) / ray.getRayDirection().getValue(1);
	t2 = (maxPoint.getY() - ray.getRayStart().getValue(1)) / ray.getRayDirection().getValue(1);

	if (t2 < t1){
		double tmp;
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	if (t1 > tNear){
		tNear = t1;
	}

	if (t2 < tFar){
		tFar = t2;
	}

	if (tNear > tFar){
		return nullptr;
	}

	if (tFar < 0.0000000001){
		return nullptr;
	}

	//для z=const плоскостей
	if (ray.getRayDirection().getValue(2) == 0.0){
		double x = ray.getRayStart().getValue(2);
		if (x < minPoint.getZ() || x > maxPoint.getZ()){
			return nullptr;
		}
	}

	t1 = (minPoint.getZ() - ray.getRayStart().getValue(2)) / ray.getRayDirection().getValue(2);
	t2 = (maxPoint.getZ() - ray.getRayStart().getValue(2)) / ray.getRayDirection().getValue(2);

	if (t2 < t1){
		double tmp;
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	if (t1 > tNear){
		tNear = t1;
	}

	if (t2 < tFar){
		tFar = t2;
	}

	if (tNear > tFar){
		return nullptr;
	}

	if (tFar < 0.0000000001){
		return nullptr;
	}

	if (tNear < 0.0000000001){
		return nullptr;
	}

	double t = tNear;

	PointVector3D intersection = ray.getRayStart() + t * ray.getRayDirection();

	return std::make_unique<IntersectionInfo>(intersection, opticProperties, t, calculateNormal(intersection),
	                                          std::make_shared<Transmission>(), isTrasparency());
}

PointVector3D Box::calculateNormal(PointVector3D& intersection) const
{
	double x = intersection.getValue(0);
	double y = intersection.getValue(1);
	double z = intersection.getValue(2);

	double xMin = minPoint.getX();
	double yMin = minPoint.getY();
	double zMin = minPoint.getZ();

	double xMax = maxPoint.getX();
	double yMax = maxPoint.getY();
	double zMax = maxPoint.getZ();

	if (abs(x - xMin) < 0.0000001){
		return PointVector3D(-1.0, 0, 0);
	}

	if (abs(x - xMax) < 0.0000001){
		return PointVector3D(1.0, 0, 0);
	}
	//////////////////////////
	if (abs(y - yMin) < 0.0000001){
		return PointVector3D(0, -1.0, 0);
	}

	if (abs(y - yMax) < 0.0000001){
		return PointVector3D(0, 1.0, 0);
	}
	//////////////////////////
	if (abs(z - zMin) < 0.0000001){
		return PointVector3D(0, 0, -1.0);
	}

	if (abs(z - zMax) < 0.0000001){
		return PointVector3D(0, 0, 1.0);
	}

	return PointVector3D(0, 0, 0);
}

std::shared_ptr<std::vector<LineSegment>> Box::getTransformFigure(Matrix<double> rotateMatrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

	transformBox = std::make_shared<std::vector<LineSegment>>();

	double xMin = 0, xMax = 0, yMin = 0, yMax = 0, zMin = 0, zMax = 0;

	bool set = false;

	for (LineSegment ls : lineSegments){
		const PointVector3D& p1 = ls.getStart();
		const PointVector3D& p2 = ls.getFinish();

		PointVector3D v1 = rotateMatrix * p1;
		PointVector3D v2 = rotateMatrix * p2;

		double x1 = v1[0] / v1[3];
		double y1 = v1[1] / v1[3];
		double z1 = v1[2];
		double u1 = v1[3];

		double x2 = v2[0] / v2[3];
		double y2 = v2[1] / v2[3];
		double z2 = v2[2];
		double u2 = v2[3];

		if (!set){
			xMin = x1;
			yMin = y1;
			zMin = z1;
			xMax = x1;
			yMax = y1;
			zMax = z1;
			set = true;
		}
		xMax = x1 > xMax ? x1 : xMax;
		yMax = y1 > yMax ? y1 : yMax;
		zMax = z1 > zMax ? z1 : zMax;
		xMin = x1 < xMin ? x1 : xMin;
		yMin = y1 < yMin ? y1 : yMin;
		zMin = z1 < zMin ? z1 : zMin;

		xMax = x2 > xMax ? x2 : xMax;
		yMax = y2 > yMax ? y2 : yMax;
		zMax = z2 > zMax ? z2 : zMax;
		xMin = x2 < xMin ? x2 : xMin;
		yMin = y2 < yMin ? y2 : yMin;
		zMin = z2 < zMin ? z2 : zMin;

		PointVector3D transformP1 = PointVector3D(x1, y1, z1, u1);
		PointVector3D transformP2 = PointVector3D(x2, y2, z2, u2);

		transformBox->push_back(LineSegment(transformP1, transformP2));
	}


	this->xMin = xMin;
	this->yMin = yMin;
	this->zMin = zMin;
	this->xMax = xMax;
	this->yMax = yMax;
	this->zMax = zMax;

	return transformBox;
}

std::shared_ptr<std::vector<LineSegment>> Box::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformBox == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}
	
	return transformBox;
}

const OpticProperties& Box::getOpticProperties()
{
	return *opticProperties;
}

QRgb Box::getColor()
{
	return opticProperties->getDiffuse().getRGB();
}

bool Box::isTrasparency()
{
	return false;
}

bool Box::isLightSource()
{
	return false;
}

void Box::calculateLineSegment() 
{
	double deltaX;
	double deltaY;

	deltaX = maxPoint.getX() - minPoint.getX();
	deltaY = maxPoint.getY() - minPoint.getY();
	maxPoint.getZ() - maxPoint.getY();

	PointVector3D vertex1 = minPoint;
	PointVector3D vertex2(vertex1.getX() + deltaX, vertex1.getY(), vertex1.getZ());
	PointVector3D vertex3(vertex1.getX(), vertex1.getY() + deltaY, vertex1.getZ());
	PointVector3D vertex4(vertex1.getX() + deltaX, vertex1.getY() + deltaY, vertex1.getZ());

	PointVector3D vertex5 = maxPoint;
	PointVector3D vertex6(vertex5.getX() - deltaX, vertex5.getY(), vertex5.getZ());
	PointVector3D vertex7(vertex5.getX(), vertex5.getY() - deltaY, vertex5.getZ());
	PointVector3D vertex8(vertex5.getX() - deltaX, vertex5.getY() - deltaY, vertex5.getZ());
	
	lineSegments.push_back(LineSegment(vertex1, vertex2));
	lineSegments.push_back(LineSegment(vertex1, vertex3));
	lineSegments.push_back(LineSegment(vertex2, vertex4));
	lineSegments.push_back(LineSegment(vertex3, vertex4));
	
	lineSegments.push_back(LineSegment(vertex5, vertex6));
	lineSegments.push_back(LineSegment(vertex5, vertex7));
	lineSegments.push_back(LineSegment(vertex8, vertex6));
	lineSegments.push_back(LineSegment(vertex8, vertex7));

	lineSegments.push_back(LineSegment(vertex1, vertex8));
	lineSegments.push_back(LineSegment(vertex2, vertex7));
	lineSegments.push_back(LineSegment(vertex3, vertex6));
	lineSegments.push_back(LineSegment(vertex4, vertex5));
}

std::ostream& operator<<(std::ostream& os, const Box& obj)
{
	return os
		<< "minPoint: " << obj.minPoint
		<< " maxPoint: " << obj.maxPoint
		<< " opticProperties: " << obj.opticProperties << std::endl;
}
