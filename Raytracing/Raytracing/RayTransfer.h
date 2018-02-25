#pragma once
#include <vector>
#include "Ray.h"
#include "SceneFileInfo.h"
#include "NewLens.h"
#include "PolarizedRay.h"
#include "Quadrangle.h"

class TransferInfo;

class RayTransfer
{
public:
	RayTransfer(std::string path);
	~RayTransfer();

	void startTransfer();

	void transfer();

private:
	std::string path;// = "D:/учеба/диплом/programming/QT/Raytracing/Raytracing/Data/lense.transfer";
	std::vector<Ray> rays;

	//std::shared_ptr<NewLens> lens;
	
	void nodePassageForTransmitted(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
		const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const;

	void nodePassageForReflection(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
		const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const;


	void normalFallCase(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
		const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const;

	void noNormalFallCase(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
		const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const;

	Ray* createTransmittedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, double n1, double n2,
		bool isInner) const;
	Ray createReflectedRay(const PointVector3D& normal, const  PointVector3D& hit, const  Ray& lRay, bool isInner) const;
};

class TransferFileParser
{
public:
	TransferFileParser(std::string& path);

	std::unique_ptr<TransferInfo> parse() const;

private:
	std::string path;
	std::unique_ptr<NewLens> readLense(std::ifstream& inputFile) const;
	std::unique_ptr<Quadrangle> readQuadrangle(std::ifstream& inputFile) const;
	void readPolarizedRays(std::vector<PolarizedRay>& rays, std::ifstream& inputFile) const;
};

class TransferInfo
{
public:
	TransferInfo(std::string pathToOutputFile, std::vector<PolarizedRay>&& polarizedRays, std::unique_ptr<NewLens> newLens, 
		std::unique_ptr<Quadrangle> quadranngle)
		: pathToOputputFile(pathToOutputFile), rays(polarizedRays),
			lens(std::move(newLens)), quadranngle(std::move(quadranngle))
	{
	}

	TransferInfo(const TransferInfo& other) = delete;
	TransferInfo(TransferInfo&& other) = delete;
	TransferInfo& operator=(const TransferInfo& other) = delete;
	TransferInfo& operator=(TransferInfo&& other) = delete;

	std::vector<PolarizedRay>& getRays();
	NewLens* getLens() const;
	Quadrangle* getQuadranngle() const;
	std::string& getPath();

private:
	std::string pathToOputputFile;
	std::vector<PolarizedRay> rays;
	std::unique_ptr<NewLens> lens = nullptr;
	std::unique_ptr<Quadrangle> quadranngle = nullptr;
};