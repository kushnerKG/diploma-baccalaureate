#include "Scene.h"
#include <QPainter>
#include <QDebug>
#include "Matrix.h"
#include "MatrixFactory.h"

Scene::Scene() :rotateMatrix(Matrix<double>(4, 4, 0.0))
{
	rotateMatrix[0][0] = 1.0;
	rotateMatrix[1][1] = 1.0;
	rotateMatrix[2][2] = 1.0;
	rotateMatrix[3][3] = 1.0;
}


Scene::~Scene()
{
}

void Scene::drawScene(std::shared_ptr<QImage> image)
{
	int width = image.get()->width();
	int height = image.get()->height();

	int color = qRgb(0, 0, 0);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image->setPixel(x, y, color);
		}
	}

	std::vector<Shape*> shapes = sceneFileInfo->getShapes();

	QPainter painter;
	painter.begin(image.get());
	painter.setRenderHint(QPainter::Antialiasing, true);

	Matrix<double> transformMatrix = createTransformMatrix();

	for (Shape* shape : shapes)
	{
		std::shared_ptr<std::vector<LineSegment>> transfromedFigure;
		if (wasRotate)
		{
			transfromedFigure = shape->getTransformFigure(transformMatrix);
		}
		else
		{
			transfromedFigure = shape->getLastRotatesFigure(transformMatrix);
		}

		QRgb gridColor = shape->getColor();
		painter.setPen(QPen(gridColor));

		for (LineSegment line : *transfromedFigure)
		{
			Point p1 = fromCoordinatesToPixel(line.getStart(), image->width(), image->height());
			Point p2 = fromCoordinatesToPixel(line.getFinish(), image->width(), image->height());

			painter.drawLine(QLine(p1.getX(), p1.getY(), p2.getX(), p2.getY()));
		}
	}

	if (isShowCoordinatesSystem)
	{
		painter.setPen(QPen(qRgb(255, 0, 0)));
		Point p1 = fromCoordinatesToPixel(transformMatrix * PointVector3D(0, 0, 0), image->width(), image->height());
		Point p2 = fromCoordinatesToPixel(transformMatrix * PointVector3D(20, 0, 0), image->width(), image->height());
		painter.drawLine(QLine(p1.getX(), p1.getY(), p2.getX(), p2.getY()));

		painter.setPen(QPen(qRgb(0, 255, 0)));
		p1 = fromCoordinatesToPixel(transformMatrix * PointVector3D(0, 0, 0), image->width(), image->height());
		p2 = fromCoordinatesToPixel(transformMatrix * PointVector3D(0, 20, 0), image->width(), image->height());
		painter.drawLine(QLine(p1.getX(), p1.getY(), p2.getX(), p2.getY()));

		painter.setPen(QPen(qRgb(0, 0, 255)));
		p1 = fromCoordinatesToPixel(transformMatrix * PointVector3D(0, 0, 0), image->width(), image->height());
		p2 = fromCoordinatesToPixel(transformMatrix * PointVector3D(0, 0, 20), image->width(), image->height());
		painter.drawLine(QLine(p1.getX(), p1.getY(), p2.getX(), p2.getY()));
	}

	painter.end();

	wasRotate = false;
}

void Scene::setSceneFileInfo(std::shared_ptr<SceneFileInfo> sceneFileInfo)
{
	rotateMatrix = Matrix<double>(4, 4, 0);
	rotateMatrix[0][0] = 1.0;
	rotateMatrix[1][1] = 1.0;
	rotateMatrix[2][2] = 1.0;
	rotateMatrix[3][3] = 1.0;
	this->sceneFileInfo = sceneFileInfo;
}

void Scene::setRenderFileInfo(std::shared_ptr<RenderInfo> renderInfo)
{
	this->renderInfo = renderInfo;
	PointVector3D view = renderInfo->getViewPoint();
	PointVector3D eye = renderInfo->getEyePoint();
	PointVector3D up = renderInfo->getUpVector();
	camera = Camera(view, eye, up);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rotateMatrix[i][j] = 0;
		}
	}
	rotateMatrix[0][0] = 1.0;
	rotateMatrix[1][1] = 1.0;
	rotateMatrix[2][2] = 1.0;
	rotateMatrix[3][3] = 1.0;
}

Point Scene::fromCoordinatesToPixel(PointVector3D coord, int width, int height) const
{
	double x = coord.getX(), y = coord.getY();

	double deltaW = width / (renderInfo->getSw());
	double deltaH = height / (renderInfo->getSh());

	int pX = static_cast<int>(width / 2 + x * deltaW);
	int pY = static_cast<int>(height / 2 - deltaH * y);

	return Point(pX, pY);
}

void Scene::renderScene(std::shared_ptr<QImage> renderImage)
{

}

void Scene::rotateScene(double angleX, double angleY)
{
	wasRotate = true;
	Matrix<double> XRotationMatrix = MatrixFactory::getXAxisRotation(-angleX);
	Matrix<double> YRotationMatrix = MatrixFactory::getYAxisRotation(-angleY);

	rotateMatrix = XRotationMatrix * rotateMatrix;
	rotateMatrix = YRotationMatrix * rotateMatrix;
}

void Scene::drawCoordinatesSystem()
{
	isShowCoordinatesSystem = !isShowCoordinatesSystem;
}

Matrix<double> Scene::getRotateMatrix() const
{
	return rotateMatrix;
}

const Camera& Scene::getCamera() const
{
	return camera;
}

Matrix<double> Scene::createTransformMatrix() const
{
	const PointVector3D& viewPoint = camera.getViewPoint();
	Matrix<double> matrix = rotateMatrix * MatrixFactory::getMoveMatrix(-viewPoint.getX(), -viewPoint.getY(), -viewPoint.getZ());
	matrix = MatrixFactory::getMoveMatrix(viewPoint.getX(), viewPoint.getY(), viewPoint.getZ()) * matrix;
	matrix = camera.getMatrixOfCamera() * matrix;
	Matrix<double> proj = MatrixFactory::getPSPMatrix(renderInfo->getZn());

	return  proj *matrix;
}
