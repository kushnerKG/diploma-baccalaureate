#pragma once
#include <list>
#include "IModelObservable.h"
#include "SceneFileInfo.h"
#include "Scene.h"

enum renderTypeWithFlatLight
{
	SPECRUM,
	POLARIZATION
};
class Model : public IModelObservable
{
public:
	Model();
	~Model();
	
	void subscribe(std::shared_ptr<IModelListener> listener) override;

	void openScene(std::string pathToSceneFile, std::string pathToRenderFile = "");
	void openRenderFile(std::string fileName, RenderType renderType);

	void saveCurrentImage(QString& fileName) const;
	void saveCurrentRenderSettings(std::string pathToRenderFile);

	void updateWorkSpaceHeight(int height);
	void updateImage();

	void rgbRenderImage();
	void spectrumRenderImage();
	void renderWithPolarizationFlatLight(renderTypeWithFlatLight type);

	void rotateScene(double angleX, double angleY);
	void showCoordinateSystem();
	void notifyListeners();

	void notifyListenerFromQtThread();
	void notifyListenerAboutStatusBar(double value);

	void setRotated(bool rotated);
	bool isRotated() const;
private:

	bool isValideImage = false;
	bool isValideSize = false;
	bool rotated = true;

	std::shared_ptr<SceneFileInfo> sceneFileInfo;
	std::shared_ptr<RenderInfo> renderInfo;
	Scene scene;

	std::shared_ptr<QImage> image;

	int widthWorkspace;
	int heightWorkSpace;

	std::list<std::shared_ptr<IModelListener>> subscribers;

	int sw = 5;//stub
	int sh = 5;//stub
};

