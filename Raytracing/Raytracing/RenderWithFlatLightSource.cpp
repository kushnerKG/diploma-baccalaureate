#include "RenderWithFlatLightSource.h"
#include <qimage.h>
#include "Ray.h"
#include "MatrixFactory.h"
#include "Camera.h"
#include "RgbIntensity.h"
#include "AdvancedOpticProperties.h"
#include <thread>
#include <QDebug>
#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Windows.h>

double tracingTime = 0;
typedef unsigned long long ull;

#include <intrin.h>
uint64_t rdtsc(){
	return __rdtsc();
}

/*
* CHECK_REFLECTION - ведется проверка влияния отраженных
* если включено, то в output выведется максимальное и среднее значение по всем лучам, попавшим в линзу и имеющих прямой луч
*/
#define CHECK_REFLECTION

/*
* DELTA_FILE_NAME - по каждому лучу, попавшим в линзу и имеющих прямой лучу, информация о влиянии преломленных юудет выведена в файл
*/
//#define DELTA_FILE_NAME "./Data/test_delta_extended4.csv"

/*
* WITH_SCHLICK - для расчета изображения без поляризации (только дисперсия) будет применяться SCHLICK
* иначе константы, заданные из файла
*/
#define WITH_SCHLICK

/*
* WITH_ABSORPTION - при расчете изображения с поляризации и дисперсия (Шлик и не шлик) будет применятся закон Бугера-Ламберат-Бера
* корректно только для материала N_SF11
* если незадефайнено, то закон не учитывается.
*/
#define WITH_ABSORPTION

double absorbtionRatioForN_SF11[SPECTRUM_SIZE] = {
	0.8481, 0.60931, 0.37664, 0.28939, 0.20433, 0.16019, 0.13427, 0.10898, 0.084287,
	0.075124, 0.066174, 0.057429, 0.05181, 0.04704, 0.042376, 0.037815, 0.033353, 0.031458,
	0.029604, 0.027788, 0.026011, 0.02427, 0.022565, 0.020894, 0.019256, 0.018039, 0.016846,
	0.015675, 0.014528, 0.013402, 0.012297, 0.011213, 0.010149, 0.0091048, 0.0085947, 0.0082214,
	0.0078549, 0.0074948, 0.007141, 0.0067934, 0.0064517, 0.0066136, 0.0067726, 0.006929, 0.0070828,
	0.0072341, 0.0073828, 0.0075292, 0.0076732, 0.0076732, 0.0076732, 0.0076732, 0.0076731, 0.0076731,
	0.0076731, 0.0076731, 0.0076731, 0.0074589, 0.007248, 0.0070401, 0.0068354, 0.0066336, 0.0064347,
	0.0062387, 0.0060455, 0.005936, 0.005828, 0.0057215, 0.0056165, 0.005513, 0.0054109, 0.0053101,
	0.0052108, 0.0051127, 0.005016, 0.0049205, 0.0048263, 0.0047334, 0.0046416, 0.004551, 0.0044616
};

float D65[SPECTRUM_SIZE];

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

RenderWithFlatLightSource::RenderWithFlatLightSource(std::shared_ptr<SceneFileInfo> sceneFileInfo, 
	std::shared_ptr<RenderInfo> renderInfo, const Camera& camera, Matrix<double> rotateMatrix, Model& model) :
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
	
	crt_get_d65(sourceSpectrFileName, ambientSpectrum);
	crt_get_d65(sourceSpectrFileName, D65);
}


RenderWithFlatLightSource::~RenderWithFlatLightSource()
{
}

void RenderWithFlatLightSource::render(std::shared_ptr<QImage> image, renderTypeWithFlatLight type)
{
	int quality = renderInfo->getQuality();
	int width = image->width();
	int height = image->height();

	switch (quality)
	{
	case 0:
		intensities = std::make_shared<SpectrumIntensityMap>(height % 2 == 0 ? height / 2 : height / 2 + 2,
			width % 2 == 0 ? width / 2 : width / 2 + 2);
		roughtRenderImage(image, type);
		break;
	case 1:
		intensities = std::make_shared<SpectrumIntensityMap>(height, width);
		normalRenderImage(image, type);
		break;
	case 2:
		intensities = std::make_shared<SpectrumIntensityMap>(height * 2, width * 2);
		fineRenderImage(image, type);
		break;
	default:
		break;
	}
}

void RenderWithFlatLightSource::operator()(std::shared_ptr<QImage> image, Model& model, 
	std::shared_ptr<TaskQueue> taskQueue, renderTypeWithFlatLight type)
{
	render(image, type);
	model.notifyListenerFromQtThread();
}

PointVector3D RenderWithFlatLightSource::adjustUpVector(const PointVector3D& viewPoint, const PointVector3D& eyePoint, const PointVector3D& upVector) const
{
	PointVector3D eyeDirection = viewPoint - eyePoint;
	eyeDirection.normalizePointVector();
	PointVector3D right = upVector * eyeDirection;
	right.normalizePointVector();

	PointVector3D up = eyeDirection*right;
	up.normalizePointVector();

	return up;
}

void RenderWithFlatLightSource::roughtRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type)//stub
{
}

void RenderWithFlatLightSource::createTree(Node* node, int depth) const
{
	if (depth > 0)
	{
		node->left = new Node();
		node->right = new Node();
		node->left->parent = node;
		node->right->parent = node;
		createTree(node->left, depth - 1);
		createTree(node->right, depth - 1);
	}
}

