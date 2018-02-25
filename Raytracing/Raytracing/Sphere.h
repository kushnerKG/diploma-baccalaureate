#pragma once
#include "Shape.h"
#include "OpticProperties.h"
#include <ostream>
#include "AdvancedOpticProperties.h"

class Sphere : public Shape
{
public:
	Sphere(PointVector3D center, double radius, std::shared_ptr<OpticProperties> opticProperties, bool isTransparency);
	~Sphere();


	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;
	const OpticProperties& getOpticProperties() override;
	std::shared_ptr<std::vector<LineSegment>> getLastRotatesFigure(Matrix<double> rotateMatrix) override;
	std::shared_ptr<std::vector<LineSegment>> getTransformFigure(Matrix<double> rotateMatrix) override;
	QRgb getColor() override;
	bool isTrasparency() override;
	bool isLightSource() override;
	
	PointVector3D getCenter() const;
	double getRadius() const;
	friend std::ostream& operator<<(std::ostream& os, const Sphere& obj);

private:

	static const int M = 30;
	AdvancedOpticProperties* advancedOpticProperties = nullptr;
	const PointVector3D center;
	const double radius;
	std::shared_ptr<OpticProperties> opticProperties;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformSphere = nullptr;

	void calculateLineSegment();

	std::vector<std::vector<PointVector3D>*>* createXSections(std::vector<PointVector3D>* coord2D) const;
	std::vector<std::vector<PointVector3D>*>* createPHSections(std::vector<std::vector<PointVector3D>*>* xSections) const;

 	inline Matrix<double> createTransformationMatrix(double ph) const;

	const bool isTransparency;
};

