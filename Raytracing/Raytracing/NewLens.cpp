#define _USE_MATH_DEFINES
#include <math.h>
#include "NewLens.h"
#include "MatrixFactory.h"
#include <QDebug.h>
#include "AdvancedOpticProperties.h"


NewLens::NewLens(double leftRadius, double rightRadius, double lensRadius, double cylinderHeight, PointVector3D directionStart, 
	PointVector3D directionFinish, PointVector3D lensCenter, std::shared_ptr<OpticProperties> opticProperties, std::shared_ptr<OpticProperties> cylinderAdvancedOpticProperties) :
									leftSurfaceRadius(leftRadius), rightSurfaceRadius(rightRadius), lensRadius(lensRadius), cylinderHeight(cylinderHeight),
									directionStart(directionStart), directionFinish(directionFinish), lensCenter(lensCenter),
									advancedOpticProperties(opticProperties), cylinderAdvancedOpticProperties(cylinderAdvancedOpticProperties),
									fromWorldToModelMatrix(Matrix<double>(4, 4, 0)), fromModelToWorldMatrix(Matrix<double>(4, 4, 0))
{
	buildChangeCoordinateSystemMatrices();
	calculateSurface();
	PointVector3D OS = directionFinish - directionStart;
	if (leftSurfaceRadius > 0)
	{
		leftCuttingPlane = new Plane(OS, cylinderStart);
	} else
	{
		PointVector3D reverseOs = OS * -1;
		leftCuttingPlane = new Plane(reverseOs, cylinderStart);
	}

	if (rightSurfaceRadius > 0)
	{
		rightCuttingPlane = new Plane(OS, cylinderFinish);
	} else
	{
		PointVector3D reverseOs = OS * -1;
		rightCuttingPlane = new Plane(reverseOs, cylinderFinish);
	}
	
	if (leftRadius != 0.0 && rightRadius != 0.0)
	{
		TYPE = BICONVEX;
		return;
	}
	if (leftRadius == 0.0)
	{
		TYPE = LEFT_PLANOCONVEX;
		return;
	}
	if (rightRadius == 0.0)
	{
		TYPE = RIGHT_PLANOCONVEX;
		return;
	}
}


NewLens::~NewLens()
{
	if (leftSphere != nullptr)
	{
		delete leftSphere;
	}
	if (rightSphere != nullptr)
	{
		delete rightSphere;
	}
	if (cylinder != nullptr)
	{
		delete cylinder;
	}
}

Shape* NewLens::clone()
{
	return new NewLens(leftSurfaceRadius, rightSurfaceRadius, lensRadius, cylinderHeight, directionStart, directionFinish, lensCenter, advancedOpticProperties, cylinderAdvancedOpticProperties);
}

std::unique_ptr<IntersectionInfo> NewLens::checkRay(Ray& ray)
{
	
	switch (TYPE)
	{
	case BICONVEX:
		return checkBiconvexRay(ray);
	case LEFT_PLANOCONVEX:
		return checkLeftPlanoconvexRay(ray);
	default: 
		return nullptr;
	}
}

std::unique_ptr<IntersectionInfo> NewLens::checkBiconvexRay(Ray& ray) const
{
	Index distance[3];
	distance[0].id = DEFAULT;
	distance[1].id = DEFAULT;
	distance[2].id = DEFAULT;
	int count = 0;
	std::unique_ptr<IntersectionInfo> rightIntersection = rightSphere->checkRay(ray);
	if (rightIntersection != nullptr)
	{
		PointVector3D intrPoint = rightIntersection->getIntersectionPoints();
		if (rightCuttingPlane->distanceFrom(intrPoint) > 0)
		{
			distance[count].t = rightIntersection->getLen();
			distance[count].id = RIGHT_SPHERE;
			count++;
		}
	}
	
	std::unique_ptr<IntersectionInfo> leftIntersection = leftSphere->checkRay(ray);
	if (leftIntersection != nullptr)
	{
		PointVector3D intrPoint = leftIntersection->getIntersectionPoints();
		if (leftCuttingPlane->distanceFrom(intrPoint) < 0)
		{
			distance[count].t = leftIntersection->getLen();
			distance[count].id = LEFT_SPHERE;
			count++;
		}
	}

	std::unique_ptr<IntersectionInfo> cylinderIntersection = cylinder->checkRay(ray);

	if (cylinderIntersection != nullptr)
	{
		distance[count].t = cylinderIntersection->getLen();
		distance[count].id = CYLINDER;
		count++;
	}

	sortIndexeds(distance, count);
	AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
	AdvancedOpticProperties* cylinderOP = nullptr;
	switch (distance[0].id)
	{
	case LEFT_SPHERE:
		cylinderOP = dynamic_cast<AdvancedOpticProperties*>(cylinderAdvancedOpticProperties.get());
		leftIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(opticProperties->getTransmitted()) :
			(cylinderIntersection == nullptr ? (std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted())) :
			(std::make_shared<Transmission>(opticProperties->getTransmitted(), cylinderOP->getTransmitted()))));
		return leftIntersection;
	case RIGHT_SPHERE:
		cylinderOP = dynamic_cast<AdvancedOpticProperties*>(cylinderAdvancedOpticProperties.get());
		rightIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(opticProperties->getTransmitted()) :
			(cylinderIntersection == nullptr ? (std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted())) :
			(std::make_shared<Transmission>(opticProperties->getTransmitted(), cylinderOP->getTransmitted()))));
		return rightIntersection;
	case CYLINDER:
		cylinderIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(opticProperties->getTransmitted()) :
			std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted()));
		return cylinderIntersection;
	default:
		break;
	}

	return nullptr;
}

