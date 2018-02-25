#include "SceneFileReader.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "RgbColor.h"
#include "ParserUtils.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Quadrangle.h"
#include "Lens.h"
#include "Cylinder.h"
#include "NewLens.h"
#include "Transmission.h"
#include "RenderSetting.h"
#include "SpectrumColor.h"

#include "QDebug"
#include "Substrate.h"

using std::string;
using std::cout;
using std::vector;
using std::stoi;
using std::stod;
using std::shared_ptr;

using RayTracingUtils::ParserUtils;

const string SceneFileReader::SPHERE_IDENTIFIER = "SPHERE";
const string SceneFileReader::TRANSPARENCE_SPHERE_IDENTIFIER = "TRANSPARENCE_SPHERE";
const string SceneFileReader::BOX_IDENTIFIER = "BOX";
const string SceneFileReader::TRIANGLE_IDENTIFIER = "TRIANGLE";
const string SceneFileReader::QUADRANGLE_IDENTIFIER = "QUADRANGLE";
const string SceneFileReader::LENS_IDENTIFIER = "LENS";
const string SceneFileReader::CYLINDER_IDENTIFIER = "CYLINDER";
const string SceneFileReader::NEW_LENS_IDENTIFIER = "NEW_LENS";
const string SceneFileReader::SUBSTRATE_IDENTIFIER = "SUBSTRATE";

double getSf11RefractionIndex(double lambda)//index Abbe 25.68
{
	double n = (1.73759695 * pow(lambda, 2)) / (pow(lambda, 2) - 0.013188707)
		+ (0.313747346 * pow(lambda, 2) / (pow(lambda, 2) - 0.0623068142) +
		(1.89878101 * pow(lambda, 2)) / (pow(lambda, 2) - 155.23629));

	return sqrt(n + 1);
}

double getBk7RefaractionIndex(double lambda)//index Abbe 64.7
{
	double n = (1.03961212 * pow(lambda, 2)) / (pow(lambda, 2) - 0.00600069867)
		+ (0.231792344 * pow(lambda, 2) / (pow(lambda, 2) - 0.0200179144) +
		(1.01046945 * pow(lambda, 2)) / (pow(lambda, 2) - 103.560653));

	return sqrt(n + 1);
}

void getCustom(double* indexes, int size, double startRefractionIndexes, double finishRefractionIndexes)
{
	double delta = (finishRefractionIndexes - startRefractionIndexes) / static_cast<double>(size - 1);

	for (int i = 0; i < size; i++)
	{
		indexes[i] = startRefractionIndexes + i * delta;
	}
}

SceneFileReader::SceneFileReader(std::string path, RenderType renderType)
{
	this->pathToFile = path;
	this->renderType = renderType;
	mapStringValues[SPHERE_IDENTIFIER] = SPHERE;
	mapStringValues[TRANSPARENCE_SPHERE_IDENTIFIER] = TRANSPARENCE_SPHERE;
	mapStringValues[BOX_IDENTIFIER] = BOX;
	mapStringValues[TRIANGLE_IDENTIFIER] = TRIANGLE;
	mapStringValues[QUADRANGLE_IDENTIFIER] = QUADRANGLE;
	mapStringValues[LENS_IDENTIFIER] = LENS;
	mapStringValues[CYLINDER_IDENTIFIER] = CYLINDER;
	mapStringValues[NEW_LENS_IDENTIFIER] = NEW_LENS;
	mapStringValues[SUBSTRATE_IDENTIFIER] = SUBSTRATE;
}

SceneFileReader::~SceneFileReader()
{
}

shared_ptr<SceneFileInfo> SceneFileReader::read() const
{	
	std::ifstream inputFile;
	inputFile.open(pathToFile);

	if (!inputFile.is_open())
	{
		return nullptr;
	}

	string fileStr;

	getline(inputFile, fileStr);
	
	ParserUtils::removeComment(fileStr);

	vector<string> ambient = ParserUtils::splitBySpace(fileStr);
	shared_ptr<SceneFileInfo> sceneFileInfo;
	if (renderType == RGB_RENDER){
		CURRENT_RENDER_MODE = RGB_RENDER;
		sceneFileInfo = std::make_shared<SceneFileInfo>(new RgbColor(stoi(ambient.at(0)), stoi(ambient.at(1)), stoi(ambient.at(2))));

	}
	else
	{
		CURRENT_RENDER_MODE = SPECTRUM_RENDER;
		float spectrum[SPECTRUM_SIZE];
		double red = stoi(ambient.at(0)) / 255.0;
		double green = stoi(ambient.at(1)) / 255.0;
		double blue = stoi(ambient.at(2)) / 255.0;
		convert_rgb_to_spectrum(sourceSpectrFileName, red, green, blue, spectrum, SPECTRUM_SIZE);
		Color* ambientColor = new SpectrumColor(spectrum);
		sceneFileInfo = std::make_shared<SceneFileInfo>(ambientColor);
	}

	readLightSource(inputFile, sceneFileInfo);
	readShapes(inputFile, sceneFileInfo);

	inputFile.close();

	return sceneFileInfo;
}

