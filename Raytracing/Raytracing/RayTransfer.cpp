#include "RayTransfer.h"
#include "ParserUtils.h"
#include <fstream>
#include <memory>
#include "SpectrumColor.h"
#include "SceneFileReader.h"
#include "Quadrangle.h"
#include "PolarizedRay.h"
#include "QDebug"

#define _USE_MATH_DEFINES
#include <math.h>

//#define WITH_ABSORPTION

using namespace std;
using namespace RayTracingUtils;

RayTransfer::RayTransfer(std::string path) : path (path)
{
}

RayTransfer::~RayTransfer()
{
}

PointVector3D setPolarization(PointVector3D ray, PointVector3D polarizedVector)
{
	PointVector3D right = ray * polarizedVector;

	PointVector3D pol = right * ray;
	pol.normalizePointVector();

	return pol;
}


void initPrint(PolarizedRay& polarizedRay, std::ofstream& file, int& rayNumber)
{
	file << "RAY #" << rayNumber++ << "\n";
	file << "Init" << "\n";

	file << "start: " << polarizedRay.getNotPolarizedRay()->getRayStart().getX() << " " << polarizedRay.getNotPolarizedRay()->getRayStart().getY() << " " <<
		polarizedRay.getNotPolarizedRay()->getRayStart().getZ() << "\n";

	file << "direction: " << polarizedRay.getNotPolarizedRay()->getRayDirection().getX() << " " << polarizedRay.getNotPolarizedRay()->getRayDirection().getY()
		<< " " << polarizedRay.getNotPolarizedRay()->getRayDirection().getZ() << "\n";
	file << "polarization: " << polarizedRay.getPolarizedVector().getX() << " " << polarizedRay.getPolarizedVector().getY() << " " <<
		polarizedRay.getPolarizedVector().getZ() << " " << polarizedRay.getPolarizedVector().getLen() << "\n";

	file << "\n" << "Start transfer" << "\n";
}

void RayTransfer::startTransfer()
{
	std::ofstream file;

	TransferFileParser parser = TransferFileParser(path);
	std::unique_ptr<TransferInfo> transferInfo = parser.parse();

	file.open(transferInfo->getPath());

	std::vector<PolarizedRay>& rays = transferInfo->getRays();
	NewLens* lens = transferInfo->getLens();
	Quadrangle* quadrangle = transferInfo->getQuadranngle();
	//createPolarizedRay(rays);
	int check = false;
	int rayNumber = 1;
	for (PolarizedRay polarizedRay : rays){

		initPrint(polarizedRay, file, rayNumber);

		int i = 1;

		Ray ray = *polarizedRay.getNotPolarizedRay();
		unique_ptr<IntersectionInfo> hit = lens->checkRay(ray);

		while (hit != nullptr)
		{
			PointVector3D intersectionPoint = hit->getIntersectionPoints();

			file << "intersection # " << i++ << "\n";
			file << "intersection: " << intersectionPoint.getX() << " " << intersectionPoint.getY() << " " << intersectionPoint.getZ() << "\n";

			hit->getOpticProperties().get();
			AdvancedOpticProperties* advancedOpticProperties = static_cast<AdvancedOpticProperties*>(hit->getOpticProperties().get());

			double n1, n2;
			if (ray.isInner())
			{
				n1 = advancedOpticProperties->getN()[0];
				n2 = 1;
			}
			else
			{
				n1 = 1;
				n2 = advancedOpticProperties->getN()[0];
			}
			Ray* transmitted = createTransmittedRay(hit->getNormal(), hit->getIntersectionPoints(),
				ray, n1, n2, !ray.isInner());


			Ray reflected = createReflectedRay(hit->getNormal(), hit->getIntersectionPoints(), 
				Ray(hit->getIntersectionPoints(), ray.getRayStart(), false), ray.isInner());


			if (transmitted == nullptr)
			{
				check = true;
				file << "угол полного внутреннего отражения" << "\n";
				PointVector3D normal = hit->getNormal();
				file << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
				file << -1 * (ray.getRayDirection() | normal) << "\n\n";

				break;
			}

			PointVector3D polarizedVector = polarizedRay.getPolarizedVector();

			PolarizedRay tmp = polarizedRay;

			nodePassageForTransmitted(polarizedVector, polarizedRay.getIntensity(), ray.getRayDirection(), transmitted->getRayDirection(),
				hit->getNormal(), n1, n2, ray.isInner(), hit->getLen());
			polarizedRay.setPolarizedVector(polarizedVector);

			polarizedVector = tmp.getPolarizedVector();
			nodePassageForReflection(polarizedVector, tmp.getIntensity(), ray.getRayDirection(), transmitted->getRayDirection(),
				hit->getNormal(), n1, n2, ray.isInner(), hit->getLen());
			tmp.setPolarizedVector(polarizedVector);

			file << "direction: " << transmitted->getRayDirection().getX() << " " << transmitted->getRayDirection().getY() << " " <<
				transmitted->getRayDirection().getZ() << "\n";
			file << "polarization: " << polarizedRay.getPolarizedVector().getX() << " " << polarizedRay.getPolarizedVector().getY() << " " <<
				polarizedRay.getPolarizedVector().getZ() << " " << polarizedRay.getPolarizedVector().getLen() << "\n";
			file << "intensity: " << polarizedRay.getIntensity() << "\n" << "\n";

			hit = lens->checkRay(*transmitted);

			ray = Ray(*transmitted);

			polarizedRay.setRayDirection(ray.getRayDirection());

			delete transmitted;
		}

		hit = quadrangle->checkRay(ray);

		PointVector3D intersectionPoint = hit->getIntersectionPoints();
		if (!check){
			file << "intersection # " << i++ << " with plane" << "\n";
			file << "intersection: " << intersectionPoint.getX() << " " << intersectionPoint.getY() << " " << intersectionPoint.getZ() << "\n" << "\n" << "\n";
			check = false;
		}
	}
	file.close();
}

