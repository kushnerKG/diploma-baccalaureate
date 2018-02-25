#include "Cylinder.h"
#include "MatrixFactory.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QDebug>
#include "AdvancedOpticProperties.h"
#include "Color.h"


Cylinder::Cylinder(PointVector3D p1, PointVector3D p2, double radius, std::shared_ptr<OpticProperties> advancedOpticProperties, bool isTransparency) :
		p1(p1), p2(p2), radius(radius), opticProperties(advancedOpticProperties),
		fromWorldToModelMatrix(Matrix<double>(4, 4, 0)), fromModelToWorldMatrix(Matrix<double>(4, 4, 0)), isTransparency(isTransparency)
{
	this->advancedOpticProperties = dynamic_cast<AdvancedOpticProperties*>(opticProperties.get());
	buildChangeCoordinateSystemMatrices();
	modelP1 = fromWorldToModelMatrix * p1;
	modelP2 = fromWorldToModelMatrix * p2;
}

Cylinder::~Cylinder()
{
}

Shape* Cylinder::clone()
{
	return new Cylinder(p1, p2, radius, opticProperties, isTrasparency());
}

std::unique_ptr<IntersectionInfo> Cylinder::checkRay(Ray& ray)
{
	PointVector3D start = ray.getRayStart();
	PointVector3D onRay = start + ray.getRayDirection();

	PointVector3D modelStart = fromWorldToModelMatrix * start;
	PointVector3D modelOnRay = fromWorldToModelMatrix * onRay;

	Ray modelRay = Ray(modelStart, modelOnRay, false);
	double x, y, l, m, discriminant;
	x = modelStart.getX();
	y = modelStart.getY();
	l = modelRay.getRayDirection().getX();
	m = modelRay.getRayDirection().getY();

	if (abs(l) < 0.0000000001 && abs(m) < 0.0000000001) //параллельно оси OZ в модельной системе координат
	{
		if (abs(modelRay.getRayStart().getX()) < radius && abs(modelRay.getRayStart().getY()) < radius
			&& abs(modelRay.getRayStart().getX()) > radius && abs(modelRay.getRayStart().getY()) > radius)
		{
			return nullptr;
		}
	}

	discriminant = (2 * x * l + 2 * y * m) * (2 * x * l + 2 * y * m) - 4 * (l * l + m * m) * (x * x - radius * radius + y * y);

	if (discriminant < 0)
	{
		return nullptr;
	}

	double t1, t2;
//	if (abs(discriminant) <= 0.000000000001)//внутри или только граница
//	{
//		qDebug() << "sdvsdvsdvds";
//		t1 = (-(2 * x * l + 2 * y * WIDTH)) / (2 * (l * l + WIDTH * WIDTH));
//		PointVector3D modelIntersection = modelRay.getRayStart() + (modelRay.getRayDirection() * (t1 - 0.00000001));
//		if (modelIntersection.getZ() > modelP2.getZ() || modelIntersection.getZ() < modelP1.getZ() || t1 < 0)
//		{
//			return nullptr;
//		}
//
//		PointVector3D modelNormal = PointVector3D(modelIntersection.getX(), modelIntersection.getY(), 0, 1).getNormolizeVector();
//
//		PointVector3D worldIntersection = fromModelToWorldMatrix * modelIntersection;
//		PointVector3D worldNormal = fromModelToWorldMatrix * modelNormal;
//		worldNormal.normalizePointVector();
//
//		worldNormal *= -1;
//
//		return new IntersectionInfo(worldIntersection, opticProperties, t1, worldNormal, false);
//
//	}
	//Transmission transmission(0,0,0);
	//Intensity* transmission = IntersectionInfo::getDefaultIntensity();
	//std::shared_ptr<Transmission> transmission = std::make_shared<Transmission>();
	bool inner = false;
	bool transparensyFlag = false;
	if (discriminant > 0)
	{
		t1 = (-(2 * x * l + 2 * y * m) - sqrt(discriminant)) / (2 * (l * l + m * m));
		PointVector3D modelIntersection = modelRay.getRayStart() + (modelRay.getRayDirection() * (t1));

		if (modelIntersection.getZ() > modelP2.getZ() || modelIntersection.getZ() < modelP1.getZ() || t1 < 0)
		{
			t2 = (-(2 * x * l + 2 * y * m) + sqrt(discriminant)) / (2 * (l * l + m * m));

			modelIntersection = modelRay.getRayStart() + (modelRay.getRayDirection() * (t2));
			if (modelIntersection.getZ() > modelP2.getZ() || modelIntersection.getZ() < modelP1.getZ() || t2 < 0)
			{
				return nullptr;
			}
			transparensyFlag = true;
			inner = true;
		}
		else
		{
			t2 = (-(2 * x * l + 2 * y * m) + sqrt(discriminant)) / (2 * (l * l + m * m));

			PointVector3D tmp = modelRay.getRayStart() + (modelRay.getRayDirection() * (t2));
			if (!(tmp.getZ() > modelP2.getZ() || tmp.getZ() < modelP1.getZ() || t2 < 0))
			{
				transparensyFlag = true;
			}
		}

		PointVector3D modelNormal = PointVector3D(modelIntersection.getX(), modelIntersection.getY(), 0, 0).getNormolizeVector();
		PointVector3D worldIntersection = fromModelToWorldMatrix * modelIntersection;
		modelNormal[3] = 0.0;
		PointVector3D worldNormal = fromModelToWorldMatrix * modelNormal;
		worldNormal = inner ? worldNormal *= -1 : worldNormal;

		
		return std::make_unique<IntersectionInfo>(worldIntersection, opticProperties, inner ? t2 : t1, worldNormal,
		                                          transparensyFlag ? std::make_shared<Transmission>(advancedOpticProperties->getTransmitted(), advancedOpticProperties->getTransmitted()) :
			                                          std::make_shared<Transmission>(), isTransparency);
	}

	return nullptr;
}

