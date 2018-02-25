#include "Render.h"
#include <qimage.h>
#include "Ray.h"
#include "IntersectionInfo.h"
#include "AdvancedOpticProperties.h"
#include "RenderSetting.h"
#include <thread>
#include <QDebug>
#include <ctime>
#include <fstream>
#include <Windows.h>

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

static Intensity* getDefaultIntensity(Color& backgroundColor);

Render::Render(std::shared_ptr<SceneFileInfo> sceneFileInfo, std::shared_ptr<RenderInfo> renderInfo, const Camera& camera, 
	Matrix<double> rotateMatrix, Model& model) :
	sceneFileInfo(sceneFileInfo), renderInfo(renderInfo), camera(camera), model(model)
{
	ambientColor = &((sceneFileInfo->getAmbientColor()) / 255.0);

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


Render::~Render()
{
}

void Render::render(std::shared_ptr<QImage> image)
{
	int quality = renderInfo->getQuality();
	int width = image->width();
	int height = image->height();

	switch (quality)
	{
		case 0:
			intensities = std::make_shared<RgbIntensityMap>(height % 2 == 0 ? height / 2 : height / 2 + 2,
				width % 2 == 0 ? width / 2 : width / 2 + 2);
			roughtRenderImage(image);
			break;
		case 1:
			intensities = std::make_shared<RgbIntensityMap>(height, width);
			normalRenderImage(image);
			break;
		case 2:
			intensities = std::make_shared<RgbIntensityMap>(height * 2, width * 2);
			fineRenderImage(image);
			break;
		default: 
			break;
	}
}

void Render::normalRenderImage(std::shared_ptr<QImage> image) const
{
	int width = image->width();
	int height = image->height();
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();

	PointVector3D eye = eyePoint;
	
	PointVector3D normalizeUp = upPoint.getNormolizeVector();

	int depth = renderInfo->getDepth();
	qDebug() << depth;
	Color& bacgroundColor = renderInfo->getBackgroundColor();


	double startTime = getCPUTime();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			if (depth > 0){
				PointVector3D pixelIn3D = convert(Point(x, y), width, height, sw, sh);
				Ray ray(eye, pixelIn3D, false);

				Intensity& i = tracing(ray, depth - 1, true);
				intensities->getIntensity(y, x) = i;
				delete &i;
			}
			else
			{
				intensities->getIntensity(y, x) = RgbIntensity(-1, -1, -1);
			}
		}
		model.notifyListenerAboutStatusBar(double(y + 1) / double(height));
	}
	double time = getCPUTime() - startTime;

	std::ofstream file;
	file.open("time.txt", std::ios_base::app);

	file << "RGB " << depth << " " << height << " " << width << " " << time << "\HEIGHT";

	double normolaizeRatio = 1.0 / calculateMaxIntensity();
	double inverseGamma = 1.0 / renderInfo->getGamma();
	
	for (int y = 0; y < image->height(); y++) {	
		for (int x = 0; x < image->width(); x++) {
			if (intensities->getIntensity(y, x).isBackground())
			{
				image->setPixel(x, y, RgbIntensity(bacgroundColor).getColor());
				continue;
			}
			intensities->getIntensity(y, x) *= normolaizeRatio;
			//intensities->getIntensity(y, x).pow(inverseGamma);

			image->setPixel(x, y, intensities->getIntensity(y, x).getColor());
		}
	}
}

void Render::roughtRenderImage(std::shared_ptr<QImage> image)//stub
{
}

void Render::fineRenderImage(std::shared_ptr<QImage> image) const
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
			
			Intensity& i = tracing(ray, depth - 1, true);
			intensities->getIntensity(y, x) = i;
			delete &i;
		}
		model.notifyListenerAboutStatusBar(double(y + 1) / double(height * 2));
	}

	double normolaizeRatio = 1.0 / calculateMaxIntensity();
	double inverseGamma = 1.0 / renderInfo->getGamma();

	for (int y = 0; y < image->height(); y++) 
	{
		for (int x = 0; x < image->width(); x++) 
		{
			double r = 0.0;
			double g = 0.0;
			double b = 0.0;
			int k = 0;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (!intensities->getIntensity(y * 2 + i, x * 2 + j).isBackground())
					{
						r += intensities->getIntensity(y * 2 + i, x * 2 + j).getComponent(0);
						g += intensities->getIntensity(y * 2 + i, x * 2 + j).getComponent(1);
						b += intensities->getIntensity(y * 2 + i, x * 2 + j).getComponent(2);
					}
					else
					{
						k++;
					}
					
				}
			}
			if (k == 4)
			{
				r = bacgroundColor[0];
				g = bacgroundColor[1];
				b = bacgroundColor[2];
			}
			else
			{
				r /= (4.0 - k);
				g /= (4.0 - k);
				b /= (4.0 - k);
			}

			double red = pow(r * normolaizeRatio, inverseGamma);
			double green = pow(g * normolaizeRatio, inverseGamma);
			double blue = pow(b * normolaizeRatio, inverseGamma);

			image->setPixel(x, y, qRgb(red * 255.0, green * 255.0, blue * 255.0));
		}
	}
}