std::unique_ptr<IntersectionInfo> NewLens::checkLeftPlanoconvexRay(Ray& ray) const
{
	Index distance[3];
	distance[0].id = DEFAULT;
	distance[1].id = DEFAULT;
	distance[2].id = DEFAULT;
	int count = 0;

	std::unique_ptr<IntersectionInfo> rightIntersection = rightSphere->checkRay(ray);
	if (rightIntersection != nullptr){
		PointVector3D intrPoint = rightIntersection->getIntersectionPoints();
		if (rightCuttingPlane->distanceFrom(intrPoint) > 0)
		{
			distance[count].t = rightIntersection->getLen();
			distance[count].id = RIGHT_SPHERE;
			count++;
		}
		else
		{
			rightIntersection == nullptr;
		}
	}

	std::unique_ptr<IntersectionInfo> leftIntersection = checkRayWithCircle(ray);
	if (leftIntersection != nullptr)
	{

		distance[count].t = leftIntersection->getLen();
		distance[count].id = LEFT_SPHERE;
		count++;
	}

	std::unique_ptr<IntersectionInfo> cylinderIntersection = cylinder->checkRay(ray);
	if (cylinderIntersection != nullptr)
	{
		distance[count].t = cylinderIntersection->getLen();
		distance[count].id = CYLINDER;
		count++;
	}

	sortIndexeds(distance, count);
	AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
	AdvancedOpticProperties* cylinderOP = dynamic_cast<AdvancedOpticProperties*>(cylinderAdvancedOpticProperties.get());

	switch (distance[0].id)
	{
	case LEFT_SPHERE:
		leftIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(opticProperties->getTransmitted()) :
			(cylinderIntersection == nullptr ? (std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted())) :
			(std::make_shared<Transmission>(opticProperties->getTransmitted(), cylinderOP->getTransmitted()))));
		return leftIntersection;
	case RIGHT_SPHERE:
		rightIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(opticProperties->getTransmitted()) :
			(cylinderIntersection == nullptr ? (std::make_shared<Transmission>(opticProperties->getTransmitted(), opticProperties->getTransmitted())) :
			(std::make_shared<Transmission>(opticProperties->getTransmitted(), cylinderOP->getTransmitted()))));
		return rightIntersection;
	case CYLINDER:
		cylinderIntersection->setTransmission(count == 1 ? std::make_shared<Transmission>(cylinderOP->getTransmitted()) :
			std::make_shared<Transmission>(opticProperties->getTransmitted(), cylinderOP->getTransmitted()));
		cylinderIntersection->setIntersectionWithCylinder(true);
		return cylinderIntersection;
	default:
		break;
	}

	return nullptr;
}

PointVector3D NewLens::calculateCylinderNormal(Ray& ray, PointVector3D& worldIntersection, bool isInner) const
{
	PointVector3D modelIntersect = fromWorldToModelMatrix * worldIntersection;

	PointVector3D modelNormal = PointVector3D(modelIntersect.getX(), modelIntersect.getY(), 0, 1).getNormolizeVector();
	modelNormal[3] = 0.0;
	PointVector3D worldNormal = fromModelToWorldMatrix * modelNormal;

	worldNormal = isInner ? worldNormal *= -1 : worldNormal;

	return worldNormal;
}

void NewLens::sortIndexeds(Index* indexes, int size)
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

