#include "Lens.h"
#include <math.h>
#include "MatrixFactory.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QDebug>
#include "Color.h"

Lens::Lens(PointVector3D center1, double radius1, PointVector3D center2, double radius2, std::shared_ptr<OpticProperties> opticProperties) :
	center1(center1), radius1(radius1), center2(center2), radius2(radius2), sphere1(Sphere(center1, radius1, opticProperties, true)),
	sphere2(Sphere(center2, radius2, opticProperties, true)), advancedOpticProperties(opticProperties)
{
}


Lens::~Lens()
{
}

Shape* Lens::clone()
{
	return new Lens(center1, radius1, center2, radius2, advancedOpticProperties);
}

std::unique_ptr<IntersectionInfo> Lens::checkRay(Ray& ray)
{
	Index* indexs;
	int size = 0;
	double t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	checkSphere(sphere1, ray, t1, t2);
	size = t1 != -1 ? size + 1 : size;
	size = t2 != -1 ? size + 1 : size;

	checkSphere(sphere2, ray, t3, t4);
	size = t3 != -1 ? size + 1 : size;
	size = t4 != -1 ? size + 1 : size;

	if (!ray.isInner() && (size == 0 || size == 2 || size == 1))
	{
		return nullptr;
	}

	indexs = new Index[size];
	int count = 0;
	
	if (t1 != -1)
	{
		indexs[count].t = t1;
		indexs[count].id = 1;
		count++;
	}

	if (t2 != -1)
	{
		indexs[count].t = t2;
		indexs[count].id = 1;
		count++;
	}

	if (t3 != -1)
	{
		indexs[count].t = t3;
		indexs[count].id = 2;
		count++;
	}

	if (t4 != -1)
	{
		indexs[count].t = t4;
		indexs[count].id = 2;
		count++;
	}

	sortIndexeds(indexs, size);

	if (!ray.isInner())
	{
		if (count == 4){
			std::unique_ptr<IntersectionInfo> intersection = std::unique_ptr<IntersectionInfo>(checkOutsideRayFor4Intersects(ray, indexs));
			delete[] indexs;
			return intersection;
		}
		if (count == 3)
		{
			std::unique_ptr<IntersectionInfo> intersection = std::unique_ptr<IntersectionInfo>(checkOutsideRayFor3Intersects(ray, indexs));
			delete[] indexs;
			return intersection;
		}
	}
	else
	{			
		if (count == 2){
			std::unique_ptr<IntersectionInfo> intersection = std::unique_ptr<IntersectionInfo>(checkInnerRayFor2Intersects(ray, indexs));
			delete[] indexs;
			return intersection;
		}
	}

	delete[] indexs;
	return nullptr;
}

IntersectionInfo* Lens::checkOutsideRayFor4Intersects(Ray& ray, Index* indexs) const
{
	if (indexs[0].id == 1 && indexs[1].id == 2 && indexs[2].id == 1 && indexs[3].id == 2)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[1].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center2) / radius2;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[1].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}
	if (indexs[0].id == 2 && indexs[1].id == 1 && indexs[2].id == 2 && indexs[3].id == 1)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[1].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center1) / radius1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[1].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	if (indexs[0].id == 1 && indexs[1].id == 2 && indexs[2].id == 2 && indexs[3].id == 1)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[1].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center2) / radius2;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[1].t, normal,
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	if (indexs[0].id == 2 && indexs[1].id == 1 && indexs[2].id == 1 && indexs[3].id == 2)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[1].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center1) / radius1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[1].t, normal,
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	return nullptr;
}

IntersectionInfo* Lens::checkOutsideRayFor3Intersects(Ray& ray, Index* indexs) const
{
	if (indexs[0].id == 2 && indexs[1].id == 1 && indexs[2].id == 2)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center2) / radius2;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	if (indexs[0].id == 1 && indexs[1].id == 2 && indexs[2].id == 1)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center1) / radius1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	if (indexs[0].id == 2 && indexs[1].id == 2 && indexs[2].id == 1)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center2) / radius2;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	if (indexs[0].id == 1 && indexs[1].id == 1 && indexs[2].id == 2)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center1) / radius1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	return nullptr;
}

IntersectionInfo* Lens::checkInnerRayFor2Intersects(Ray& ray, Index* indexs) const
{
	if (indexs[0].id == 1 && indexs[1].id == 2)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center1) / radius1;
		normal *= -1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}
	if (indexs[0].id == 2 && indexs[1].id == 1)
	{
		PointVector3D intersection = ray.getRayStart() + (indexs[0].t * ray.getRayDirection());
		PointVector3D normal = (intersection - center2) / radius2;
		normal *= -1;
		AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
		return new IntersectionInfo(intersection, advancedOpticProperties, indexs[0].t, normal, 
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()), true);
	}

	return nullptr;
}

void Lens::sortIndexeds(Index* indexes, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = size - 1; j > i; j--)
		{
			if (indexes[j].t < indexes[j - 1].t)
			{
				Index tmp = indexes[j];
				indexes[j] = indexes[j - 1];
				indexes[j - 1] = tmp;
			}
		}
	}
}

void Lens::checkSphere(const Sphere& sphere, Ray& ray, double& t1, double& t2) const
{
	PointVector3D center = sphere.getCenter();
	double radius = sphere.getRadius();

	PointVector3D OC = center - ray.getRayStart();

	bool inner = false;
	if (OC.getLen() < radius)
	{
		inner = true;
	}

	double ca = OC | ray.getRayDirection();//скалярное произведение
	 
	if (ca < 0 && !inner)
	{
		t1 = -1;
		t2 = -1;
		return;
	}

	double ocLen = OC.getLen();
	double hcInPow2 = radius*radius - ocLen*ocLen + ca * ca;

	if (hcInPow2 < 0)
	{
		t1 = -1;
		t2 = -1;
		return;
	}

	double hc = sqrt(hcInPow2);
	if (!inner){
		t1 = ca - hc;
		t2 = ca + hc;
	}
	else
	{
		t1 = ca + hc;
		t2 = -1;
	}
}