void RenderWithFlatLightSource::normalRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type) const
{
	int width = image->width();
	int height = image->height();
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();

	PointVector3D eye = eyePoint;
	PointVector3D normalizeUp = upPoint.getNormolizeVector();

	int depth = renderInfo->getDepth();
	Color& bacgroundColor = renderInfo->getBackgroundColor();



#ifdef CHECK_REFLECTION

	double deltaOneReflection = 0;
	double deltaTwoReflections = 0;
	int xMaxOneRefletion = -1, yMaxOneReflection = -1;
	int xMaxTwoRefletion = -1, yMaxTwoReflection = -1;

	std::vector<double> intensitiesWithoutReflections(SPECTRUM_SIZE);	
	std::vector<double> intensitiesWithOneReflections(SPECTRUM_SIZE);
	std::vector<double> intensitiesWithTwoReflections(SPECTRUM_SIZE);
	double totalSumma = 0;
	double totalSummaWithOneReflections = 0;
	double totalSummaWithTwoReflections = 0;

	double totalAmountForOneReflectionForMax = 0;
	double totalAmountForTwoReflectionForMax = 0;

	double amountForOneReflectionForMax = 0;
	double amountForTwoReflectionForMax = 0;


#ifdef DELTA_FILE_NAME
	std::ofstream file(DELTA_FILE_NAME);
#endif
#else
	double withoutReflection = 0;
#endif

	tracingTime = 0;

//	double h = 8.5;
//	double w = 0.839375;
//	
//	double g = sqrt(h*h + w*w);
//	
//	double cosAlpha = h / g;
//	qDebug() << cosAlpha;
//	PointVector3D dir(0, 0, -1);
	


	double stratTime = getCPUTime();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			PointVector3D pixelIn3D = convert(Point(x, y), width, height, sw, sh);
			Ray ray(eye, pixelIn3D, false);

//			if ((ray.getRayDirection() | dir) < cosAlpha)
//			{
//				continue;
//			}

			if (type == SPECRUM)
			{
				for (int i = 0; i < SPECTRUM_SIZE; i++)
				{
#ifdef WITH_SCHLICK
					intensities->getIntensity(y, x)[i] = tracingWithDispersionAndSchlick(ray, depth - 1, i);
#else
					intensities->getIntensity(y, x)[i] = tracingWithDispersion(ray, depth - 1, i);
#endif
				}
			}
			else
			{
				for (int i = 0; i < SPECTRUM_SIZE; i++)
				{
#ifdef CHECK_REFLECTION					
					intensitiesWithoutReflections[i] = 0;

					intensities->getIntensity(y, x)[i] = tracingWithPolarizationNewWay(ray, depth - 1, i, intensitiesWithoutReflections[i],
						intensitiesWithOneReflections[i], intensitiesWithTwoReflections[i]);
#else
					intensities->getIntensity(y, x)[i] = tracingWithPolarizationNewWay(ray, depth - 1, i, withoutReflection,
						withoutReflection, withoutReflection);
#endif
				}
			}
			

			if ((intensities->getIntensity(y, x)).isBackground())
			{
				for (int i = 0; i < SPECTRUM_SIZE; i++)
				{
					intensities->getIntensity(y, x)[i] = bacgroundColor[i];
				}
				continue;
			}
#ifdef CHECK_REFLECTION
			//double summaWithoutReflections = 0;
			double summaWithOneReflection = 0;
			double summaWithTwoReflections = 0;
			bool noDirect = false;
			bool noLens = false;
			for (auto val : intensitiesWithoutReflections)
			{
				if (val == -10000000) // флаг для того, чтобы отследить отсутствие прямых лучей
				{
					noDirect = true;
					break;
				}
				if (val == -20000000) // попадание в подложку
				{
					noLens = true;
					break;
				}
			}
			if (noDirect || noLens)
			{
				continue;
			}

			for (int i = 0; i < SPECTRUM_SIZE; i++)
			{
				summaWithOneReflection += abs(intensitiesWithOneReflections[i]);
			}

			for (int i = 0; i < SPECTRUM_SIZE; i++)
			{
				summaWithTwoReflections += abs(intensitiesWithTwoReflections[i]);
			}

			double summaAll = 0;
			for (int i = 0; i < SPECTRUM_SIZE; i++)
			{
				summaAll += abs(intensities->getIntensity(y, x)[i]);
			}
#ifdef DELTA_FILE_NAME
			file << x << ";" << y << ";" << summaAll << ";" << summaWithoutReflections << "\HEIGHT";
#endif
			if (summaAll == 0)
			{
				continue;
			}
			totalSumma += summaAll;
			totalSummaWithOneReflections += summaWithOneReflection;
			totalSummaWithTwoReflections += summaWithTwoReflections;
			

			count++;
			if (summaWithOneReflection / summaAll > deltaOneReflection)
			{
				deltaOneReflection = summaWithOneReflection / summaAll;
				totalAmountForOneReflectionForMax = summaAll;
				amountForOneReflectionForMax = summaWithOneReflection;
				xMaxOneRefletion = x;
				yMaxOneReflection = y;
			}

			if (summaWithTwoReflections / summaAll > deltaTwoReflections)
			{
				deltaTwoReflections = summaWithTwoReflections / summaAll;
				totalAmountForTwoReflectionForMax = summaAll;
				amountForTwoReflectionForMax = summaWithTwoReflections;
				xMaxTwoRefletion = x;
				yMaxTwoReflection = y;
			}
			
#endif
		}
		model.notifyListenerAboutStatusBar(double(y + 1) / double(height));
	}

	/*double time = getCPUTime() - stratTime;

	std::ofstream file;
	file.open("time.txt", std::ios_base::app);
	if (type == SPECRUM)
	{
		file << "SCHLICK " << depth << " " << height << " " << width << " " << time << " " << tracingTime << "\HEIGHT";	
	}
	else
	{
		file << "FRENEL " << depth << " " << height << " " << width << " " << time << " " << tracingTime << "\HEIGHT";
	}
	
	file.close(); */
