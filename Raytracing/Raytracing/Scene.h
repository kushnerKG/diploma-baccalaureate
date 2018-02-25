#pragma once
#include <vector>
#include "Shape.h"
#include <qimage.h>
#include <memory>
#include "SceneFileInfo.h"
#include "RenderInfo.h"
#include "Point.h"
#include "Matrix.h"
#include "Camera.h"
class Scene
{
public:
	Scene();
	~Scene();

	void drawScene(std::shared_ptr<QImage> image);
	
	void setSceneFileInfo(std::shared_ptr<SceneFileInfo> sceneFileInfo);
	void setRenderFileInfo(std::shared_ptr<RenderInfo> renderInfo);
	Point fromCoordinatesToPixel(PointVector3D coord, int width, int height) const;
	void renderScene(std::shared_ptr<QImage> renderImage);
	void rotateScene(double angleX, double angleY);
	void drawCoordinatesSystem();

	Matrix<double> getRotateMatrix() const;
	const Camera& getCamera() const;
private:
	
	bool wasRotate = false;
	bool isShowCoordinatesSystem = true;

	std::vector<Shape*> shapes;
	std::shared_ptr<SceneFileInfo> sceneFileInfo;
	std::shared_ptr<RenderInfo> renderInfo;
	
	Camera camera = Camera();
	Matrix<double> rotateMatrix;

	Matrix<double> createTransformMatrix() const;
};

