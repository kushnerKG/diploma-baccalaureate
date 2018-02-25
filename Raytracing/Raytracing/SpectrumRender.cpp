#include "SpectrumRender.h"
#include <qimage.h>
#include "Ray.h"
#include "MatrixFactory.h"
#include "Camera.h"
#include "RgbIntensity.h"
#include "RenderSetting.h"
#include <thread>
#include <QDebug>
#include "AdvancedOpticProperties.h"
#include "SpectrumColor.h"
#include <Windows.h>
#include <fstream>
class SpectrumColor;
static bool isFill = false;

static Intensity* getDefaultIntensity();

double static getCPUTime()
{
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if (GetProcessTimes(GetCurrentProcess(),
		&createTime, &exitTime, &kernelTime, &userTime) != -1)
	{
		SYSTEMTIME userSystemTime;
		if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1)
			return static_cast<double>(userSystemTime.wHour) * 3600.0 +
			static_cast<double>(userSystemTime.wMinute) * 60.0 +
			static_cast<double>(userSystemTime.wSecond) +
			static_cast<double>(userSystemTime.wMilliseconds) / 1000.0;
	}
	return 0;
}

SpectrumRender::SpectrumRender(std::shared_ptr<SceneFileInfo> sceneFileInfo, std::shared_ptr<RenderInfo> renderInfo, 
	const Camera& camera, Matrix<double> rotateMatrix, Model& model):
			sceneFileInfo(sceneFileInfo), renderInfo(renderInfo), camera(camera), model(model)
{
	ambientColor = &((sceneFileInfo->getAmbientColor()));

	Matrix<double> inverseMatrix = rotateMatrix.transpose();

	upPoint = inverseMatrix * camera.getUpVector();
	inverseMatrix = inverseMatrix * MatrixFactory::getMoveMatrix(-camera.getViewPoint().getX(), -camera.getViewPoint().getY(), -camera.getViewPoint().getZ());
	inverseMatrix = MatrixFactory::getMoveMatrix(camera.getViewPoint().getX(), camera.getViewPoint().getY(), camera.getViewPoint().getZ()) * inverseMatrix;

	viewPoint = inverseMatrix * camera.getViewPoint();
	eyePoint = inverseMatrix * camera.getEyePoint();

	upVector = adjustUpVector(viewPoint, eyePoint, upPoint);

	PointVector3D direction = viewPoint - eyePoint;
	direction.normalizePointVector();

	PointVector3D upSq = upVector.getNormolizeVector();

	right = direction * upSq;
	right.normalizePointVector();

	PointVector3D center = eyePoint + (direction * (renderInfo->getZn()));
	PointVector3D shift = (upSq * (renderInfo->getSh() / 2.0)) + (right * (-(renderInfo->getSw() / 2.0)));

	start = center + shift;
}

SpectrumRender::~SpectrumRender()
{
}

void SpectrumRender::render(std::shared_ptr<QImage> image)
{
	int quality = renderInfo->getQuality();
	int width = image->width();
	int height = image->height();
	switch (quality)
	{
	case 0:
		intensities = std::make_shared<SpectrumIntensityMap>(height % 2 == 0 ? height / 2 : height / 2 + 2,
			width % 2 == 0 ? width / 2 : width / 2 + 2);
		roughtRenderImage(image);
		break;
	case 1:
		intensities = std::make_shared<SpectrumIntensityMap>(height, width);
		normalRenderImage(image);
		break;
	case 2:
		intensities = std::make_shared<SpectrumIntensityMap>(height * 2, width * 2);
		fineRenderImage(image);
		break;
	default:
		break;
	}
}

PointVector3D SpectrumRender::adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const
{
	PointVector3D eyeDirection = viewPoint - eyePoint;
	eyeDirection.normalizePointVector();
	PointVector3D right = upVector * eyeDirection;
	right.normalizePointVector();

	PointVector3D up = eyeDirection*right;
	up.normalizePointVector();

	return up;
}

void SpectrumRender::roughtRenderImage(std::shared_ptr<QImage> image)
{
}

