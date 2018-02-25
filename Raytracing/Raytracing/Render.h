#pragma once
#include <memory>
#include <qimage.h>
#include "SceneFileInfo.h"
#include "RenderInfo.h"
#include "Camera.h"
#include "RgbIntensityMap.h"
#include "Point.h"
#include "RgbIntensity.h"
#include "Model.h"
#include "TaskQueue.h"
class Ray;

class Render
{
public:
	Render(std::shared_ptr<SceneFileInfo> sceneFileInfo, std::shared_ptr<RenderInfo> renderInfo, 
		const Camera& camera, Matrix<double> rotateMatrix, Model& model);
	~Render();

	void render(std::shared_ptr<QImage> image);

	void operator()(std::shared_ptr<QImage> image, Model& model, std::shared_ptr<TaskQueue> taskQueue, std::string pathToSave = "");

private:

	std::shared_ptr<SceneFileInfo> sceneFileInfo;
	std::shared_ptr<RenderInfo> renderInfo;
	const Camera& camera;
	Model& model;

	PointVector3D right;
	PointVector3D start;
	PointVector3D upVector;

	PointVector3D viewPoint;
	PointVector3D eyePoint;
	PointVector3D upPoint;

	std::shared_ptr<IntensityMap> intensities = nullptr;

	Color* ambientColor;

//	double ambientRed;
//	double ambientGreen;
//	double ambientBlue;

	void normalRenderImage(std::shared_ptr<QImage> image) const;
	void roughtRenderImage(std::shared_ptr<QImage> image);//stub
	void fineRenderImage(std::shared_ptr<QImage> image) const;

	PointVector3D adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const;

	inline PointVector3D convert(Point point, int width, int height, float sw, float sh) const;

	Intensity& tracing(Ray ray, int depth, bool isFirstRay) const;

	inline bool checkLightSource(const LightSource& lightSource, const PointVector3D& hit, Transmission& transmission, Ray& ray) const;

	inline Ray createReflectedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, bool isInner) const;
	inline Ray* createTransmittedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, double n1, double n2, 
		bool isInner) const;

	double calculateMaxIntensity() const;

};