void SceneFileReader::readLightSource(std::ifstream& inputFile, shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	string fileStr;
	getline(inputFile, fileStr);
	int lightSourceNumber = stoi(fileStr);

	for (int i = 0; i < lightSourceNumber; i++)
	{
		getline(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		vector<string> lightSource = ParserUtils::splitBySpace(fileStr);
		PointVector3D LightSourceCoordinates(stod(lightSource.at(0)), stod(lightSource.at(1)), stod(lightSource.at(2)));
		if (renderType == RGB_RENDER){
			
			RgbColor color(stoi(lightSource.at(3)) / 255.0, stoi(lightSource.at(4)) / 255.0, stoi(lightSource.at(5)) / 255.0);
			LightSource s(LightSourceCoordinates, &color);
			sceneFileInfo->addLightSource(s);
		}
		else
		{
			float spectrum[SPECTRUM_SIZE];

			crt_get_d65(sourceSpectrFileName, spectrum);
			SpectrumColor color(spectrum);
			LightSource s(LightSourceCoordinates, &color);
			sceneFileInfo->addLightSource(s);
		}
	}
}

void SceneFileReader::readShapes(std::ifstream& inputFile, shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	string fileStr;

	while (ParserUtils::readNextNotEmptyString(inputFile, fileStr))
	{
		switch (mapStringValues.at(fileStr))
		{
		case SPHERE:
			readSphere(inputFile, sceneFileInfo);
			break;
		case TRANSPARENCE_SPHERE:
			readTransparenceSphere(inputFile, sceneFileInfo);
			break;
		case LENS:
			readLens(inputFile, sceneFileInfo);
			break;
		case BOX:
			readBox(inputFile, sceneFileInfo);
			break;
		case TRIANGLE:
			readTriangle(inputFile, sceneFileInfo);
			break;
		case QUADRANGLE:
			readQuadrangle(inputFile, sceneFileInfo);
			break;
		case CYLINDER:
			readCylinder(inputFile, sceneFileInfo);
			break;
		case NEW_LENS:
			readNewLense(inputFile, sceneFileInfo);
			break;
		case SUBSTRATE:
			readSubstarte(inputFile, sceneFileInfo);
			break;
		default: break;
		}
	}
}

void SceneFileReader::readSubstarte(std::ifstream& inputFile, shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	string fileStr;
	
	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	SUBSTRATE_TYPE substrateType;
	if (fileStr == "CHESS_BOARD")
	{
		substrateType = CHESS_BOARD;
	} else if(fileStr == "LABIRINTH")
	{
		substrateType = LABIRINTH;
	} else
	{
		substrateType = CUSTOM;
	}

	PointVector3D vertex1 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex2 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex3 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex4 = ParserUtils::readVertex(inputFile);
	if (renderType == RGB_RENDER){
		sceneFileInfo->addShape(Substrate(RGB_RENDER, substrateType, vertex1, vertex2, vertex3, vertex4, readOpticProperties(inputFile)));
	}
	else
	{
		getline(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		vector<string> subStrings = ParserUtils::splitBySpace(fileStr);

		double diffuseRed = atof(subStrings.at(0).c_str()), diffuseGreen = atof(subStrings.at(1).c_str()),
			diffuseBlue = atof(subStrings.at(2).c_str()), specularRed = atof(subStrings.at(3).c_str()),
			specularGreen = atof(subStrings.at(5).c_str()), specularBlue = atof(subStrings.at(5).c_str());

		float spectrum[SPECTRUM_SIZE];
		for (float& val : spectrum)
		{
			val = 0.0;
		}
		convert_rgb_to_spectrum(sourceSpectrFileName, diffuseRed, diffuseGreen, diffuseBlue, spectrum, SPECTRUM_SIZE);
		std::unique_ptr<Color> diffuse = std::unique_ptr<Color>(std::make_unique<SpectrumColor>(spectrum));

		for (float& val : spectrum)
		{
			val = 0.0;
		}

		convert_rgb_to_spectrum(sourceSpectrFileName, specularRed, specularGreen, specularBlue, spectrum, SPECTRUM_SIZE);
		std::unique_ptr<Color> specular = std::make_unique<SpectrumColor>(spectrum);

		shared_ptr<OpticProperties> opticProperties = std::make_shared<OpticProperties>(
			std::move(diffuse), std::move(specular), atof(subStrings.at(6).c_str()));
		
		sceneFileInfo->addShape(Substrate(SPECTRUM_RENDER, substrateType, vertex1, vertex2, vertex3, vertex4, opticProperties));
	}
}

void SceneFileReader::readNewLense(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	string fileStr;
	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double leftRadius = atof(fileStr.c_str());

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
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

	if (renderType == RGB_RENDER){
		shared_ptr<OpticProperties> opticProperties = readAdvancedOpticProperties(inputFile);
		shared_ptr<OpticProperties> cylinderOpticProperties = readAdvancedOpticProperties(inputFile);
		sceneFileInfo->addShape(NewLens(leftRadius, rightRadius, lensRadius, cylinderHeight,
			directionStart, directionFinsih, lensCenter, opticProperties, cylinderOpticProperties));
	}
	else
	{
		//surface
		getline(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		vector<string> subStrings = ParserUtils::splitBySpace(fileStr);
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
		//cylinder
		getline(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		subStrings = ParserUtils::splitBySpace(fileStr);
		
		shared_ptr<OpticProperties> cylinderOpticProperties = std::make_shared<AdvancedOpticProperties>(
			std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(0).c_str()))),
			std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(1).c_str()))), 
			atof(subStrings.at(2).c_str()),
			std::move(std::make_unique<SpectrumColor>(atof(subStrings.at(3).c_str()))),
			refractionIndexes, SPECTRUM_RENDER);

		sceneFileInfo->addShape(NewLens(leftRadius, rightRadius, lensRadius, cylinderHeight,
			directionStart, directionFinsih, lensCenter, SurfaceOpticProperties, cylinderOpticProperties));
	}
}

