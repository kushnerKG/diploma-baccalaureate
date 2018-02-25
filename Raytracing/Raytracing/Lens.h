#pragma once
#include "Shape.h"
#include "AdvancedOpticProperties.h"
#include "Sphere.h"
class Lens :
	public Shape
{
public:
	Lens(PointVector3D center1, double radius1, PointVector3D center2, double radius2, std::shared_ptr<OpticProperties> opticProperties);
	~Lens();

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

	struct Index
	{
		double t;
		int id;
	};

	const PointVector3D center1;
	const double radius1;
	const PointVector3D center2;
	const double radius2;

	const Sphere sphere1;
	const Sphere sphere2;

	std::shared_ptr<OpticProperties> advancedOpticProperties;
	
	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformLens = nullptr;

	inline Matrix<double> Lens::createTransformationMatrix(double ph) const;
	void calculateLineSegment();
	
	void checkSphere(const Sphere& sphere, Ray& ray, double& t1, double& t2) const;
	static void sortIndexeds(Index *indexes, int size);

	IntersectionInfo* checkOutsideRayFor4Intersects(Ray& ray, Index* indexs) const;
	IntersectionInfo* checkOutsideRayFor3Intersects(Ray& ray, Index* indexs) const;
	IntersectionInfo* checkInnerRayFor2Intersects(Ray& ray, Index* indexs) const;
};	