std::shared_ptr<std::vector<LineSegment>> Lens::getTransformFigure(Matrix<double> rotateMatrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}
	//Matrix<double> matrix = MatrixFactory::getMoveMatrix(center.getX(), center.getY(), center.getZ());
	Matrix<double> matrix = rotateMatrix;

	transformLens = std::make_shared<std::vector<LineSegment>>();

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

		transformLens->push_back(LineSegment(transformP1, transformP2));
	}

	return transformLens;
}

std::shared_ptr<std::vector<LineSegment>> Lens::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformLens == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformLens;
}

const OpticProperties& Lens::getOpticProperties()
{
	return *advancedOpticProperties;
}

QRgb Lens::getColor()
{
	return advancedOpticProperties->getDiffuse().getRGB();
}

bool Lens::isTrasparency()
{
	return true;
}

bool Lens::isLightSource()
{
	return false;
}

void Lens::calculateLineSegment()
{
	PointVector3D center1 = sphere1.getCenter();
	PointVector3D center2 = sphere2.getCenter();
	double radius1 = sphere1.getRadius();
	double radius2 = sphere2.getRadius();
	double len = (center2 - center1).getLen();

	double x = (radius1*radius1 - radius2*radius2 + len*len) / (2*len);

	std::vector<PointVector3D> coordinates2Ds;

	double startAngle = acos(x / radius1);
	double stopAngle = -startAngle;

	double step = (startAngle - stopAngle) / M;

	for (double angle = startAngle; angle >= stopAngle; angle -= step)
	{
		coordinates2Ds.push_back(PointVector3D(radius1 * cos(angle) - x, radius1 * sin(angle), 0, 1));
	}

	startAngle = acos((len - x) / radius2);
	stopAngle = -startAngle;
	step = (startAngle - stopAngle) / M;

	for (double angle = startAngle; angle >= stopAngle; angle -= step)
	{
		coordinates2Ds.push_back(PointVector3D(-(radius2 * cos(angle) - (len - x)), -radius2 * sin(angle), 0, 1));
	}
	
	std::vector<LineSegment> firstSegments;
	//просчитываем для сегмента в плоскости OXY
	for (int i = 0; i < coordinates2Ds.size() - 1; i++)
	{
		firstSegments.push_back(LineSegment(coordinates2Ds.at(i), coordinates2Ds.at(i + 1)));
	}
	firstSegments.push_back(LineSegment(coordinates2Ds.at(coordinates2Ds.size() - 1), coordinates2Ds.at(0)));
	
	//описываем окружность расчитанным ранее сегментом в плоскости OXY
	double deltaPH = M_PI * 2 / N;
	std::vector<std::vector<LineSegment>> tmpSegments;
	for (double ph = 0; ph <=2 * M_PI; ph += deltaPH){

		std::vector<LineSegment> coordinates;
		Matrix<double> transformnMatrix = createTransformationMatrix(ph);

		for (LineSegment segment : firstSegments)
		{
			PointVector3D p1 = segment.getStart();
			PointVector3D p2 = segment.getFinish();
			coordinates.push_back(LineSegment(transformnMatrix * PointVector3D(p1.getX(), p1.getY(), p1.getY(), 1),
				transformnMatrix * PointVector3D(p2.getX(), p2.getY(), p2.getY(), 1)));
		}

		tmpSegments.push_back(coordinates);
	}

	PointVector3D OS = (center2 - center1).getNormolizeVector();
	PointVector3D A = center1 + (x * OS);

	double alpha = acos(OS | PointVector3D(1, 0, 0));
	double betta = acos(OS | PointVector3D(0, 1, 0));
	double gamma = acos(OS | PointVector3D(0, 0, 1));

	Matrix<double> matrix = MatrixFactory::getEMatrix();
	matrix = MatrixFactory::getXAxisRotation(alpha) * matrix;
	matrix = MatrixFactory::getYAxisRotation(betta) * matrix;
	matrix = MatrixFactory::getZAxisRotation(gamma) * matrix;

	matrix = MatrixFactory::getMoveMatrix(A.getX(), A.getY(), A.getZ()) * matrix;

	//соединяем поперечными линиями
	for (int i = 0; i < tmpSegments.size() - 1; i++){
		for (int j = 0; j < tmpSegments.at(i).size() - 1; j++){
			lineSegments.push_back(LineSegment(matrix * tmpSegments.at(i).at(j).getStart(), matrix * tmpSegments.at(i + 1).at(j).getStart()));
		}
	}
	
	for (std::vector<LineSegment> xx : tmpSegments)
	{
		for (LineSegment y : xx)
		{
			lineSegments.push_back(LineSegment(matrix * y.getStart(), matrix * y.getFinish()));
		}
	}



	lineSegments.push_back(LineSegment(PointVector3D(0, 0, 0, 1), PointVector3D(10, 0, 0, 1)));
}

inline Matrix<double> Lens::createTransformationMatrix(double ph) const
{
	Matrix<double> matrix(4, 4, 0);
	matrix[0][0] = 1;
	matrix[1][1] = cos(ph);
	matrix[2][2] = sin(ph);
	matrix[3][3] = 1;

	return matrix;
}