#ifdef CHECK_REFLECTION
	qDebug() << "COUNT " << count;
	
	qDebug() << "MAX RATIO One reflection" << deltaOneReflection << " " << amountForOneReflectionForMax << " " << totalAmountForOneReflectionForMax;
	qDebug() << "MAX RATIO Two reflections" << deltaTwoReflections << " " << amountForTwoReflectionForMax << " " << totalAmountForTwoReflectionForMax;

	qDebug() << "AVG RATIO One Reflection" << totalSummaWithOneReflections / totalSumma << " " << totalSummaWithOneReflections << " " << totalSumma;
	qDebug() << "AVG RATIO Two Reflections" << totalSummaWithTwoReflections / totalSumma << " " << totalSummaWithTwoReflections << " " << totalSumma;;
#endif

#ifdef DELTA_FILE_NAME
	file.close();
#endif

	for (int y = 0; y < image->height(); y++) {
		for (int x = 0; x < image->width(); x++) {
			image->setPixel(x, y, intensities->getIntensity(y, x).getColor());
		}
	}
}

void RenderWithFlatLightSource::fineRenderImage(std::shared_ptr<QImage> image, renderTypeWithFlatLight type) const//stub
{
}

void RenderWithFlatLightSource::deleteTree(Node* tree) const
{
	if (tree == nullptr)
	{
		return;
	}
	if (tree->left != nullptr)
	{
		deleteTree(tree->left);
	}
	
	if (tree->right != nullptr)
	{
		deleteTree(tree->right);
	}

	if (tree->ray)
	{
		delete tree->ray;
	}
	
	delete tree;
}



void calculateAlphaAndBetta(double& alpha, double& betta, double n1, double n2, PointVector3D& incident, PointVector3D normal)
{
	alpha = acos((incident | normal) * -1);
	betta = asin(n1 * sin(alpha) / n2);
}

double RenderWithFlatLightSource::tracingWithPolarization(class Ray& ray, int depth, int spectrumComponent, double& intensityWithoutReflections) const
{
	std::vector<Node*> leaves = std::vector<Node*>();
	intensityWithoutReflections = -10000000;
 	Node* root = buildTree(ray, leaves, depth, spectrumComponent, 1.0, 1);
	if (leaves.size() == 1 && leaves[0]->parent == nullptr && leaves[0]->left == nullptr &&
		leaves[0]->right == nullptr && leaves[0]->intersectionInfo == nullptr)
	{
		deleteTree(root);
		return -1;
	}
	double intensity = 0;
	if (root != nullptr)
	{
		root->parent = nullptr;
		
		for (Node *node : leaves)
		{
			bool withReflections = false;
			const PointVector3D direction = (node->ray->getRayDirection()) * -1.0;
			PointVector3D OX = PointVector3D(1, 0, 0);
			
			PointVector3D Ex = OX * direction;
			PointVector3D Ey = direction * Ex;
			Ex.normalizePointVector();
			Ey.normalizePointVector();
			double tmp = 0;
			if (node->intersectionInfo == nullptr) //берем d65 или расейяный из файла
			{
				//tmp = ambientSpectrum[spectrumComponent] * 0.0;
				continue;
			}
			else
			{
				tmp = node->intersectionInfo->getOpticProperties()->getDiffuse()[spectrumComponent];
			}
			Node* tmpNode = node;
			while (tmpNode->parent != nullptr)
			{
				if (!tmpNode->isLeft){
					nodePassageForTransmitted(tmpNode, Ex);
				}
				else
				{
					withReflections = true;
					nodePassageForReflection(tmpNode, Ex);
				}
#ifdef WITH_ABSORPTION
				if (tmpNode->ray->isInner())
				{
					double len = tmpNode->intersectionInfo->getLen();
					double r = pow(M_E, -1 * absorbtionRatioForN_SF11[spectrumComponent] * len);
					Ex *= r;
				}
#endif
				tmpNode = tmpNode->parent;
			}

			tmpNode = node;
			while (tmpNode->parent != nullptr)
			{
				if (!tmpNode->isLeft){
					nodePassageForTransmitted(tmpNode, Ey);
				}
				else
				{
					withReflections = true;
					nodePassageForReflection(tmpNode, Ey);
				}
#ifdef WITH_ABSORPTION
				if (tmpNode->ray->isInner())
				{
					double len = tmpNode->intersectionInfo->getLen();
					double r = pow(M_E, -1 * absorbtionRatioForN_SF11[spectrumComponent] * len);
					Ey *= r;
				}
#endif
				tmpNode = tmpNode->parent;
			}
			intensity += tmp * (Ex.getLen() + Ey.getLen()) / 2;
			if (!withReflections)
			{
#ifdef CHECK_REFLECTION
				if (intensityWithoutReflections == -10000000){
					intensityWithoutReflections = 0;
				}
#endif
				intensityWithoutReflections += tmp * (Ex.getLen() + Ey.getLen()) / 2;


#ifdef CHECK_REFLECTION
				if (leaves.size() == 1)//попадание в подложку
				{
					intensityWithoutReflections = -20000000;
				}
#endif
			}
		}
		deleteTree(root);
	}
	return intensity;
}



