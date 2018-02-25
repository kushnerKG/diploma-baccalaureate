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
#include "crt.h"
class Model;
struct Node;

class RenderWithFlatLightSource
{
public:
	RenderWithFlatLightSource(std::shared_ptr<SceneFileInfo> sceneFileInfo, std::shared_ptr<RenderInfo> renderInfo, 
		const Camera& camera, Matrix<double> rotateMatrix, Model& model);
	~RenderWithFlatLightSource();

	void render(std::shared_ptr<QImage> image, renderTypeWithFlatLight type);

	void operator()(std::shared_ptr<QImage> image, Model& model, std::shared_ptr<TaskQueue> taskQueue, renderTypeWithFlatLight type);

private:

	enum Reason{INTERSECTION_WITH_LIGHT_SOURCE, MISS_ALL, ACHIEVED_MAX_DEPTH, NOT_LEAF};

	struct Node //Звено дерева
	{
		Node *parent = nullptr;
		Node *left = nullptr, *right = nullptr; //Это указатели на новые звенья
		std::unique_ptr<IntersectionInfo> intersectionInfo;
		bool isLeft;
		Ray* ray = nullptr;
		double refractionIndexFrom = 1;
		double refractionIndexTo = 1;
		Reason reason = NOT_LEAF;
		int level;
	};

	struct IntensityWithElectroWave
	{
		PointVector3D electroWave;
		double intensity;
	};

	Model& model;
	std::shared_ptr<SceneFileInfo> sceneFileInfo;
	std::shared_ptr<RenderInfo> renderInfo;
	const Camera& camera;

	Color* ambientColor;

	PointVector3D right;
	PointVector3D start;
	PointVector3D upVector;

	PointVector3D viewPoint;
	PointVector3D eyePoint;
	PointVector3D upPoint;
	

	float ambientSpectrum[SPECTRUM_SIZE];

	std::shared_ptr<SpectrumIntensityMap> intensities = nullptr;

	PointVector3D adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const;

	void roughtRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type);//stub
	void normalRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type) const;
	void fineRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type) const;//stub
	double tracingWithPolarization(Ray& ray, int depth, int spectrumComponent, double& intensityWithoutReflections) const;

	double tracingWithPolarizationNewWay(Ray& ray, int depth, int spectrumComponent, 
		double& intensityWithoutReflections, double& oneReflections, double& twoReflections) const;
	void nodePassageForTransmittedNewWay(Node* node, IntensityWithElectroWave& intensityWithWave) const;
	void nodePassageForReflectionNewWay(Node* node, IntensityWithElectroWave& Ix) const;

	double tracingWithDispersion(Ray& ray, int depth, int spectrumComponent) const;
	double tracingWithDispersionAndSchlick(Ray& ray, int depth, int spectrumComponent) const;

	void nodePassageForTransmitted(Node* node, PointVector3D& Ix) const;
	void nodePassageForReflection(Node* node, PointVector3D& Ix) const;

	void deleteTree(Node* tree) const;

	void createTree(Node* node, int depth) const;

	Node* buildTree(Ray& ray, std::vector<Node*>& leaves, int depth, int spectrumComponent,
		double refractionIndex, int level) const;

	bool newBuildTree(Node* node, Ray& ray, std::vector<Node*>& leaves, int depth, int spectrumComponent,
		double refractionIndex, int level) const;

	PointVector3D convert(Point point, int width, int height, float sw, float sh) const;
	bool checkLightSource(const LightSource& lightSource, const PointVector3D& hit, double& transmission, int spectrumComponent) const;

	Ray createReflectedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, bool isInner) const;
	Ray* createTransmittedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, double n1, double n2,
		bool isInner) const;

	double calculateMaxIntensity() const;

	const double AMBIENT_RATIO = 0.4;
};


