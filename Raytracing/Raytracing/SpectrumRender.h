#pragma once
#include <memory>
#include <qimage.h>
#include "SceneFileInfo.h"
#include "RenderInfo.h"
#include "Camera.h"
#include "Point.h"
#include "SpectrumIntensityMap.h"
#include "Model.h"
#include "TaskQueue.h"
class Ray;

class SpectrumRender
{
public:
	SpectrumRender(std::shared_ptr<SceneFileInfo> sceneFileInfo, std::shared_ptr<RenderInfo> renderInfo, 
		const Camera& camera, Matrix<double> rotateMatrix, Model& model);
	~SpectrumRender();

	void render(std::shared_ptr<QImage> image);

	void operator()(std::shared_ptr<QImage> image, Model& model, std::shared_ptr<TaskQueue> taskQueue);


private:
	std::shared_ptr<SceneFileInfo> sceneFileInfo;
	std::shared_ptr<RenderInfo> renderInfo;
	const Camera& camera;
	Model& model;
	Color* ambientColor;

	PointVector3D right;
	PointVector3D start;
	PointVector3D upVector;

	PointVector3D viewPoint;
	PointVector3D eyePoint;
	PointVector3D upPoint;

	std::shared_ptr<SpectrumIntensityMap> intensities = nullptr;

	PointVector3D adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const;
	
	void roughtRenderImage(std::shared_ptr<QImage> image);//stub
	void normalRenderImage(std::shared_ptr<QImage> image) const;
	void fineRenderImage(std::shared_ptr<QImage> image) const;


	double tracing(Ray ray, int depth, int spectrumComponent, bool isFirstRay) const;
	PointVector3D convert(Point point, int width, int height, float sw, float sh) const;
	bool checkLightSource(const LightSource& lightSource, const PointVector3D& hit, double& transmission, int spectrumComponent, Ray& r) const;

	Ray createReflectedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, bool isInner) const;
	Ray* createTransmittedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, double n1, double n2,
		bool isInner) const;

	double calculateMaxIntensity() const;
};