double RenderWithFlatLightSource::tracingWithPolarizationNewWay(Ray& ray, int depth, int spectrumComponent,
	double& intensityWithoutReflections, double& oneReflections, double& twoReflections) const
{
	std::vector<Node*> leaves = std::vector<Node*>();
	intensityWithoutReflections = -10000000;
	oneReflections = 0;
	twoReflections = 0;

	Node* root = buildTree(ray, leaves, depth, spectrumComponent, 1.0, 1);

	if (leaves.size() == 1 && leaves[0]->parent == nullptr && leaves[0]->left == nullptr &&
		leaves[0]->right == nullptr && leaves[0]->intersectionInfo == nullptr)
	{
		deleteTree(root);
		return -1;
	}
	double intensity = 0;
	double stratTime = getCPUTime();
	if (root != nullptr)
	{
		root->parent = nullptr;

		for (Node *node : leaves)
		{
			bool withReflections = false;
			const PointVector3D direction = (node->ray->getRayDirection()) * -1.0;
			PointVector3D OX = PointVector3D(1, 0, 0);

			double startNotPolarizedRayIntensity = 0;
			if (node->intersectionInfo == nullptr) //берем d65 или расейяный из файла
			{
				//tmp = ambientSpectrum[spectrumComponent] * 0.0;
				continue;
			}
			else
			{
				//startNotPolarizedRayIntensity = D65[spectrumComponent];
				startNotPolarizedRayIntensity = node->intersectionInfo->getOpticProperties()->getDiffuse()[spectrumComponent];
				//startNotPolarizedRayIntensity = D65[spectrumComponent];
			}

			PointVector3D Ex = OX * direction;
			PointVector3D Ey = direction * Ex;
			Ex.normalizePointVector();
			Ey.normalizePointVector();

			IntensityWithElectroWave Ix;
			Ix.electroWave = Ex;
			Ix.intensity = startNotPolarizedRayIntensity / 2;
			IntensityWithElectroWave Iy;
			Iy.electroWave = Ey;
			Iy.intensity = startNotPolarizedRayIntensity / 2;

			Node* tmpNode = node;
			while (tmpNode->parent != nullptr)
			{
				if (!tmpNode->isLeft)
				{
					nodePassageForTransmittedNewWay(tmpNode, Ix);
					nodePassageForTransmittedNewWay(tmpNode, Iy);
				}
				else
				{
					nodePassageForReflectionNewWay(tmpNode, Ix);
					nodePassageForReflectionNewWay(tmpNode, Iy);
				}
#ifdef WITH_ABSORPTION
				if (tmpNode->ray->isInner())
				{
					double len = tmpNode->intersectionInfo->getLen();
					double r = pow(M_E, -1 * absorbtionRatioForN_SF11[spectrumComponent] * len);
					Ix.intensity *= r;
					Iy.intensity *= r;
				}
#endif
				tmpNode = tmpNode->parent;
			}
			
//			tmpNode = node;
//			while (tmpNode->parent != nullptr)
//			{
//				if (!tmpNode->isLeft)
//				{
//					nodePassageForTransmittedNewWay(tmpNode, Iy);
//				}
//				else
//				{
//					nodePassageForReflectionNewWay(tmpNode, Iy);
//				}
//#ifdef WITH_ABSORPTION
//				if (tmpNode->ray->isInner())
//				{
//					double len = tmpNode->intersectionInfo->getLen();
//					double r = pow(M_E, -1 * absorbtionRatioForN_SF11[spectrumComponent] * len);
//					Iy.intensity *= r;
//				}
//#endif
//				tmpNode = tmpNode->parent;
//			}

			intensity += Ix.intensity;
			intensity += Iy.intensity;

//			if (!withReflections)
//			{
#ifdef CHECK_REFLECTION
				if (node->level == 3)
				{
					if (intensityWithoutReflections == -10000000)
					{
						intensityWithoutReflections = 0;
					}
					intensityWithoutReflections += (Ix.intensity + Iy.intensity);
				}
				if (node->level == 5){
//					if (oneReflections == -10000000){
//						oneReflections = 0;
//					}
					oneReflections += (Ix.intensity + Iy.intensity);
				}
				if (node->level == 7){
//					if (twoReflections == -10000000){
//						twoReflections = 0;
//					}
					twoReflections += (Ix.intensity + Iy.intensity);
				}

				if (leaves.size() == 1 || leaves.at(0)->reason == INTERSECTION_WITH_LIGHT_SOURCE)//попадание в подложку
				{
					intensityWithoutReflections = -20000000;
				}
			
#endif
			//}
		}
		deleteTree(root);
	}


	tracingTime += (getCPUTime() - stratTime);

	return intensity;
}

void RenderWithFlatLightSource::nodePassageForTransmittedNewWay(Node* node, IntensityWithElectroWave& intensityWithWave) const
{
	PointVector3D incident = -1 * node->ray->getRayDirection();
	PointVector3D intersectionNormal = -1 * (node->parent->intersectionInfo->getNormal());
	PointVector3D transmitted = -1 * node->parent->ray->getRayDirection();

	PointVector3D normalToPlaneOfIncidence = incident * intersectionNormal;

	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * incident;
	W.normalizePointVector();

	//меняется так как трассируем в другую стороны
	double refractionIndexFrom = node->refractionIndexTo;
	double refractionIndexTo = node->refractionIndexFrom;

	double cosBetweenElectroWaveAndFallPlane = intensityWithWave.electroWave | W;
	double cosBetweenElectroWaveAndNormalToFallPlane = intensityWithWave.electroWave | normalToPlaneOfIncidence;
	
	//вычисление угла
	double alpha = acos((-1 * incident) | intersectionNormal);
	double betta = acos((-1 * transmitted) | intersectionNormal);
	
	if (abs(alpha) < 0.00001 && abs(betta) < 0.00001)
	{
		double intensityRatio = (4 * refractionIndexFrom * refractionIndexTo) / pow(refractionIndexFrom + refractionIndexTo, 2);
		intensityWithWave.intensity *= intensityRatio;
		return;
	}
	double pIncidentIntensity = cosBetweenElectroWaveAndFallPlane * cosBetweenElectroWaveAndFallPlane * intensityWithWave.intensity;
	double sIncidentIntensity = cosBetweenElectroWaveAndNormalToFallPlane * cosBetweenElectroWaveAndNormalToFallPlane * intensityWithWave.intensity;
	
	double pIntensityRatio = 1 - pow((tan(alpha - betta) / tan(alpha + betta)), 2);
	double sIntensityRatio = 1 - pow((sin(alpha - betta) / sin(alpha + betta)), 2);

	intensityWithWave.intensity = pIncidentIntensity * pIntensityRatio + sIncidentIntensity * sIntensityRatio;

	double pWaveRatio = (2 * sin(betta) * cos(alpha)) / (sin(alpha + betta) * cos(alpha - betta));
	double sWaveRatio = (2 * sin(betta) * cos(alpha)) / (sin(alpha + betta));
		 
	PointVector3D sTransmittedWave = cosBetweenElectroWaveAndNormalToFallPlane * sWaveRatio * normalToPlaneOfIncidence;
	PointVector3D pTransmittedWave = cosBetweenElectroWaveAndFallPlane * pWaveRatio * (normalToPlaneOfIncidence * transmitted);

	intensityWithWave.electroWave = sTransmittedWave + pTransmittedWave;
	intensityWithWave.electroWave.normalizePointVector();
}

