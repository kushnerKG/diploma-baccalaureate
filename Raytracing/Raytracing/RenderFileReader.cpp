#include "RenderFileReader.h"
#include "ParserUtils.h"
#include <fstream>
#include <string>
#include "RgbColor.h"
#include <qstring.h>
#include "RenderSetting.h"
#include "crt.h"
#include "SpectrumColor.h"
#include "QDebug"
using std::stoi;
using std::vector;
using std::string;
using std::cout;
using std::endl;

using RayTracingUtils::ParserUtils;

RenderFileReader::RenderFileReader(std::string path, RenderType renderType): pathToFile(path), renderType(renderType)
{
}

RenderFileReader::RenderFileReader(QString path, RenderType renderType) : pathToFile(path.toStdString()), renderType(renderType)
{
}


RenderFileReader::~RenderFileReader()
{
}

std::shared_ptr<RenderInfo> RenderFileReader::read() const
{
	std::ifstream inputFile;
	inputFile.open(pathToFile);
	
	if (!inputFile.is_open())
	{
		return nullptr;
	}

	std::string fileStr;

	getline(inputFile, fileStr);
	ParserUtils::removeComment(fileStr);
	vector<string> subStrings = ParserUtils::splitBySpace(fileStr);
	Color* backgroundColor;
	if (renderType == RGB_RENDER){
		backgroundColor = new RgbColor(stoi(subStrings.at(0)) / 255.0, stoi(subStrings.at(1)) / 255.0, stoi(subStrings.at(2)) / 255.0);
	}
	else
	{
		double red = stoi(subStrings.at(0)) / 255.0;
		double green = stoi(subStrings.at(1)) / 255.0;
		double blue = stoi(subStrings.at(2)) / 255.0;

		float spectrum[SPECTRUM_SIZE];
//		for (int i = 0; i < SPECTRUM_SIZE; i++)
//		{
//			ambientSpectrum[i] = 0.0;
//		}
		convert_rgb_to_spectrum(sourceSpectrFileName, red, green, blue, spectrum, SPECTRUM_SIZE);
		backgroundColor  = new SpectrumColor(spectrum);
		//*backgroundColor /= (1/0.1);

		double r, g, b;

		double spect[SPECTRUM_SIZE];
		for (int i = 0; i < SPECTRUM_SIZE; i++)
		{
			spect[i] = spectrum[i];
		}
	}

	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	double gamma = atof(fileStr.c_str());

	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	int depth = stoi(fileStr);

	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	int quality = 0;
	if (fileStr == "ROUGH")
	{
		quality = 0;
	}
	else if (fileStr == "NORMAL")
	{
		quality = 1;
	}
	else if (fileStr == "FINE")
	{
		quality = 2;
	}

	PointVector3D eyePoint = ParserUtils::readVertex(inputFile);
	PointVector3D viewPoint = ParserUtils::readVertex(inputFile);
	PointVector3D upVector = ParserUtils::readVertex(inputFile);

	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	subStrings = ParserUtils::splitBySpace(fileStr);

	double zn = atof(subStrings.at(0).c_str());
	double zf = atof(subStrings.at(1).c_str());

	ParserUtils::readNextNotEmptyString(inputFile, fileStr);
	subStrings = ParserUtils::splitBySpace(fileStr);

	double sw = atof(subStrings.at(0).c_str());
	double sh = atof(subStrings.at(1).c_str());
	

	std::shared_ptr<RenderInfo> renderInfo(new RenderInfo(backgroundColor, gamma, depth, quality, eyePoint, viewPoint, upVector, zn, zf, sw, sh));

	inputFile.close();

	return renderInfo;
}