void SpectrumRender::normalRenderImage(std::shared_ptr<QImage> image) const
{
	int width = image->width();
	int height = image->height();
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();

	PointVector3D eye = eyePoint;

	PointVector3D normalizeUp = upPoint.getNormolizeVector();

	int depth = renderInfo->getDepth();
	Color& bacgroundColor = renderInfo->getBackgroundColor();
	double startTime = getCPUTime();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			PointVector3D pixelIn3D = convert(Point(x, y), width, height, sw, sh);
			Ray ray(eye, pixelIn3D, false);
			for (int i = 0; i < SPECTRUM_SIZE; i++)
			{
				intensities->getIntensity(y, x)[i] = tracing(ray, depth - 1, i, true);
			}
			if ((intensities->getIntensity(y, x)).isBackground())
			{
				for (int i = 0; i < SPECTRUM_SIZE; i++)
				{
					intensities->getIntensity(y, x)[i] = bacgroundColor[i];
				}
				//intensities->getIntensity(y, x) += bacgroundColor;
			}
		}
		model.notifyListenerAboutStatusBar(double(y + 1) / double(height));
	}

	double time = getCPUTime() - startTime;

	std::ofstream file;
	file.open("time.txt", std::ios_base::app);

	file << "Spectr " << depth << " " << height << " " << width << " " << time << "\HEIGHT";


	//double inverseGamma = 1.0 / renderInfo->getGamma();

	for (int y = 0; y < image->height(); y++) {
		for (int x = 0; x < image->width(); x++) {
			//intensities->getIntensity(y, x).pow(inverseGamma);//но скоркк всего это лишнее
			image->setPixel(x, y, intensities->getIntensity(y, x).getColor());
		}
	}
}

void SpectrumRender::fineRenderImage(std::shared_ptr<QImage> image) const
{

	int width = image->width();
	int height = image->height();
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();

	PointVector3D eye = eyePoint;

	PointVector3D normalizeUp = upPoint.getNormolizeVector();

	int depth = renderInfo->getDepth();
	Color& bacgroundColor = renderInfo->getBackgroundColor();

	for (int y = 0; y < height * 2; y++)
	{
		for (int x = 0; x < width * 2; x++)
		{
			PointVector3D pixelIn3D = convert(Point(0.25 + 0.5 * x, 0.25 + 0.5 * y), width, height, sw, sh);
			Ray ray(eye, pixelIn3D, false);
			for (int i = 0; i < SPECTRUM_SIZE; i++)
			{
				intensities->getIntensity(y, x)[i] = tracing(ray, depth - 1, i, true);
			}

			if ((intensities->getIntensity(y, x)).isBackground())
			{
				intensities->getIntensity(y, x) += bacgroundColor;
			}
		}
		model.notifyListenerAboutStatusBar(double(y + 1) / double(height * 2));
	}

	double inverseGamma = 1.0 / renderInfo->getGamma();

	for (int y = 0; y < image->height(); y++) {
		for (int x = 0; x < image->width(); x++) {

			SpectrumIntensity tmpIntensity = SpectrumIntensity(0.0);

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					tmpIntensity += intensities->getIntensity(y * 2 + i, x * 2 + j);
				}
			}

			tmpIntensity /= 4.0;
			tmpIntensity.pow(inverseGamma);
			image->setPixel(x, y, tmpIntensity.getColor());
		}
	}
}

