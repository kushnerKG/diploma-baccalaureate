#include "Camera.h"
#include "MatrixFactory.h"

Camera::Camera():
		viewPoint(PointVector3D(0.0, 0.0, 0.0)), eyePoint(PointVector3D(-20.0, 0.0, 0.0)), upVector(PointVector3D(0.0, 0.0, 1.0)), 
				matrixOfCamera(MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector))
{
	
}


Camera::Camera(PointVector3D& viewPoint, PointVector3D& eyePoint, PointVector3D& upVector):
		viewPoint(viewPoint), eyePoint(eyePoint), upVector(upVector), 
				matrixOfCamera(MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector))
{
}

Camera::~Camera()
{
}

void Camera::zoom(double ratio)
{
	PointVector3D z = viewPoint - eyePoint;
	z.normalizePointVector();
	z *= ratio;
	eyePoint -= z;
	matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
}

void Camera::moveToRight()
{
	eyePoint[1] -= DELTA;
	viewPoint[1] -= DELTA;
	matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
}

void Camera::moveToLeft()
{
	eyePoint[1] += DELTA;
	viewPoint[1] += DELTA;
	matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
}

void Camera::moveToUp()
{
	eyePoint[2] += DELTA;
	viewPoint[2] += DELTA;
	matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
}

void Camera::moveToDown()
{
	eyePoint[2] -= DELTA;
	viewPoint[2] -= DELTA;
	matrixOfCamera = MatrixFactory::getMatrixOfCamera(eyePoint, viewPoint, upVector);
}

const Matrix<double>& Camera::getMatrixOfCamera() const
{
	return matrixOfCamera;
}

const PointVector3D& Camera::getViewPoint() const
{
	return viewPoint;
}

const PointVector3D& Camera::getEyePoint() const
{
	return eyePoint;
}

const PointVector3D& Camera::getUpVector() const
{
	return upVector;
}

void Camera::setViewPoint(const PointVector3D& viewPoint)
{
	this->viewPoint = viewPoint;
}

void Camera::setEyePoint(const PointVector3D& eyePoint)
{
	this->eyePoint = eyePoint;
}

void Camera::setUpVector(const PointVector3D& upVector)
{
	this->upVector = upVector;
}
