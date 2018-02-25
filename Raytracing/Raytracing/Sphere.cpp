#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "MatrixFactory.h"
#include <QDebug>
#include "SceneFileInfo.h"
#include "AdvancedOpticProperties.h"

Sphere::Sphere(PointVector3D center, double radius, std::shared_ptr<OpticProperties> opticProperties, bool isTransparency) :
			center(center), radius(radius), opticProperties(opticProperties), isTransparency(isTransparency)
{
	
	if (isTransparency)
	{
		this->advancedOpticProperties = dynamic_cast<AdvancedOpticProperties*>(opticProperties.get());
	}
}

Sphere::~Sphere()
{

}

Shape* Sphere::clone()
{
	return new Sphere(center, radius, opticProperties, isTransparency);
}

std::unique_ptr<IntersectionInfo> Sphere::checkRay(Ray& ray)
{
	PointVector3D OC = center - ray.getRayStart();

	bool inner = false;
	if (OC.getLen() < radius)
	{
		inner = true;
	}

	double ca = OC | ray.getRayDirection();//скалярное произведение

	if (ca < 0 && !inner)
	{
		return nullptr;
	}

	double ocLen = OC.getLen();
	double hcInPow2 = radius*radius - ocLen*ocLen + ca * ca; 

	if (hcInPow2 < 0)
	{
		return nullptr;
	}

	double hc = sqrt(hcInPow2);
	double t;
	if (!inner)
	{
		t = ca - hc;
	}
	else
	{
		t = ca + hc;
	}

	PointVector3D intersection = ray.getRayStart() + t * ray.getRayDirection();
	PointVector3D normal = (intersection - center) / radius;

//	if (ray.isInner())
//	{
//		normal *= -1;
//	}

	if (inner)
	{
		normal *= -1;
	}

	return std::make_unique<IntersectionInfo>(intersection, opticProperties, t, normal,
	                                          !isTransparency ? std::make_shared<Transmission>() :
		                                          inner ? std::make_shared<Transmission>(advancedOpticProperties->getTransmitted()) :
		                                          std::make_shared<Transmission>(advancedOpticProperties->getTransmitted(), advancedOpticProperties->getTransmitted()), isTransparency);
}

const OpticProperties& Sphere::getOpticProperties()
{
	return *opticProperties;
}

std::shared_ptr<std::vector<LineSegment>> Sphere::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformSphere == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformSphere;
}

std::shared_ptr<std::vector<LineSegment>> Sphere::getTransformFigure(Matrix<double> rotateMatrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

	Matrix<double> matrix = MatrixFactory::getMoveMatrix(center.getX(), center.getY(), center.getZ());
	matrix = rotateMatrix * matrix;

	transformSphere = std::make_shared<std::vector<LineSegment>>();

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

		transformSphere->push_back(LineSegment(transformP1, transformP2));
	}

	return transformSphere;
}

QRgb Sphere::getColor()
{
	return opticProperties->getDiffuse().getRGB();
}

bool Sphere::isTrasparency()
{
	return isTransparency;
}

bool Sphere::isLightSource()
{
	return false;
}

PointVector3D Sphere::getCenter() const
{
	return center;
}

double Sphere::getRadius() const
{
	return radius;
}

void Sphere::calculateLineSegment()
{
	double step = M_PI * 2 / M;

	std::vector<PointVector3D>* coordinates2Ds = new std::vector<PointVector3D>();

	for (double angle = 0; angle <= 2 * M_PI; angle += step)
	{
		coordinates2Ds->push_back(PointVector3D(radius * cos(angle), radius * sin(angle), 0, 0));
	}

	std::vector<std::vector<PointVector3D>*>* xSections = createXSections(coordinates2Ds);
	std::vector<std::vector<PointVector3D>*>* phSections = createPHSections(xSections);


	for (std::vector<PointVector3D>* coordinates3Ds : *xSections)
	{
		for (int i = 0; i < coordinates3Ds->size() - 1; i++){
			lineSegments.push_back(LineSegment(coordinates3Ds->at(i), coordinates3Ds->at(i + 1)));
		}
	}

	for (std::vector<PointVector3D>* coordinates3Ds : *phSections)
	{
		for (int i = 0; i < coordinates3Ds->size() - 1; i++){
			lineSegments.push_back(LineSegment(coordinates3Ds->at(i), coordinates3Ds->at(i + 1)));
		}
	}
	
	for (std::vector<PointVector3D>* section: *xSections)
	{
		delete section;
	}


	for (std::vector<PointVector3D>* section : *phSections)
	{
		delete section;
	}
	
	delete xSections;
	delete phSections;
	delete coordinates2Ds;
}

std::vector<std::vector<PointVector3D>*>* Sphere::createXSections(std::vector<PointVector3D>* coord2D) const
{
	double deltaPH = M_PI * 2 / M;
	std::vector<std::vector<PointVector3D>*>* xSection = new std::vector<std::vector<PointVector3D>*>();

	for (double ph = 0; ph <= 2 * M_PI; ph += deltaPH){

		std::vector<PointVector3D>* coordinates = new std::vector<PointVector3D>();
		Matrix<double> transformnMatrix = createTransformationMatrix(ph);

		for (PointVector3D coord: *coord2D)
		{
			coordinates->push_back(transformnMatrix * PointVector3D(coord.getX(), coord.getY(), coord.getY(), 1));
		}
	
		xSection->push_back(coordinates);
	}

	return xSection;
}

std::vector<std::vector<PointVector3D>*>* Sphere::createPHSections(std::vector<std::vector<PointVector3D>*>* xSections) const
{
	std::vector<std::vector<PointVector3D>*>* phSections = new std::vector<std::vector<PointVector3D>*>();

	for (int i = 0; i < xSections->size(); i++)
	{
		std::vector<PointVector3D>* section = new std::vector<PointVector3D>();

		for (std::vector<PointVector3D>* tmp : *xSections)
		{
			section->push_back(PointVector3D(tmp->at(i).getX(), tmp->at(i).getY(), tmp->at(i).getZ(), 1));
		}

		phSections->push_back(section);
	}
	
	return  phSections;
}


inline Matrix<double> Sphere::createTransformationMatrix(double ph) const
{
	Matrix<double> matrix(4, 4, 0);
	matrix[0][0] = 1;
	matrix[1][1] = cos(ph);
	matrix[2][2] = sin(ph);
	matrix[3][3] = 1;

	return matrix;
}

std::ostream& operator<<(std::ostream& os, const Sphere& obj)
{
	return os
		<< " center: " << obj.center 
		<< " radius: " << obj.radius << std::endl
		<< " opticProperties: " << obj.opticProperties;
}
