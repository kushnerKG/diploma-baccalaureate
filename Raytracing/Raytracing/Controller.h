#pragma once

#include <memory>
#include "Model.h"
#include "MainWindow.h"

class Controller
{
public:
	Controller();
	~Controller();

	void onOpenSceneFile(const std::string& pathToSceneFile, const std::string pathToRenderFile = "") const;
	void onSaveImage(QString& fileName) const;
	void onSaveRenderSettings(std::string pathToFile) const;
	void onOpenRenderFile(std::string& pathToRenderFile, RenderType renderType) const;
	void showCoordinatesSystem() const;
	void updateWorkSpaceSize(int width, int height) const;
	void toRender(bool checked) const;
	void toSpectrumRender(bool checked) const;
	void toPolarizationRender(bool checked) const;
	void toSpectrumRenderWithSurfaceLightSource(bool checked) const;

	void rotateScene(double angleX, double angleY) const;

private:

	std::shared_ptr<MainWindow> mainWindow;
	std::shared_ptr<Model> model;
};