void RenderWithFlatLightSource::nodePassageForReflectionNewWay(Node* node, IntensityWithElectroWave& intensityWithWave) const
{
	PointVector3D incident = -1.0 * node->ray->getRayDirection();
	PointVector3D intersectionNormal = node->parent->intersectionInfo->getNormal();
	PointVector3D reflection = -1.0 * node->parent->ray->getRayDirection();

	PointVector3D normalToPlaneOfIncidence = incident * intersectionNormal;
	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * incident;
	W.normalizePointVector();

	//остается одинаковыми при обратной трассировки, так как отраженный остается с то же стороны, для преломленного должно быть наоборот
	double refractionIndexFrom = node->refractionIndexFrom;
	double refractionIndexTo = node->refractionIndexTo;

	double alpha, betta;
	alpha = acos((-1 * incident) | intersectionNormal);
	if (node->parent->right != nullptr){
		betta = asin((refractionIndexFrom * sin(alpha)) / refractionIndexTo);
	}
	else
	{
		betta = M_PI / 2;
	}
	
	if (abs(alpha) < 0.00001 && abs(betta) < 0.00001)
	{
		double intensityRatio = pow((refractionIndexFrom - refractionIndexTo) / (refractionIndexFrom + refractionIndexTo), 2);
		intensityWithWave.intensity *= intensityRatio;
		return;
	}

	double cosBetweenElectroWaveAndFallPlane = intensityWithWave.electroWave | W;
	double cosBetweenElectroWaveAndNormalToFallPlane = intensityWithWave.electroWave | normalToPlaneOfIncidence;

	double pIncidentIntensity = cosBetweenElectroWaveAndFallPlane * cosBetweenElectroWaveAndFallPlane * intensityWithWave.intensity;
	double sIncidentIntensity = cosBetweenElectroWaveAndNormalToFallPlane * cosBetweenElectroWaveAndNormalToFallPlane * intensityWithWave.intensity;
	
	double pIntensityRatio = pow((tan(alpha - betta) / tan(alpha + betta)), 2);
	double sIntensityRatio = pow((sin(alpha - betta) / sin(alpha + betta)), 2);
	intensityWithWave.intensity = pIncidentIntensity * pIntensityRatio + sIncidentIntensity * sIntensityRatio;

	double pWaveRatio = -1 * tan(alpha - betta) / tan(alpha + betta);
	double sWaveRatio = -1 * sin(alpha - betta) / sin(alpha + betta);

	PointVector3D sTransmittedWave = cosBetweenElectroWaveAndNormalToFallPlane * sWaveRatio * normalToPlaneOfIncidence;
	PointVector3D pTransmittedWave = cosBetweenElectroWaveAndFallPlane * pWaveRatio * (reflection * normalToPlaneOfIncidence);

	intensityWithWave.electroWave = sTransmittedWave + pTransmittedWave;
	intensityWithWave.electroWave.normalizePointVector();
}

static double getSchkickRefkectionRatio(PointVector3D& incident, PointVector3D& normal, double refractionIndexFrom, double refractionIndexTo)
{
	double R0 = pow((refractionIndexFrom - refractionIndexTo) / (refractionIndexFrom + refractionIndexTo), 2);
	return R0 + (1 - R0)*(1 + (incident | normal));//+ т.к. у меня incident и normal направлены в разные стороны
}

double RenderWithFlatLightSource::tracingWithDispersionAndSchlick(Ray& ray, int depth, int spectrumComponent) const
{
	std::vector<Node*> leaves = std::vector<Node*>();

	Node* root = buildTree(ray, leaves, depth, spectrumComponent, 1.0, 1);
	
	if (leaves.size() == 1 && leaves[0]->parent == nullptr && leaves[0]->left == nullptr &&
		leaves[0]->right == nullptr && leaves[0]->intersectionInfo == nullptr)
	{
		deleteTree(root);
		return -1;
	}

	double intensity = 0;
	double stratTime = getCPUTime();
	if (root != nullptr)
	{
		root->parent = nullptr;
		for (Node *node : leaves)
		{
			double tmp = 0;
			if (node->intersectionInfo == nullptr) //берем d65 или расейяный из файла
			{
				continue;
				//tmp = ambientSpectrum[spectrumComponent] * 0.0;
			}
			tmp = node->intersectionInfo->getOpticProperties()->getDiffuse()[spectrumComponent];
			Node* tmpNode = node;

			double ratio = 1;
			
			while (tmpNode->parent != nullptr)
			{
				PointVector3D incident = -1 * tmpNode->ray->getRayDirection();
				
				double refractionIndexFrom = tmpNode->refractionIndexFrom;
				double refractionIndexTo = tmpNode->refractionIndexTo;

				if (!tmpNode->isLeft){//преломленный
					PointVector3D intersectionNormal = -1 * (tmpNode->parent->intersectionInfo->getNormal());
					double reflectionSchlickRatio = getSchkickRefkectionRatio(incident, intersectionNormal, refractionIndexFrom, refractionIndexTo);
					ratio *= (1 - reflectionSchlickRatio);
				}
				else//отраженный
				{
					PointVector3D intersectionNormal = (tmpNode->parent->intersectionInfo->getNormal());
					double reflectionSchlickRatio = getSchkickRefkectionRatio(incident, intersectionNormal, refractionIndexFrom, refractionIndexTo);
					ratio *= reflectionSchlickRatio;
				}
#ifdef WITH_ABSORPTION
				if (tmpNode->ray->isInner())
				{
					double len = tmpNode->intersectionInfo->getLen();
					double r = pow(M_E, -1 * absorbtionRatioForN_SF11[spectrumComponent] * len);
					ratio *= r;
				}
#endif
				tmpNode = tmpNode->parent;
			}
			intensity += ratio * tmp;
		}
		deleteTree(root);
	}
	tracingTime += (getCPUTime() - stratTime);
	return intensity;
}