PointVector3D Render::adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const
{
	PointVector3D eyeDirection = viewPoint - eyePoint;
	eyeDirection.normalizePointVector();
	PointVector3D right = upVector * eyeDirection;
	right.normalizePointVector();

	PointVector3D up = eyeDirection*right;
	up.normalizePointVector();

	return up;
}

inline PointVector3D Render::convert(Point point, int width, int height, float sw, float sh) const
{
	double deltaW = sw / width;
	double deltaH = sh / height;

	PointVector3D r = right * (point.getX() * deltaW);
	PointVector3D u = upVector * (point.getY() * -deltaH);

	PointVector3D tmp = start + (r + u);
	
	return tmp;
}

Intensity& Render::tracing(Ray ray, int depth, bool isFirstRay) const
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

	Intensity* intensity = nullptr;
	Intensity* born = nullptr;
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

		intensity = &(op->getDiffuse() * (*ambientColor));
		for (LightSource lightSource : sceneFileInfo->getLightsources())
		{
			Transmission transmission(RGB_RENDER, 1);
			if (!checkLightSource(lightSource, hitIntersectionPoint, transmission, ray))
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

				Intensity& tmp = op->getDiffuse() * dRatio;
				Intensity& tmp2 = op->getSpecular() * sRatio;
				tmp += tmp2;
				tmp *= lightSourceColor;
				tmp *= fatt;
				tmp *= transmission;
				*intensity += tmp;

				delete &tmp;
				delete &tmp2;
			}
		}
		intersections.clear();
		
		if (depth > 0)
		{
			Ray reflected = createReflectedRay(normal, hit->getIntersectionPoints(), Ray(hit->getIntersectionPoints(),
				ray.getRayStart(), false), ray.isInner());

			born = &tracing(reflected, depth - 1, false);
			*born *= op->getSpecular();
			if (isTranperancy)
			{
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
				Ray* transmitted = createTransmittedRay(normal, hit->getIntersectionPoints(), 
					ray, n1, n2, !ray.isInner());
				if (transmitted != nullptr)
				{
					Intensity& tmp1 = tracing(*transmitted, depth - 1, false);
					tmp1 *= advancedOpticProperties->getTransmitted();
					*intensity += tmp1;
					delete &tmp1;
					delete transmitted;
				}
			}
		}
	}
	if (born != nullptr)
	{
		*intensity += *born;
		delete born;
	}
	if (intensity == nullptr)
	{
		if (isFirstRay)
		{
			intensity = new RgbIntensity(-1, -1, -1);
		}
		else
		{
			intensity = new RgbIntensity(0, 0, 0);
		}
	}

	return *intensity;
}

bool Render::checkLightSource(const LightSource& lightSource, const PointVector3D& hit, Transmission& transmission, Ray& r) const
{
	const PointVector3D& lightCoord = lightSource.getLightSourceCoordinates();
	PointVector3D direction = lightCoord - hit;
	direction.normalizePointVector();
	direction[3] = 1.0;
	Ray ray = Ray(hit - INTERSECTION_DELTA * r.getRayDirection(), lightCoord, false);
	std::unique_ptr<IntersectionInfo> intersection;

	for (Shape* shape : sceneFileInfo->getShapes())
	{
		intersection = nullptr;
		if ((intersection = shape->checkRay(ray)) != nullptr && !intersection->isTrasasparency())
		{
			return true;
		}
		if ((intersection = shape->checkRay(ray)) != nullptr)
		{
			transmission *= intersection->getTransmission();
		}
	}
	return false;
}

inline Ray Render::createReflectedRay(const PointVector3D& normal, const PointVector3D& hit, const  Ray& lRay, bool isInner) const
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

Ray* Render::createTransmittedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, double n1, double n2, bool isInner) const
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

double Render::calculateMaxIntensity() const
{
	double max = intensities->getIntensity(0, 0).getMaxIntensity();

	for (int i = 0; i < intensities->getRows(); i++){

		for (int j = 0; j < intensities->getColumns(); j++) {

			if (intensities->getIntensity(i, j).isBackground())
			{
				continue;
			}

			if (max < intensities->getIntensity(i, j).getMaxIntensity()) {
				max = intensities->getIntensity(i, j).getMaxIntensity();
			}
		}
	}

	return max;
}

void Render::operator()(std::shared_ptr<QImage> image, Model& model, std::shared_ptr<TaskQueue> taskQueue, std::string pathToSave)
{
	render(image);

	model.notifyListenerFromQtThread();
}

static Intensity* getDefaultIntensity(Color& backgroundColor)
{
	return new RgbIntensity();
}