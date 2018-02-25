#pragma once
#include <string>
#include <memory>
#include <map>

#include "SceneFileInfo.h"
#include "AdvancedOpticProperties.h"

double getSf11RefractionIndex(double lambda);
double getBk7RefaractionIndex(double lambda);
void getCustom(double* indexes, int size, double startRefractionIndexes, double finishRefractionIndexes);

class SceneFileReader
{
public:
	explicit SceneFileReader(std::string path, RenderType renderType);
	~SceneFileReader();

	std::shared_ptr<SceneFileInfo> read() const;
	
private:

	static const std::string SPHERE_IDENTIFIER;
	static const std::string TRANSPARENCE_SPHERE_IDENTIFIER;
	static const std::string BOX_IDENTIFIER;
	static const std::string TRIANGLE_IDENTIFIER;
	static const std::string QUADRANGLE_IDENTIFIER;
	static const std::string LENS_IDENTIFIER;
	static const std::string CYLINDER_IDENTIFIER;
	static const std::string NEW_LENS_IDENTIFIER;
	static const std::string SUBSTRATE_IDENTIFIER;

	RenderType renderType;

	enum ShapeValue {
		SPHERE,
		TRANSPARENCE_SPHERE,
		BOX,
		TRIANGLE,
		QUADRANGLE,
		LENS,
		CYLINDER,
		NEW_LENS,
		SUBSTRATE
	};

	std::string pathToFile;
	std::map<std::string, ShapeValue> mapStringValues;

	void readShapes(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readLightSource(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;

	void readLens(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readDebLens(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readSphere(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readBox(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readTriangle(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readQuadrangle(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readTransparenceSphere(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readCylinder(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readNewLense(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	void readSubstarte(std::ifstream& inputFile, std::shared_ptr<SceneFileInfo>& sceneFileInfo) const;
	
	std::shared_ptr<OpticProperties> readOpticProperties(std::ifstream& inputFile) const;
	std::shared_ptr<OpticProperties> SceneFileReader::readAdvancedOpticProperties(std::ifstream& inputFile) const;
};