double SpectrumRender::tracing(Ray ray, int depth, int spectrumComponent, bool isFirstRay) const
{
	std::vector<std::unique_ptr<IntersectionInfo>> intersections = std::vector<std::unique_ptr<IntersectionInfo>>();

	for (auto shape : sceneFileInfo->getShapes())
	{		
		std::unique_ptr<IntersectionInfo> c = shape->checkRay(ray);
		if (c != nullptr)
		{
			intersections.push_back(std::move(c));
		}
	}

	if (isFirstRay && intersections.size() == 0)
	{
		return -1;
	}
	
	double intensity = 0;

	if (intersections.size() > 0)
	{
		std::unique_ptr<IntersectionInfo> hit = std::move(intersections.at(0));

		for (int i = 1; i < intersections.size(); i++)
		{
			if (intersections.at(i)->getLen() < hit->getLen()) {
				hit = std::move(intersections.at(i));
			}
		}
		std::shared_ptr<OpticProperties> op = hit->getOpticProperties();
		const PointVector3D& normal = hit->getNormal();
		const PointVector3D& hitIntersectionPoint = hit->getIntersectionPoints();
		const bool isTranperancy = hit->isTrasasparency();

		intensity = (op->getDiffuse()[spectrumComponent]) * ((*ambientColor)[spectrumComponent]);

		for (LightSource lightSource : sceneFileInfo->getLightsources())
		{
			double transmission = 1.0;

			if (!checkLightSource(lightSource, hitIntersectionPoint, transmission, spectrumComponent, ray))
			{
				Ray directionOnLightSource = Ray(hitIntersectionPoint, lightSource.getLightSourceCoordinates(), false);

				const Color& lightSourceColor = lightSource.getLightSourceColor();
				double dRatio = normal | directionOnLightSource.getRayDirection();
				double distanceToLight = (hitIntersectionPoint - lightSource.getLightSourceCoordinates()).getLen();
				double fatt = 1.0 / (1.0 + distanceToLight);

				Ray directionOnEye = Ray(hitIntersectionPoint, ray.getRayStart(), false);
				PointVector3D H = (directionOnLightSource.getRayDirection() + directionOnEye.getRayDirection());
				H.normalizePointVector();

				double sRatio = normal | H;

				if (dRatio < 0.0 && sRatio < 0.0)
				{
					continue;
				}

				sRatio = pow(sRatio, op->getPower());

				double tmp = op->getDiffuse()[spectrumComponent] * dRatio + op->getSpecular()[spectrumComponent] * sRatio;
				tmp *= lightSourceColor[spectrumComponent]; // проблема здесь
				tmp *= fatt;
				tmp *= transmission;

				intensity += tmp;
			}
		}

		if (depth > 0)
		{
			Ray reflected = createReflectedRay(normal, hit->getIntersectionPoints(), Ray(hit->getIntersectionPoints(),
				ray.getRayStart(), false), ray.isInner());

			double tmp = tracing(reflected, depth - 1, spectrumComponent, false);
			tmp *= op->getSpecular()[spectrumComponent];
			intensity += tmp;

			if (isTranperancy)
			{

				hit->getOpticProperties().get();
				AdvancedOpticProperties* advancedOpticProperties = static_cast<AdvancedOpticProperties*>(hit->getOpticProperties().get());

				double n1, n2;
				if (ray.isInner())
				{
					n1 = advancedOpticProperties->getN()[spectrumComponent];
					n2 = 1;
				}
				else
				{
					n1 = 1;
					n2 = advancedOpticProperties->getN()[spectrumComponent];
				}

				Ray* transmitted = createTransmittedRay(normal, hit->getIntersectionPoints(),
					ray, n1, n2, !ray.isInner());
				if (transmitted != nullptr)
				{
					double tmp1 = tracing(*transmitted, depth - 1, spectrumComponent, false);
					tmp1 *= advancedOpticProperties->getTransmitted()[spectrumComponent];
					intensity += tmp1;
					delete transmitted;
				}
			}
		}

		intersections.clear();
	}

	return intensity;
}

PointVector3D SpectrumRender::convert(Point point, int width, int height, float sw, float sh) const
{
	double deltaW = sw / width;
	double deltaH = sh / height;

	PointVector3D r = right * (point.getX() * deltaW);
	PointVector3D u = upVector * (point.getY() * -deltaH);

	PointVector3D tmp = start + (r + u);

	return tmp;
}

bool SpectrumRender::checkLightSource(const LightSource& lightSource, const PointVector3D& hit, double& transmission, int spectrumComponent, Ray& r) const
{
	const PointVector3D& lightCoord = lightSource.getLightSourceCoordinates();

	Ray ray = Ray(hit - INTERSECTION_DELTA * r.getRayDirection(), lightCoord, false);

	std::unique_ptr<IntersectionInfo> intersection;

	for (Shape* shape : sceneFileInfo->getShapes())
	{
		intersection = shape->checkRay(ray);
		if (intersection != nullptr && !intersection->isTrasasparency())
		{
			return true;
		}
		if (intersection != nullptr)
		{
			transmission *= intersection->getTransmission()[spectrumComponent];
		}
	}
	return false;
}

Ray SpectrumRender::createReflectedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, bool isInner) const
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

Ray* SpectrumRender::createTransmittedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, double n1, double n2, bool isInner) const
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
	ray->setRayStart(hit + INTERSECTION_DELTA * transmittedDirection);
	ray->setRayDirection(transmittedDirection);
	return ray;
}

double SpectrumRender::calculateMaxIntensity() const
{
	double max = intensities->getIntensity(1,2).getMaxIntensity();

	for (int i = 0; i < intensities->getRows(); i++){

		for (int j = 0; j < intensities->getColumns(); j++) {
			if (max < intensities->getIntensity(i,j).getMaxIntensity()) {
				max = intensities->getIntensity(i, j).getMaxIntensity();
			}
		}
	}

	return max;
}

void SpectrumRender::operator()(std::shared_ptr<QImage> image, Model& model, std::shared_ptr<TaskQueue> taskQueue)
{
	render(image);
	model.notifyListenerFromQtThread();
}

static Intensity* getDefaultIntensity()
{
	return new SpectrumIntensity();
}