std::shared_ptr<std::vector<LineSegment>> Cylinder::getTransformFigure(Matrix<double> rotateMatrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

	Matrix<double> matrix = rotateMatrix;


	transformCylinder = std::make_shared<std::vector<LineSegment>>();

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

		transformCylinder->push_back(LineSegment(transformP1, transformP2));
	}

	return transformCylinder;
}

std::shared_ptr<std::vector<LineSegment>> Cylinder::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformCylinder == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformCylinder;
}

const OpticProperties& Cylinder::getOpticProperties()
{
	return *opticProperties;
}

QRgb Cylinder::getColor()
{
	return opticProperties->getDiffuse().getRGB();
}

bool Cylinder::isTrasparency()
{
	return isTransparency;
}

bool Cylinder::isLightSource()
{
	return false;
}

void Cylinder::calculateLineSegment()
{
	double step = M_PI * 2 / M;
	std::vector<PointVector3D>* coordinates2Ds = new std::vector<PointVector3D>();
	std::vector<LineSegment>* line2Ds = new std::vector<LineSegment>();

	for (double angle = 0; angle <= 2 * M_PI; angle += step)
	{
		coordinates2Ds->push_back(PointVector3D(radius * cos(angle), radius * sin(angle), modelP1.getZ(), 1));
	}

	double deltaH = (modelP2.getZ() - modelP1.getZ()) / N;
	for (double h = modelP1.getZ(); h <= modelP2.getZ(); h+=deltaH)
	{
		for (int i = 0; i < coordinates2Ds->size() - 1; i++)
		{
			line2Ds->push_back(LineSegment(PointVector3D(coordinates2Ds->at(i).getX(), coordinates2Ds->at(i).getY(), h), 
				PointVector3D(coordinates2Ds->at(i + 1).getX(), coordinates2Ds->at(i + 1).getY(), h)));
		}
		line2Ds->push_back(LineSegment(PointVector3D(coordinates2Ds->at(coordinates2Ds->size() - 1).getX(), coordinates2Ds->at(coordinates2Ds->size() - 1).getY(), h),
			PointVector3D(coordinates2Ds->at(0).getX(), coordinates2Ds->at(0).getY(), h)));
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			line2Ds->push_back(LineSegment(line2Ds->at(i * (M) + j).getStart(), line2Ds->at((i + 1) * (M) + j).getStart()));
		}
	}

	for (LineSegment ls : *line2Ds)
	{
		lineSegments.push_back(LineSegment(fromModelToWorldMatrix * ls.getStart(), fromModelToWorldMatrix * ls.getFinish()));
	}
	
	delete line2Ds;
	delete coordinates2Ds;
}

void Cylinder::buildChangeCoordinateSystemMatrices()
{
	PointVector3D z = p2 - p1;
	z.normalizePointVector();
	PointVector3D yy = (PointVector3D(1, 0, 0) == z || PointVector3D(-1, 0, 0) == z) ? PointVector3D(1, 0, 1) : PointVector3D(1, 0, 0);
	PointVector3D x = yy * z;
	x.normalizePointVector();
	PointVector3D y = z * x;
	y.normalizePointVector();
	PointVector3D O = PointVector3D((p2.getX() + p1.getX()) / 2, (p2.getY() + p1.getY()) / 2, (p2.getZ() + p1.getZ()) / 2);

	Matrix<double> M1 = MatrixFactory::getMoveMatrix(-O.getX(), -O.getY(), -O.getZ());
	Matrix<double> M2 = Matrix<double>(4, 4, 0);
	for (int i = 0; i < 3; i++){
		M2[0][i] = x[i];
		M2[1][i] = y[i];
		M2[2][i] = z[i];
	}
	M2[3][3] = 1.0;
	fromWorldToModelMatrix = M2 * M1;

	Matrix<double> M3 = Matrix<double>(4, 4, 0);
	for (int i = 0; i < 3; i++){
		M3[i][0] = x[i];
		M3[i][1] = y[i];
		M3[i][2] = z[i];
	}
	M3[3][3] = 1.0;
	M1 = MatrixFactory::getMoveMatrix(O.getX(), O.getY(), O.getZ());
	fromModelToWorldMatrix = M1 * M3;
}