double RenderWithFlatLightSource::tracingWithDispersion(Ray& ray, int depth, int spectrumComponent) const
{
	std::vector<Node*> leaves = std::vector<Node*>();

	Node* root = buildTree(ray, leaves, depth, spectrumComponent, 1.0, 1);

	if (leaves.size() == 1 && leaves[0]->parent == nullptr && leaves[0]->left == nullptr &&
		leaves[0]->right == nullptr && leaves[0]->intersectionInfo == nullptr)
	{
		deleteTree(root);
		return -1;
	}

	double intensity = 0;
	if (root != nullptr)
	{
		root->parent = nullptr;

		for (Node *node : leaves)
		{
			double tmp = 0;
			if (node->intersectionInfo == nullptr) //берем d65 или расейяный из файла
			{
				continue;
				//tmp = ambientSpectrum[spectrumComponent] * 0.0;
			}
			
			tmp = node->intersectionInfo->getOpticProperties()->getDiffuse()[spectrumComponent];

			Node* tmpNode = node;
			
			double ratio = 1;
			
			while (tmpNode->parent != nullptr)
			{
				AdvancedOpticProperties* advancedOpticProperties = static_cast<AdvancedOpticProperties*>(node->parent->intersectionInfo->getOpticProperties().get());
				if (!tmpNode->isLeft){
					ratio *= advancedOpticProperties->getTransmitted()[spectrumComponent];
				}
				else
				{
					ratio *= advancedOpticProperties->getSpecular()[spectrumComponent];
				}
				tmpNode = tmpNode->parent;
			}
			intensity += ratio * tmp;
		}
		deleteTree(root);
	}
	return intensity;
}

void RenderWithFlatLightSource::nodePassageForTransmitted(Node* node, PointVector3D& E0) const
{
	PointVector3D incident = -1 * node->ray->getRayDirection();
	PointVector3D intersectionNormal = -1 * (node->parent->intersectionInfo->getNormal());
	PointVector3D transmitted = -1 * node->parent->ray->getRayDirection();

	PointVector3D normalToPlaneOfIncidence = incident * intersectionNormal;
	
	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * incident;
	W.normalizePointVector();

	//меняется так как трассируем в другую стороны
	double refractionIndexFrom = node->refractionIndexTo;
	double refractionIndexTo = node->refractionIndexFrom;

	double sIncidentIntensity = E0 | normalToPlaneOfIncidence;// * normalToPlaneOfIncidence;
	double pIncidentIntensity = E0 | W;// *W;

	//вычисление угла
	double alpha = acos((-1 * incident) | intersectionNormal);
	double betta = acos((-1 * transmitted) | intersectionNormal);
	
	if (abs(alpha) < 0.00001 && abs(betta) < 0.00001)
	{
		double ratio = (4 * refractionIndexFrom * refractionIndexTo) / pow(refractionIndexFrom + refractionIndexTo, 2);
		E0 = E0 * ratio;
		return;
	}

	//вычисляем коэффициенты
	double pIntensityRatio = 1 - pow((tan(alpha - betta) / tan(alpha + betta)), 2);
	double sIntensityRatio = 1 - pow((sin(alpha - betta) / sin(alpha + betta)), 2);

	//вычисляем выходные коэффициенты
	PointVector3D sTransmittedIntensity = sIncidentIntensity * sIntensityRatio * normalToPlaneOfIncidence;
	PointVector3D pTransmittedIntensity = pIncidentIntensity * pIntensityRatio * (normalToPlaneOfIncidence * transmitted);

	E0 = sTransmittedIntensity + pTransmittedIntensity;
}

void RenderWithFlatLightSource::nodePassageForReflection(Node* node, PointVector3D& E0) const
{
	PointVector3D incident = -1.0 * node->ray->getRayDirection();
	PointVector3D intersectionNormal = node->parent->intersectionInfo->getNormal();
	PointVector3D reflection = -1.0 * node->parent->ray->getRayDirection();

	PointVector3D normalToPlaneOfIncidence = incident * intersectionNormal;
	normalToPlaneOfIncidence.normalizePointVector();

	PointVector3D W = normalToPlaneOfIncidence * incident;
	W.normalizePointVector();

	//остается одинаковыми при обратной трассировки, так как отраженный остается с то же стороны, для преломленного должно быть наоборот
	double refractionIndexFrom = node->refractionIndexFrom;
	double refractionIndexTo = node->refractionIndexTo;

	double sIncidentIntensity = E0 | normalToPlaneOfIncidence;// * normalToPlaneOfIncidence;
	double pIncidentIntensity = E0 | W;// *W;
	double alpha, betta;
	alpha = acos((-1 * incident) | intersectionNormal);
	if (node->parent->right != nullptr){
		betta = asin((refractionIndexFrom * sin(alpha)) / refractionIndexTo);
	}
	else
	{
		betta = M_PI / 2;
	}

	double pIntensityRatio;
	double sIntensityRatio;

	if (abs(alpha) > 0.0000001 && abs(betta) > 0.0000001)
	{
		pIntensityRatio = pow((tan(alpha - betta) / tan(alpha + betta)), 2);
		sIntensityRatio = pow((sin(alpha - betta) / sin(alpha + betta)), 2);
		PointVector3D sTransmittedIntensity = sIncidentIntensity * sIntensityRatio * normalToPlaneOfIncidence;
		PointVector3D pTransmittedIntensity = pIncidentIntensity * pIntensityRatio * (reflection * normalToPlaneOfIncidence);

		E0 = sTransmittedIntensity + pTransmittedIntensity;
	}
	else
	{
		double ratio = pow((refractionIndexFrom - refractionIndexTo) / (refractionIndexFrom + refractionIndexTo), 2);
		E0 = E0 * ratio;
	}
}

