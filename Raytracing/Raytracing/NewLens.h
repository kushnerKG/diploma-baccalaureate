#pragma once
#include "Shape.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Plane.h"

class NewLens :
	public Shape
{
public:
	NewLens(double leftRadius, double rightRadius, double lensRadius, double cylinderHeight, PointVector3D directionStart, PointVector3D directionFinish,
		PointVector3D lensCenter, std::shared_ptr<OpticProperties> opticProperties, std::shared_ptr<OpticProperties> cylinderAdvancedOpticProperties);
	~NewLens();

	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	const OpticProperties& getOpticProperties() override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;

private:
	static const int M = 10;
	static const int N = 30;

	enum Surfaces {
		LEFT_SPHERE,
		RIGHT_SPHERE,
		CYLINDER,
		DEFAULT
	};

	enum LENS_TYPE {
		BICONVEX,//двоякувыпуклая
		LEFT_PLANOCONVEX,//плосковыпуклая(слева плоская)
		RIGHT_PLANOCONVEX//плосковыпуклая(справа плоская)
	};

	enum COMPARE_CUTTING_TYPE
	{
		LESS_ZERO,
		MORE_ZERO
	};

	struct Index
	{
		double t;
		Surfaces id;
	};

	LENS_TYPE TYPE = BICONVEX;

	const double leftSurfaceRadius;
	const double rightSurfaceRadius;
	const double lensRadius;
	const double cylinderHeight;
	const PointVector3D directionStart;
	const PointVector3D directionFinish;
	const PointVector3D lensCenter;
	const std::shared_ptr<OpticProperties> advancedOpticProperties;
	const std::shared_ptr<OpticProperties> cylinderAdvancedOpticProperties;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformLens = nullptr;

	Matrix<double> fromWorldToModelMatrix;
	Matrix<double> fromModelToWorldMatrix;

	PointVector3D leftSurfaceStartPoint = PointVector3D(0, 0, 0);
	PointVector3D rightSurfaceStartPoint = PointVector3D(0, 0, 0);
	PointVector3D modelLeftSurfaceStartPoint = PointVector3D(0, 0, 0);
	PointVector3D modelRightSurfaceStartPoint = PointVector3D(0, 0, 0);
	
	PointVector3D cylinderStart = PointVector3D(0, 0, 0);
	PointVector3D cylinderFinish = PointVector3D(0, 0, 0);

	Cylinder* cylinder = nullptr;
	Sphere* leftSphere = nullptr;
	Sphere* rightSphere = nullptr;
	
	Plane* leftCuttingPlane = nullptr;
	Plane* rightCuttingPlane = nullptr;

	void calculateLineSegment();
	void buildChangeCoordinateSystemMatrices();
	void calculateSurface();
	Matrix<double> createTransformationMatrix(double ph) const;
	void createLineSegmentSurface(double deltaX, double radius, double startAngle, double stopAngle);
	void createCylinderLineSegment(PointVector3D&& modelCylinderStart, PointVector3D&& modelCylinderFinish);
	void createCircleLineSegment(double deltaX);

	void checkSphere(Sphere& sphere, Ray& ray, double &t1, double &t2, Plane* plane, COMPARE_CUTTING_TYPE distanceType) const;
	void checkCylinder(Ray& ray, double& t1, double& t2) const;
	std::unique_ptr<IntersectionInfo> checkRayWithCircle(Ray& ray) const;

	static void sortIndexeds(Index *indexes, int size);

	PointVector3D calculateCylinderNormal(Ray& ray, PointVector3D& worldIntersection, bool isInner) const;

	std::unique_ptr<IntersectionInfo> checkBiconvexRay(Ray& ray) const;
	std::unique_ptr<IntersectionInfo> checkLeftPlanoconvexRay(Ray& ray) const;
};