void createTransferRays(std::vector<Ray>& rays)
{
	
}

void RayTransfer::transfer()
{
	std::ofstream file;

	TransferFileParser parser = TransferFileParser(path);
	std::unique_ptr<TransferInfo> transferInfo = parser.parse();

	file.open(transferInfo->getPath());

	NewLens* lens = transferInfo->getLens();
	Ray testRay(PointVector3D(0, 0, 10), PointVector3D(0, 0, 0), false);
	unique_ptr<IntersectionInfo> hit = lens->checkRay(testRay);
	std::vector<Ray> rays;
	int raysCount = 200;
	double delta = 4.25 / raysCount;
	for (int i = 0; i < raysCount; i++)
	{
		rays.push_back(Ray(PointVector3D(0, 0, 12.5), PointVector3D(0 + i * delta, 0, 0), false));
	}

	for (int i = 0; i < raysCount; i++){
		Ray& currentRay = rays.at(i);
		hit = lens->checkRay(currentRay);
		AdvancedOpticProperties* advancedOpticProperties = static_cast<AdvancedOpticProperties*>(hit->getOpticProperties().get());

		Ray* transmitted = createTransmittedRay(hit->getNormal(), hit->getIntersectionPoints(),
			currentRay, 1, advancedOpticProperties->getN()[0], !currentRay.isInner());

		hit = lens->checkRay(*transmitted);
		Ray reflected = *transmitted;
		for (int j = 1; j < 9; j++)
		{
			reflected = createReflectedRay(hit->getNormal(), hit->getIntersectionPoints(), Ray(hit->getIntersectionPoints(),
				reflected.getRayStart(), false), reflected.isInner());

			hit = lens->checkRay(reflected);
			PointVector3D point = hit->getIntersectionPoints();
			if (hit->getIntersectionWithCylinder())
			{
				qDebug() << i << " " << j + 3 << " " << point[0] << " " << point[1] << " " << point[2];
				break;
			}
		}
		delete transmitted;
	}


	file.close();
}