void NewLens::checkSphere(Sphere& sphere, Ray& ray, double &t1, double &t2, Plane* plane, COMPARE_CUTTING_TYPE distanceType) const
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
		t1 = -1.0;
		t2 = -1.0;
		return;
	}

	double ocLen = OC.getLen();
	double hcInPow2 = radius*radius - ocLen*ocLen + ca * ca;

	if (hcInPow2 < 0)
	{
		t1 = -1.0;
		t2 = -1.0;
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
		t2 = -1.0;
	}
}

void NewLens::checkCylinder(Ray& ray, double& t1, double& t2) const
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
		if (abs(modelRay.getRayStart().getX()) < lensRadius && abs(modelRay.getRayStart().getY()) < lensRadius
			&& abs(modelRay.getRayStart().getX()) > lensRadius && abs(modelRay.getRayStart().getY()) > lensRadius)
		{
			t1 = -1;
			t2 = -1;
			return;
		}
	}

	discriminant = (2 * x * l + 2 * y * m) * (2 * x * l + 2 * y * m) - 4 * (l * l + m * m) * (x * x - lensRadius * lensRadius + y * y);

	if (discriminant < 0)
	{
		t1 = -1;
		t2 = -1;
		return;
	}

	if (discriminant > 0)
	{
		t1 = (-(2 * x * l + 2 * y * m) - sqrt(discriminant)) / (2 * (l * l + m * m));
		t2 = (-(2 * x * l + 2 * y * m) + sqrt(discriminant)) / (2 * (l * l + m * m));
	}
}

std::unique_ptr<IntersectionInfo> NewLens::checkRayWithCircle(Ray& ray) const
{
	PointVector3D normal = leftCuttingPlane->getNormal();
//	if (ray.isInner())
//	{
//		normal *= -1;
//	}
	

	double vd = normal | ray.getRayDirection();
//	if (abs(vd) <= 0.000000000001)
//	{
//		return nullptr;
//	}

	double v0 = -(normal | ray.getRayStart()) - leftCuttingPlane->getD();
	double t = v0 / vd;
	if (t < 0)
	{
		return nullptr;
	}

	PointVector3D intersectionPoint = ray.getRayStart() + (t /*- INTERSECTION_DELTA*/) * ray.getRayDirection();

	double len = (intersectionPoint - cylinderStart).getLen();
	if (len > lensRadius)
	{
		return nullptr;
	}
	AdvancedOpticProperties* opticProperties = dynamic_cast<AdvancedOpticProperties*>(advancedOpticProperties.get());
	return std::make_unique<IntersectionInfo>(intersectionPoint, advancedOpticProperties, t,
	                                          (leftCuttingPlane->getNormal() | ray.getRayDirection()) < 0 ? leftCuttingPlane->getNormal() : leftCuttingPlane->getNormal() *= -1,
	                                          std::make_shared<Transmission>(opticProperties->getTransmitted()), true);
}

std::shared_ptr<std::vector<LineSegment>> NewLens::getTransformFigure(Matrix<double> rotateMatrix)
{
	if (lineSegments.size() == 0)
	{
		calculateLineSegment();
	}

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

std::shared_ptr<std::vector<LineSegment>> NewLens::getLastRotatesFigure(Matrix<double> rotateMatrix)
{
	if (transformLens == nullptr)
	{
		return getTransformFigure(rotateMatrix);
	}

	return transformLens;
}

void NewLens::calculateLineSegment()
{
	//left surface
	if (leftSurfaceRadius != 0.0)
	{
		double xLen = sqrt(leftSurfaceRadius * leftSurfaceRadius - lensRadius * lensRadius);
		if (leftSurfaceRadius > 0)
		{	
			double startAngle = acos(-xLen / leftSurfaceRadius);
			double stopAngle = acos((xLen / leftSurfaceRadius)) + M_PI;
			createLineSegmentSurface(xLen - 0.5 * cylinderHeight, leftSurfaceRadius, startAngle, stopAngle);
		} else
		{
			double stopAngle = acos(-1 * xLen / leftSurfaceRadius);
			double startAngle = -stopAngle;
			createLineSegmentSurface(-0.5 * cylinderHeight - xLen, -1 * leftSurfaceRadius, startAngle, stopAngle);
		}
	}
	else
	{
		createCircleLineSegment(-0.5 * cylinderHeight);
	}

	//right surface
	if (rightSurfaceRadius != 0.0)
	{
		double xLen = sqrt(rightSurfaceRadius * rightSurfaceRadius - lensRadius * lensRadius);
		if (rightSurfaceRadius > 0)
		{
			double stopAngle = acos(xLen / rightSurfaceRadius);
			double startAngle = -stopAngle;
			createLineSegmentSurface(0.5 * cylinderHeight - xLen, rightSurfaceRadius, startAngle, stopAngle);
		} else
		{
			double startAngle = acos(xLen / rightSurfaceRadius);
			double stopAngle = acos((-xLen / rightSurfaceRadius)) + M_PI;
			createLineSegmentSurface(xLen + 0.5 * cylinderHeight, -1 * rightSurfaceRadius, startAngle, stopAngle);
		}
	}
	else
	{
		createCircleLineSegment(0.5 * cylinderHeight);
	}

	//cylinder
	if (cylinderHeight != 0.0)
	{
		createCylinderLineSegment(PointVector3D(-0.5 * cylinderHeight, 0, 0), PointVector3D(0.5 * cylinderHeight, 0, 0));
	}
}

void NewLens::createCircleLineSegment(double deltaX)
{	
	double step = lensRadius / M;

	double angleDelta = M_PI * 2 / N;

	std::vector<LineSegment>* modelLines = new std::vector<LineSegment>();

	for (double radius = lensRadius; radius >=0; radius -= step)
	{
		for (double angle = 0; angle < 2 * M_PI; angle += angleDelta)
		{
			modelLines->push_back(LineSegment(PointVector3D(deltaX, radius * cos(angle), radius * sin(angle), 1),
				PointVector3D(deltaX, radius * cos(angle + angleDelta), radius * sin(angle + angleDelta), 1)));
		}
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			modelLines->push_back(LineSegment(modelLines->at(i * N + j).getStart(), modelLines->at((i + 1) * N + j).getStart()));
		}
	}

	for (LineSegment ls : *modelLines)
	{
		lineSegments.push_back(LineSegment(fromModelToWorldMatrix * ls.getStart(), fromModelToWorldMatrix * ls.getFinish()));
	}

	delete modelLines;
}

