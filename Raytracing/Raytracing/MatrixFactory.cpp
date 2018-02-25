#include "MatrixFactory.h"
#include <cmath>

namespace MatrixFactory{

	Matrix<double> getEMatrix()
	{
		Matrix<double> matrix(4, 4, 0.0);
		matrix[0][0] = 1;
		matrix[1][1] = 1;
		matrix[2][2] = 1;
		matrix[3][3] = 1;

		return matrix;
	}

	Matrix<double> getMoveMatrix(double deltaX, double deltaY, double deltaZ)
	{
		Matrix<double> matrix(4, 4, 0.0);
		matrix[0][0] = 1;
		matrix[1][1] = 1;
		matrix[2][2] = 1;
		matrix[3][3] = 1;

		matrix[0][3] = deltaX;
		matrix[1][3] = deltaY;
		matrix[2][3] = deltaZ;

		return matrix;
	}

	Matrix<double> getXAxisRotation(double angle)
	{
		Matrix<double> matrix(4, 4, 0.0);

		matrix[0][0] = 1.0;
		matrix[1][1] = cos(angle);
		matrix[1][2] = -1.0 * sin(angle);
		matrix[2][1] = sin(angle);
		matrix[2][2] = cos(angle);
		matrix[3][3] = 1.0;

		return matrix;

	}

	Matrix<double> getYAxisRotation(double angle)
	{
		Matrix<double> matrix(4, 4, 0.0);

		matrix[0][0] = cos(angle);
		matrix[1][1] = 1.0;
		matrix[0][2] = sin(angle);
		matrix[2][0] = -1.0 * sin(angle);
		matrix[2][2] = cos(angle);
		matrix[3][3] = 1.0;

		return matrix;
	}
	
	Matrix<double> getZAxisRotation(double angle)
	{
		Matrix<double> matrix(4, 4, 0.0);

		matrix[0][0] = cos(angle);
		matrix[0][1] = -1 * sin(angle);
		matrix[1][0] = sin(angle);
		matrix[1][1] = cos(angle);
		matrix[2][2] = 1.0;
		matrix[3][3] = 1.0;

		return matrix;
	}

	Matrix<double> getMatrixOfCamera(PointVector3D& pCam, PointVector3D& pView, PointVector3D& vUp)
	{

		PointVector3D zVector(pCam[0] - pView[0], pCam[1] - pView[1], pCam[2] - pView[2]);
		zVector.normalizePointVector();

		PointVector3D xVector = vUp * zVector;
		xVector.normalizePointVector();

		PointVector3D yVector = zVector * xVector;

		Matrix<double> M1(4, 4, 0.0);
		Matrix<double> M2(4, 4, 0.0);
		for (int i = 0; i < 4; i++){
			M1[i][i] = 1.0;
		}

		M1[0][3] = -pCam[0];
		M1[1][3] = -pCam[1];
		M1[2][3] = -pCam[2];

		for (int i = 0; i < 3; i++){
			M2[0][i] = xVector[i];
			M2[1][i] = yVector[i];
			M2[2][i] = zVector[i];
		}
		M2[3][3] = 1.0;

		Matrix<double> tmp = M2 * M1;

		return  tmp;
	}

	Matrix<double> getPSPMatrix(double zn)
	{
		Matrix<double> matrix(4, 4, 0.0);
		for (int i = 0; i < 3; i++){
			matrix[i][i] = 1.0;
		}

		matrix[3][2] = -1.0 / zn;//так как направление взгляда против направления ост Z

		return matrix;
	}

	Matrix<double> getInvertRotateMatrix(Matrix<double>& matrix)
	{
		return matrix.transpose();
	}
}