void RayTransfer::nodePassageForTransmitted(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
	const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const
{
	if (-1 * (directionIncidentRay | normal) == 1)
	{
		normalFallCase(polarizedVector, intensity, directionIncidentRay, directionTransmittedRay, normal, refractionIndexFrom, refractionIndexTo, isInnerRay, len);
	}
	else
	{
		noNormalFallCase(polarizedVector, intensity, directionIncidentRay, directionTransmittedRay, normal, refractionIndexFrom, refractionIndexTo, isInnerRay, len);
	}	
}

void RayTransfer::nodePassageForReflection(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay, 
	const PointVector3D& reflection, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const
{
	PointVector3D incident = directionIncidentRay;
	PointVector3D intersectionNormal = normal;
	//PointVector3D reflection = -1.0 * node->parent->ray->getRayDirection();

	PointVector3D normalToPlaneOfIncidence = incident * intersectionNormal;
	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * incident;
	W.normalizePointVector();

	//остается одинаковыми при обратной трассировки, так как отраженный остается с то же стороны, для преломленного должно быть наоборот
	//double refractionIndexFrom = node->refractionIndexFrom;
	//double refractionIndexTo = node->refractionIndexTo;

	double alpha, betta;
	alpha = acos((-1 * incident) | intersectionNormal);
	
	double d = (refractionIndexFrom * sin(alpha)) / refractionIndexTo;
	if (abs(d) <= 1){
		betta = asin((refractionIndexFrom * sin(alpha)) / refractionIndexTo);
	}
	else
	{
		betta = M_PI / 2;
	}

	if (abs(alpha) < 0.00001 && abs(betta) < 0.00001)
	{
		double intensityRatio = pow((refractionIndexFrom - refractionIndexTo) / (refractionIndexFrom + refractionIndexTo), 2);
		double waveRatio = (refractionIndexFrom - refractionIndexTo) / (refractionIndexFrom + refractionIndexTo);
		intensity *= intensityRatio;
		polarizedVector *= waveRatio;
		return;
	}

	double cosBetweenElectroWaveAndFallPlane = polarizedVector.getNormolizeVector() | W;
	double cosBetweenElectroWaveAndNormalToFallPlane = polarizedVector.getNormolizeVector() | normalToPlaneOfIncidence;

	double pIncidentIntensity = cosBetweenElectroWaveAndFallPlane * cosBetweenElectroWaveAndFallPlane * intensity;
	double sIncidentIntensity = cosBetweenElectroWaveAndNormalToFallPlane * cosBetweenElectroWaveAndNormalToFallPlane * intensity;

	double pIntensityRatio = pow((tan(alpha - betta) / tan(alpha + betta)), 2);
	double sIntensityRatio = pow((sin(alpha - betta) / sin(alpha + betta)), 2);
	intensity = pIncidentIntensity * pIntensityRatio + sIncidentIntensity * sIntensityRatio;

	double pWaveRatio = -1 * tan(alpha - betta) / tan(alpha + betta);
	double sWaveRatio = -1 * sin(alpha - betta) / sin(alpha + betta);

	PointVector3D sTransmittedWave = cosBetweenElectroWaveAndNormalToFallPlane * sWaveRatio * normalToPlaneOfIncidence;
	PointVector3D pTransmittedWave = cosBetweenElectroWaveAndFallPlane * pWaveRatio * (reflection * normalToPlaneOfIncidence);

	polarizedVector = sTransmittedWave + pTransmittedWave;
	polarizedVector.normalizePointVector();
}

void RayTransfer::normalFallCase(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
	const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const
{
	double transmittedIntensityRatio = 4 * refractionIndexFrom * refractionIndexTo / (pow(refractionIndexFrom + refractionIndexTo, 2)); //нужно другой коэффициент
	double transmittedWaveRatio = 2 * refractionIndexFrom / (refractionIndexTo + refractionIndexFrom);

//	qDebug() << transmittedWaveRatio;
//	qDebug() << "-----------------------------";
	intensity *= transmittedIntensityRatio;
#ifdef WITH_ABSORPTION
	if (isInnerRay)
	{
		double r = pow(M_E, -1 * 0.8481 * len);
		intensity *= r;
		//		Ix.intensity *= r;
		//		Iy.intensity *= r;
	}
#endif
	polarizedVector *= transmittedWaveRatio;

}

void RayTransfer::noNormalFallCase(PointVector3D& polarizedVector, double& intensity, const PointVector3D& directionIncidentRay,
	const PointVector3D& directionTransmittedRay, const PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo, bool isInnerRay, double len) const
{
	PointVector3D normalToPlaneOfIncidence = directionIncidentRay * normal;
	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * directionIncidentRay;
	W.normalizePointVector();

	double cosBetweenElectroWaveAndFallPlane = polarizedVector.getNormolizeVector() | W;
	double cosBetweenElectroWaveAndNormalToFallPlane = polarizedVector.getNormolizeVector() | normalToPlaneOfIncidence;
	
	double alpha = acos((-1 * directionIncidentRay) | normal);
	double betta = acos((-1 * directionTransmittedRay) | normal);

	double pIncidentIntensity = cosBetweenElectroWaveAndFallPlane * cosBetweenElectroWaveAndFallPlane * intensity;
	double sIncidentIntensity = cosBetweenElectroWaveAndNormalToFallPlane * cosBetweenElectroWaveAndNormalToFallPlane * intensity;

	double pIntensityRatio = 1 - pow((tan(alpha - betta) / tan(alpha + betta)), 2);
	double sIntensityRatio = 1 - pow((sin(alpha - betta) / sin(alpha + betta)), 2);

	intensity = pIncidentIntensity * pIntensityRatio + sIncidentIntensity * sIntensityRatio;
#ifdef WITH_ABSORPTION
	if (isInnerRay)
	{
		double r = pow(M_E, -1 * 0.8481 * len);
		intensity *= r;
//		Ix.intensity *= r;
//		Iy.intensity *= r;
	}
#endif
	double pWaveRatio = (2 * sin(betta) * cos(alpha)) / (sin(alpha + betta) * cos(alpha - betta));
	double sWaveRatio = (2 * sin(betta) * cos(alpha)) / (sin(alpha + betta));

	PointVector3D sTransmittedWave = (polarizedVector | normalToPlaneOfIncidence) * sWaveRatio * normalToPlaneOfIncidence;
	PointVector3D pTransmittedWave = (polarizedVector | W) * pWaveRatio * (normalToPlaneOfIncidence * directionTransmittedRay);

	polarizedVector = sTransmittedWave + pTransmittedWave;
}

Ray* RayTransfer::createTransmittedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, double n1, double n2, bool isInner) const
{
	const PointVector3D& L = -1 * lRay.getRayDirection();

	double n = n1 / n2;
	double cosAngleOfIncidence = normal | L;
	double tmp = 1 + n * n *(cosAngleOfIncidence * cosAngleOfIncidence - 1);
	if (tmp < 0)
	{
		return nullptr;
	}
	double ratio = n * cosAngleOfIncidence - sqrt(tmp);
	PointVector3D transmittedDirection = ((-1 * n) * L) + ratio * normal;

	Ray* ray = new Ray(isInner);
	ray->setRayStart(hit + 3 * INTERSECTION_DELTA * transmittedDirection);
	ray->setRayDirection(transmittedDirection);
	return ray;
}

Ray RayTransfer::createReflectedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, bool isInner) const
{
	const PointVector3D& L = lRay.getRayDirection();

	double tmp = 2.0 * (normal | L);
	PointVector3D nTmp = normal * tmp;
	PointVector3D reflectedDirection = nTmp - L;

	Ray ray(isInner);
	ray.setRayStart(hit + INTERSECTION_DELTA * reflectedDirection);
	ray.setRayDirection(reflectedDirection);

	return ray;
}

// Parser

TransferFileParser::TransferFileParser(std::string& path) :path(path)
{
}

std::unique_ptr<TransferInfo> TransferFileParser::parse() const
{
	std::ifstream inputFile;
	string pathToOutpurFile;

	inputFile.open(path);

	getline(inputFile, pathToOutpurFile);

	std::unique_ptr<NewLens> lense = readLense(inputFile);
	std::unique_ptr<Quadrangle> quadrangle = readQuadrangle(inputFile);
	std::vector<PolarizedRay> rays;

	readPolarizedRays(rays, inputFile);

	inputFile.close();

	return make_unique<TransferInfo>(pathToOutpurFile, std::move(rays), std::move(lense), std::move(quadrangle));
}

std::unique_ptr<NewLens> TransferFileParser::readLense(std::ifstream& inputFile) const
{
	string fileStr;

	getline(inputFile, fileStr);
	RayTracingUtils::ParserUtils::removeComment(fileStr);

	getline(inputFile, fileStr);
	RayTracingUtils::ParserUtils::removeComment(fileStr);
	double leftRadius = atof(fileStr.c_str());

	getline(inputFile, fileStr);
	RayTracingUtils::ParserUtils::removeComment(fileStr);
	double rightRadius = atof(fileStr.c_str());

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double lensRadius = atof(fileStr.c_str());

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double cylinderHeight = atof(fileStr.c_str());

	PointVector3D directionStart = ParserUtils::readVertex(inputFile);
	PointVector3D directionFinsih = ParserUtils::readVertex(inputFile);
	PointVector3D lensCenter = ParserUtils::readVertex(inputFile);

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	vector<string> subStrings = ParserUtils::splitBySpace(fileStr);

//	std::unique_ptr<Color> diffuse = std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str()));
//	std::unique_ptr<Color> specular = std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str()));
//	double power = atof(subStrings.at(2).c_str());

//	OpticProperties opticProperties(std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str())), 
//		std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str())),
//		atof(subStrings.at(2).c_str()));

	std::string glassMaterial = std::string(subStrings.at(4).c_str());

	double refractionIndexes[SPECTRUM_SIZE];

	if (glassMaterial == "BK7")
	{
		for (int lambda = 380, i = 0; lambda <= 780; lambda += 5, i++)
		{
			refractionIndexes[i] = getBk7RefaractionIndex(static_cast<double>(lambda) / 1000.0);
		}
	}
	else if (glassMaterial == "SF11")
	{
		for (int lambda = 380, i = 0; lambda <= 780; lambda += 5, i++)
		{
			refractionIndexes[i] = getSf11RefractionIndex(static_cast<double>(lambda) / 1000.0);
		}
	}
	else if (glassMaterial == "CUSTOM_INCREASING")
	{
		getCustom(refractionIndexes, SPECTRUM_SIZE, 1.5, 1.7);
	}
	else if (glassMaterial == "CUSTOM_DECREASING")
	{
		getCustom(refractionIndexes, SPECTRUM_SIZE, 1.7, 1.5);
	}

	shared_ptr<OpticProperties> SurfaceOpticProperties = std::make_shared<AdvancedOpticProperties>(
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str()))),
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str()))),
		atof(subStrings.at(2).c_str()),
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(3).c_str()))),
		refractionIndexes, SPECTRUM_RENDER);

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	subStrings = ParserUtils::splitBySpace(fileStr);