static void identifyRefractionIndex(double& refractionIndexFrom, double& refractionIndexTo, int spectrumComponent,
	OpticProperties* opticProperties, bool isInner)
{
	AdvancedOpticProperties* advancedOpticProperties = static_cast<AdvancedOpticProperties*>(opticProperties);

	if (isInner)
	{
		refractionIndexFrom = advancedOpticProperties->getN()[spectrumComponent];
		refractionIndexTo = 1;
	}
	else
	{
		refractionIndexFrom = 1;
		refractionIndexTo = advancedOpticProperties->getN()[spectrumComponent];
	}
}

static std::unique_ptr<IntersectionInfo> findNearestIntersection(Ray& ray, std::vector<std::unique_ptr<IntersectionInfo>>& intersections,
	std::shared_ptr<SceneFileInfo> sceneFileInfo)
{
	for (auto shape : sceneFileInfo->getShapes())
	{
		auto c = shape->checkRay(ray);
		if (c != nullptr)
		{
			intersections.push_back(std::move(c));
		}
	}
	if (intersections.size() > 0)
	{
		//ищем близжайщую точку пересечения
		std::unique_ptr<IntersectionInfo> hit = std::move(intersections.at(0));
		for (int i = 1; i < intersections.size(); i++)
		{
			if (intersections.at(i)->getLen() < hit->getLen()) {
				hit = std::move(intersections.at(i));
			}
		}
		return hit;
	}

	return nullptr;
}

/*
 * метод строит дерево трассировки
 * в std::vector<Node*>& leaves записываются все учитываемые листья (то есть те, которые либо попали в источник, либо в габаритный бокс)
 * по мимо этого запоминаем причину остановки
 * 
 *
 */
RenderWithFlatLightSource::Node* RenderWithFlatLightSource::buildTree(Ray& ray, std::vector<Node*>& leaves, int depth, int spectrumComponent, 
	double refractionIndex, int level) const
{
	Node* node = new Node();
	node->level = level;
	node->ray = new Ray(ray); //сохраняем падающий луч
	std::vector<std::unique_ptr<IntersectionInfo>> intersections = std::vector<std::unique_ptr<IntersectionInfo>>();
	std::unique_ptr<IntersectionInfo> hit = findNearestIntersection(ray, intersections, sceneFileInfo);
	
	//если есть хоть одна работаем с ней иначе считаем что луч попал в габаритный бокс
	if (hit != nullptr)
	{
		node->intersectionInfo = std::move(hit); // сохраняем среды куда входим. в hit тпереь ничего нет.
		intersections.clear();

		//проверяем истоник ли это или нет
		if (node->intersectionInfo->isIntesctionWithLightSource())
		{
			//тут добавляем лист и интенсивность;
			node->right = nullptr;
			node->left = nullptr;
			node->reason = INTERSECTION_WITH_LIGHT_SOURCE;
			leaves.push_back(node);
			return node;
		}

		if (depth > 0)
		{
			std::shared_ptr<OpticProperties> op = node->intersectionInfo->getOpticProperties();
			const PointVector3D& normal = node->intersectionInfo->getNormal();
			const PointVector3D& hitIntersectionPoint = node->intersectionInfo->getIntersectionPoints();
			
			//обрабатываем возможный отраженный луч
			Ray reflected = createReflectedRay(normal, hitIntersectionPoint,
				Ray(hitIntersectionPoint, ray.getRayStart(), false), ray.isInner());

			Node* left = buildTree(reflected, leaves, depth - 1, spectrumComponent, refractionIndex, level + 1);
			if (left != nullptr){
				node->left = left;
				left->parent = node;
				left->isLeft = true;
				if (node->intersectionInfo->isTrasasparency())
				{
					identifyRefractionIndex(left->refractionIndexFrom, left->refractionIndexTo,
						spectrumComponent, node->intersectionInfo->getOpticProperties().get(), ray.isInner());
				}
			}
			else
			{
				node->left = nullptr;
			}

			//обрабатываем возможный преломленный луч
			if (node->intersectionInfo->isTrasasparency())
			{
				double n1, n2;
				identifyRefractionIndex(n1, n2, spectrumComponent, node->intersectionInfo->getOpticProperties().get(), ray.isInner());

				Ray* transmitted = createTransmittedRay(normal, node->intersectionInfo->getIntersectionPoints(),
					ray, n1, n2, !ray.isInner());
				
				if (transmitted != nullptr)
				{
					Node* right = buildTree(*transmitted, leaves, depth - 1, spectrumComponent, n2, level + 1);
					delete transmitted;
					if (right != nullptr)
					{
						node->right = right;
						right->parent = node;
						right->isLeft = false;
						right->refractionIndexFrom = n1;
						right->refractionIndexTo = n2;
					}
					else
					{
						node->right = nullptr;
					}
				}
				else
				{
					//qDebug() << "nulptr";
					node->right = nullptr;
				}
			}

			return node;
		}
		//если была достигнута предельная глубина трассировки, то запоминаем причину
		node->reason = ACHIEVED_MAX_DEPTH;
		node->right = nullptr;
		node->left = nullptr;
		return node;
	}
	
	//все улетело в бесконечность
	//берем 0.4 d65 или рассеяный из файла
	node->intersectionInfo = nullptr;
	node->left = nullptr;
	node->right = nullptr;
	node->reason = MISS_ALL;
	leaves.push_back(node);
	
	return node;
}