void SceneFileReader::readCylinder(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	string fileStr;
	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius = atof(fileStr.c_str());

	PointVector3D p1 = ParserUtils::readVertex(inputFile);
	PointVector3D p2 = ParserUtils::readVertex(inputFile);

	shared_ptr<OpticProperties> opticProperties = readAdvancedOpticProperties(inputFile);
	sceneFileInfo->addShape(Cylinder(p1, p2, radius, opticProperties, true));
}

void SceneFileReader::readDebLens(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D sphereCenter1 = ParserUtils::readVertex(inputFile);
	string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius1 = atof(fileStr.c_str());


	PointVector3D sphereCenter2 = ParserUtils::readVertex(inputFile);

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius2 = atof(fileStr.c_str());

	shared_ptr<OpticProperties> opticProperties = readAdvancedOpticProperties(inputFile);
	sceneFileInfo->addShape(Lens(sphereCenter1, radius1, sphereCenter2, radius2, opticProperties));
}

void SceneFileReader::readLens(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D sphereCenter1 = ParserUtils::readVertex(inputFile);
	string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius1 = atof(fileStr.c_str());

	PointVector3D sphereCenter2 = ParserUtils::readVertex(inputFile);

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius2 = atof(fileStr.c_str());

	shared_ptr<OpticProperties> opticProperties = readAdvancedOpticProperties(inputFile);
	sceneFileInfo->addShape(Lens(sphereCenter1, radius1, sphereCenter2, radius2, opticProperties));
}

void SceneFileReader::readTransparenceSphere(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D sphereCenter = ParserUtils::readVertex(inputFile);
	string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius = atof(fileStr.c_str());
	sceneFileInfo->addShape(Sphere(sphereCenter, radius, readAdvancedOpticProperties(inputFile), true));
}

