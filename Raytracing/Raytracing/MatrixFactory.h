#pragma once
#include "Matrix.h"

namespace MatrixFactory
{
	Matrix<double> getEMatrix();

	Matrix<double> getMoveMatrix(double deltaX, double deltaY, double deltaZ);

	Matrix<double> getXAxisRotation(double angle);
	Matrix<double> getYAxisRotation(double angle);
	Matrix<double> getZAxisRotation(double angle);

	Matrix<double> getMatrixOfCamera(PointVector3D& pCam, PointVector3D& pView, PointVector3D& vUp);
	Matrix<double> getPSPMatrix(double zn);
	Matrix<double> getInvertRotateMatrix(Matrix<double>& matrix);
}