bool RenderWithFlatLightSource::newBuildTree(Node* node, Ray& ray, std::vector<Node*>& leaves, int depth, int spectrumComponent, double refractionIndex, int level) const
{
	//Node* node = new Node();
	node->level = level;
	if (node->ray != nullptr)
	{
		delete node->ray;
	}
	node->ray = new Ray(ray); //сохраняем падающий луч
	std::vector<std::unique_ptr<IntersectionInfo>> intersections = std::vector<std::unique_ptr<IntersectionInfo>>();
	std::unique_ptr<IntersectionInfo> hit = findNearestIntersection(ray, intersections, sceneFileInfo);

	//если есть хоть одна работаем с ней иначе считаем что луч попал в габаритный бокс
	if (hit != nullptr)
	{
		node->intersectionInfo = std::move(hit); // сохраняем среды куда входим. в hit тпереь ничего нет.
		intersections.clear();

		//проверяем истоник ли это или нет
		if (node->intersectionInfo->isIntesctionWithLightSource())
		{
			//тут добавляем лист и интенсивность;
			node->reason = INTERSECTION_WITH_LIGHT_SOURCE;
			leaves.push_back(node);
			return true;
		}

		if (depth > 0)
		{
			std::shared_ptr<OpticProperties> op = node->intersectionInfo->getOpticProperties();
			const PointVector3D& normal = node->intersectionInfo->getNormal();
			const PointVector3D& hitIntersectionPoint = node->intersectionInfo->getIntersectionPoints();

			//обрабатываем возможный отраженный луч
			Ray reflected = createReflectedRay(normal, hitIntersectionPoint,
				Ray(hitIntersectionPoint, ray.getRayStart(), false), ray.isInner());

			//Node* left = 
			if (newBuildTree(node->left, reflected, leaves, depth - 1, spectrumComponent, refractionIndex, level + 1)){
				node->left->isLeft = true;
				if (node->intersectionInfo->isTrasasparency())
				{
					identifyRefractionIndex(node->left->refractionIndexFrom, node->left->refractionIndexTo,
						spectrumComponent, node->intersectionInfo->getOpticProperties().get(), ray.isInner());
				}
			}
			

			//обрабатываем возможный преломленный луч
			if (node->intersectionInfo->isTrasasparency())
			{
				double n1, n2;
				identifyRefractionIndex(n1, n2, spectrumComponent, node->intersectionInfo->getOpticProperties().get(), ray.isInner());

				Ray* transmitted = createTransmittedRay(normal, node->intersectionInfo->getIntersectionPoints(),
					ray, n1, n2, !ray.isInner());

				if (transmitted != nullptr)
				{
					bool flag = newBuildTree(node->right, *transmitted, leaves, depth - 1, spectrumComponent, n2, level + 1);
					delete transmitted;
					if (flag)
					{
						//node->right = right;
						node->right->parent = node;
						node->right->isLeft = false;
						node->right->refractionIndexFrom = n1;
						node->right->refractionIndexTo = n2;
					}
				}
				
			}
			return true;

			//return node;
		}
		//если была достигнута предельная глубина трассировки, то запоминаем причину
		node->reason = ACHIEVED_MAX_DEPTH;
		return true;
		
	}

	//все улетело в бесконечность
	//берем 0.4 d65 или рассеяный из файла
	node->intersectionInfo = nullptr;
	node->reason = MISS_ALL;
	leaves.push_back(node);


	return true;
	//return node;
}

PointVector3D RenderWithFlatLightSource::convert(Point point, int width, int height, float sw, float sh) const
{
	double deltaW = sw / width;
	double deltaH = sh / height;

	PointVector3D r = right * (point.getX() * deltaW);
	PointVector3D u = upVector * (point.getY() * -deltaH);

	PointVector3D tmp = start + (r + u);

	return tmp;
}

bool RenderWithFlatLightSource::checkLightSource(const LightSource& lightSource, const PointVector3D& hit, double& transmission, int spectrumComponent) const
{
//	const PointVector3D& lightCoord = lightSource.getLightSourceCoordinates();
//
//	Ray ray = Ray(hit, lightCoord, false);
//
//	IntersectionInfo* intersection;
//
//	for (Shape* shape : sceneFileInfo->getShapes())
//	{
//		intersection = shape->checkRay(ray);
//		if (intersection != nullptr && !intersection->isTrasasparency())
//		{
//			delete intersection;
//			return true;
//		}
//		if (intersection != nullptr)
//		{
//			transmission *= intersection->getTransmission()[spectrumComponent];
//			delete intersection;
//		}
//	}
	return false;
}

Ray RenderWithFlatLightSource::createReflectedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, bool isInner) const
{
	const PointVector3D& L = lRay.getRayDirection();

	double tmp = 2.0 * (normal | L);
	PointVector3D nTmp = normal * tmp;
	PointVector3D reflectedDirection = nTmp - L;
	reflectedDirection.normalizePointVector();

	Ray ray(isInner);
	ray.setRayStart(hit + INTERSECTION_DELTA * reflectedDirection);
	ray.setRayDirection(reflectedDirection);

	return ray;
}

Ray* RenderWithFlatLightSource::createTransmittedRay(const PointVector3D& normal, const PointVector3D& hit, const Ray& lRay, double n1, double n2, bool isInner) const
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
	transmittedDirection.normalizePointVector();
	ray->setRayStart(hit + INTERSECTION_DELTA * transmittedDirection);
	ray->setRayDirection(transmittedDirection);
	return ray;
}

double RenderWithFlatLightSource::calculateMaxIntensity() const
{
	double max = intensities->getIntensity(1, 2).getMaxIntensity();

	for (int i = 0; i < intensities->getRows(); i++){

		for (int j = 0; j < intensities->getColumns(); j++) {
			if (max < intensities->getIntensity(i, j).getMaxIntensity()) {
				max = intensities->getIntensity(i, j).getMaxIntensity();
			}
		}
	}

	return max;
}