void NewLens::createCylinderLineSegment(PointVector3D&& modelCylinderStart, PointVector3D&& modelCylinderFinish)
{
	double step = M_PI * 2 / N;
	std::vector<PointVector3D>* coordinates2Ds = new std::vector<PointVector3D>();
	std::vector<LineSegment>* line2Ds = new std::vector<LineSegment>();

	for (double angle = 0; angle <= 2 * M_PI; angle += step)
	{
		coordinates2Ds->push_back(PointVector3D(modelCylinderStart.getX(), lensRadius * cos(angle), lensRadius * sin(angle), 1));
	}

	double deltaH = (modelCylinderFinish.getX() - modelCylinderStart.getX()) / M;
	for (double h = modelCylinderStart.getX(); h <= modelCylinderFinish.getX(); h += deltaH)
	{
		for (int i = 0; i < coordinates2Ds->size() - 1; i++)
		{
			line2Ds->push_back(LineSegment(PointVector3D(h,coordinates2Ds->at(i).getY(), coordinates2Ds->at(i).getZ()),
				PointVector3D(h, coordinates2Ds->at(i + 1).getY(), coordinates2Ds->at(i + 1).getZ())));
		}
		line2Ds->push_back(LineSegment(PointVector3D(h, coordinates2Ds->at(coordinates2Ds->size() - 1).getY(), coordinates2Ds->at(coordinates2Ds->size() - 1).getZ()),
			PointVector3D(h, coordinates2Ds->at(0).getY(), coordinates2Ds->at(0).getZ())));
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			line2Ds->push_back(LineSegment(line2Ds->at(i * N + j).getStart(), line2Ds->at((i + 1) * N + j).getStart()));
		}
	}

	for (LineSegment ls : *line2Ds)
	{
		lineSegments.push_back(LineSegment(fromModelToWorldMatrix * ls.getStart(), fromModelToWorldMatrix * ls.getFinish()));
	}

	delete line2Ds;
	delete coordinates2Ds;
}