void SceneFileReader::readQuadrangle(std::ifstream& inputFile, shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D vertex1 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex2 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex3 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex4 = ParserUtils::readVertex(inputFile);
	if (renderType == RGB_RENDER){
		sceneFileInfo->addShape(Quadrangle(vertex1, vertex2, vertex3, vertex4, readOpticProperties(inputFile)));
	}
	else
	{
		string fileStr;

		getline(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		vector<string> subStrings = ParserUtils::splitBySpace(fileStr);
		
		double diffuseRed = atof(subStrings.at(0).c_str()), diffuseGreen = atof(subStrings.at(1).c_str()),
			diffuseBlue = atof(subStrings.at(2).c_str()), specularRed = atof(subStrings.at(3).c_str()),
			specularGreen = atof(subStrings.at(5).c_str()), specularBlue = atof(subStrings.at(5).c_str());

		float spectrum[SPECTRUM_SIZE];
		for (float& val : spectrum)
		{
			val = 0.0;
		}
		convert_rgb_to_spectrum(sourceSpectrFileName, diffuseRed, diffuseGreen, diffuseBlue, spectrum, SPECTRUM_SIZE);
		std::unique_ptr<Color> diffuse = std::unique_ptr<Color>(std::make_unique<SpectrumColor>(spectrum));

		for (float& val : spectrum)
		{
			val = 0.0;
		}

		convert_rgb_to_spectrum(sourceSpectrFileName, specularRed, specularGreen, specularBlue, spectrum, SPECTRUM_SIZE);
		std::unique_ptr<Color> specular = std::make_unique<SpectrumColor>(spectrum);

		shared_ptr<OpticProperties> opticProperties = std::make_shared<OpticProperties>(
			std::move(diffuse), std::move(specular), atof(subStrings.at(6).c_str()));
		
		sceneFileInfo->addShape(Quadrangle(vertex1, vertex2, vertex3, vertex4, opticProperties));
	}
}

void SceneFileReader::readSphere(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D sphereCenter = ParserUtils::readVertex(inputFile);
	string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	double radius = atof(fileStr.c_str());

	sceneFileInfo->addShape(Sphere(sphereCenter, radius, readOpticProperties(inputFile), false));
}

void SceneFileReader::readBox(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{
	PointVector3D minPoint = ParserUtils::readVertex(inputFile);
	PointVector3D maxPoint = ParserUtils::readVertex(inputFile);
	Box* box = new Box(minPoint, maxPoint, readOpticProperties(inputFile));
	sceneFileInfo->addShape(*box);
	delete box;
}

void SceneFileReader::readTriangle(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const
{	
	PointVector3D vertex1 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex2 = ParserUtils::readVertex(inputFile);
	PointVector3D vertex3 = ParserUtils::readVertex(inputFile);
	Triangle* triangle = new Triangle(vertex1, vertex2, vertex3, readOpticProperties(inputFile));
	sceneFileInfo->addShape(*triangle);
	delete triangle;
}

shared_ptr<OpticProperties> SceneFileReader::readOpticProperties(std::ifstream& inputFile) const
{
	string fileStr;
	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	vector<string> subStrings = ParserUtils::splitBySpace(fileStr);

	std::unique_ptr<Color> diffuse = std::make_unique<RgbColor>(atof(subStrings.at(0).c_str()), atof(subStrings.at(1).c_str()), atof(subStrings.at(2).c_str()));
	std::unique_ptr<Color> specular = std::make_unique<RgbColor>(atof(subStrings.at(3).c_str()), atof(subStrings.at(4).c_str()), atof(subStrings.at(5).c_str()));

	return std::make_shared<OpticProperties>(std::move(diffuse), std::move(specular), atof(subStrings.at(6).c_str()));
}

shared_ptr<OpticProperties> SceneFileReader::readAdvancedOpticProperties(std::ifstream& inputFile) const
{
	string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	vector<string> subStrings = ParserUtils::splitBySpace(fileStr);

	return std::make_shared<AdvancedOpticProperties>(
		std::move(std::make_unique<RgbColor>(atof(subStrings.at(0).c_str()), atof(subStrings.at(1).c_str()), atof(subStrings.at(2).c_str()))),
		std::move(std::make_unique<RgbColor>(atof(subStrings.at(3).c_str()), atof(subStrings.at(4).c_str()), atof(subStrings.at(5).c_str()))),
		atof(subStrings.at(6).c_str()),
		std::move(std::make_unique<RgbColor>(atof(subStrings.at(7).c_str()), atof(subStrings.at(8).c_str()), atof(subStrings.at(9).c_str()))), 
		atof(subStrings.at(10).c_str()), RGB_RENDER);
}