//	diffuse = std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str())));
//	specular = std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str())));
//	power = atof(subStrings.at(2).c_str());
//
//	opticProperties = OpticProperties(new SpectrumColor(atof(subStrings.at(0).c_str())), new SpectrumColor(atof(subStrings.at(1).c_str())),
//		atof(subStrings.at(2).c_str()));
	//shared_ptr<OpticProperties> cylinderOpticProperties = nullptr;
	shared_ptr<OpticProperties> cylinderOpticProperties = std::make_shared<AdvancedOpticProperties>(
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str()))),
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str()))), 
		atof(subStrings.at(2).c_str()),
		std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(3).c_str()))),
		refractionIndexes, SPECTRUM_RENDER);

	return std::make_unique<NewLens>(leftRadius, rightRadius, lensRadius, cylinderHeight,
	                                 directionStart, directionFinsih, lensCenter, SurfaceOpticProperties, cylinderOpticProperties);

}

std::unique_ptr<Quadrangle> TransferFileParser::readQuadrangle(std::ifstream& inputFile) const
{
	PointVector3D vertex1 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex2 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex3 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex4 = ParserUtils::readVertex(inputFile);

	return std::make_unique<Quadrangle>(vertex1, vertex2, vertex3, vertex4, nullptr);
}

void TransferFileParser::readPolarizedRays(std::vector<PolarizedRay>& rays, std::ifstream& inputFile) const
{
	PointVector3D start, direction, polarization;
	std::string str;
	ParserUtils::readNextNotEmptyString(inputFile, str);
	int number = stoi(str);
	double intensity;

	for (int i = 0; i < number; i++)
	{
		start = ParserUtils::readVertex(inputFile);
		direction = ParserUtils::readVertex(inputFile);
		polarization = ParserUtils::readVertex(inputFile);

		ParserUtils::readNextNotEmptyString(inputFile, str);
		intensity = stod(str);

		Ray tmp = Ray(start, direction, false);
		PolarizedRay polarizedRay = PolarizedRay(start, direction, false,
			setPolarization(tmp.getRayDirection(), PointVector3D(1, 1, 0).getNormolizeVector()), intensity);
		rays.push_back(polarizedRay);
	}
}

std::vector<PolarizedRay>& TransferInfo::getRays()
{
	return rays;
}

NewLens* TransferInfo::getLens() const
{
	return lens.get();
}

Quadrangle* TransferInfo::getQuadranngle() const
{
	return quadranngle.get();
}

std::string& TransferInfo::getPath()
{
	return pathToOputputFile;
}