void NewLens::createLineSegmentSurface(double deltaX, double radius, double startAngle, double stopAngle)
{
	double step = (stopAngle - startAngle) / M;

	std::vector<PointVector3D> surfacecoordinates2Ds;
	for (double ph = startAngle; ph < stopAngle; ph += step)
	{
		surfacecoordinates2Ds.push_back(PointVector3D(radius * cos(ph) + deltaX, radius * sin(ph), 0, 1));
	}
	surfacecoordinates2Ds.push_back(PointVector3D(radius * cos(stopAngle) + deltaX, radius * sin(stopAngle), 0, 1));

	std::vector<LineSegment> firstSegments;
	for (int i = 0; i < surfacecoordinates2Ds.size() - 1; i++)
	{
		firstSegments.push_back(LineSegment(surfacecoordinates2Ds.at(i), surfacecoordinates2Ds.at(i + 1)));
	}
	std::vector<LineSegment>* localLineSegment = new std::vector<LineSegment>();
	double deltaPH = M_PI * 2 / N;
	for (double ph = 0; ph <= 2 * M_PI; ph += deltaPH)
	{
		Matrix<double> transformnMatrix = createTransformationMatrix(ph);

		for (LineSegment segment : firstSegments)
		{
			PointVector3D p1 = segment.getStart();
			PointVector3D p2 = segment.getFinish();
			localLineSegment->push_back(LineSegment(transformnMatrix * PointVector3D(p1.getX(), p1.getY(), p1.getY(), 1),
				transformnMatrix * PointVector3D(p2.getX(), p2.getY(), p2.getY(), 1)));
		}
	}

	for (int i = 0; i < N - 1; i++)//N = 30
	{
		for (int j = 0; j < M; j++)//M = 10
		{
			localLineSegment->push_back(LineSegment(localLineSegment->at(i * M + j).getStart(), localLineSegment->at((i + 1) * M + j).getStart()));
		}
		localLineSegment->push_back(LineSegment(localLineSegment->at(i * M + M - 1).getFinish(), localLineSegment->at((i + 1) * M + M - 1).getFinish()));
	}

	for (LineSegment lineSegment : *localLineSegment)
	{
		lineSegments.push_back(LineSegment(fromModelToWorldMatrix * lineSegment.getStart(), fromModelToWorldMatrix * lineSegment.getFinish()));
	}

	delete localLineSegment;
}

const OpticProperties& NewLens::getOpticProperties()
{
	return *advancedOpticProperties;
}

QRgb NewLens::getColor()
{
	return qRgb(0, 255, 0);
}

bool NewLens::isTrasparency()
{
	return true;
}

bool NewLens::isLightSource()
{
	return false;
}

void NewLens::buildChangeCoordinateSystemMatrices()
{
	PointVector3D x = directionFinish - directionStart;
	x.normalizePointVector();
	PointVector3D yy = PointVector3D(0, 1, 0) == x ? PointVector3D(1, 1, 0) : PointVector3D(0, 1, 0);
	PointVector3D z = x * yy;
	PointVector3D y = z * x;
	z.normalizePointVector();
	y.normalizePointVector();

	Matrix<double> M1 = MatrixFactory::getMoveMatrix(-lensCenter.getX(), -lensCenter.getY(), -lensCenter.getZ());
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
	M1 = MatrixFactory::getMoveMatrix(lensCenter.getX(), lensCenter.getY(), lensCenter.getZ());
	fromModelToWorldMatrix = M1 * M3;
}

void NewLens::calculateSurface()
{
	double tmp = leftSurfaceRadius == 0 ? 0 : sqrt(leftSurfaceRadius * leftSurfaceRadius - lensRadius * lensRadius);
	modelLeftSurfaceStartPoint = PointVector3D(leftSurfaceRadius > 0 ? tmp - cylinderHeight * 0.5 : 
		-1 * (tmp + cylinderHeight * 0.5), 0, 0);
	leftSurfaceStartPoint = fromModelToWorldMatrix * modelLeftSurfaceStartPoint;

	tmp = rightSurfaceRadius == 0 ? 0 : sqrt(rightSurfaceRadius * rightSurfaceRadius - lensRadius * lensRadius);
	modelRightSurfaceStartPoint = PointVector3D(rightSurfaceRadius > 0 ? cylinderHeight * 0.5 - tmp : 
		cylinderHeight * 0.5 + tmp, 0, 0);
	rightSurfaceStartPoint = fromModelToWorldMatrix * modelRightSurfaceStartPoint;

	leftSphere = new Sphere(leftSurfaceStartPoint, leftSurfaceRadius > 0 ? 
		leftSurfaceRadius : -1 * leftSurfaceRadius, advancedOpticProperties, true);
	rightSphere = new Sphere(rightSurfaceStartPoint, rightSurfaceRadius > 0 ? 
		rightSurfaceRadius : -1 * rightSurfaceRadius, advancedOpticProperties, true);

	PointVector3D OS = directionFinish - directionStart;
	OS.normalizePointVector();
	cylinderStart = lensCenter - 0.5 * cylinderHeight * OS;
	cylinderFinish = cylinderStart + cylinderHeight * OS;
	cylinder = new Cylinder(cylinderStart, cylinderFinish, lensRadius, cylinderAdvancedOpticProperties, true);
}

Matrix<double> NewLens::createTransformationMatrix(double ph) const
{
	Matrix<double> matrix(4, 4, 0);
	matrix[0][0] = 1;
	matrix[1][1] = cos(ph);
	matrix[2][2] = sin(ph);
	matrix[3][3] = 1;

	return matrix;
}
