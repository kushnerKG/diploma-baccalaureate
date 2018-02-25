#pragma once
#include "PointVector3D.h"
#include "Matrix.h"
#include "MatrixFactory.h"
class Camera
{

public:

	Camera();
	Camera(PointVector3D& viewPoint, PointVector3D& eyePoint, PointVector3D& upVector);
	~Camera();

	void zoom(double ratio);
	
	void moveToRight();
	void moveToLeft();
	void moveToUp();
	void moveToDown();

	const Matrix<double>& getMatrixOfCamera() const;

	const PointVector3D& getViewPoint() const;
	const PointVector3D& getEyePoint() const;
	const PointVector3D& getUpVector() const;

	void setViewPoint(const PointVector3D& viewPoint);
	void setEyePoint(const PointVector3D& eyePoint);
	void setUpVector(const PointVector3D& upVector);

private:
	PointVector3D viewPoint = PointVector3D(0.0, 0.0, 0.0);
	PointVector3D eyePoint = PointVector3D(-20.0, 0.0, 0.0);
	PointVector3D upVector = PointVector3D(0.0, 0.0, 1.0);

	Matrix<double> matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
public:
	const static int DELTA = 2;
};