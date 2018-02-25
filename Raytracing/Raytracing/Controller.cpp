#include "Controller.h"

Controller::Controller()
{
	mainWindow = std::make_shared<MainWindow>(this);
	model = std::make_shared<Model>();
	model->subscribe(mainWindow);
	mainWindow->show();
}

Controller::~Controller()
{
}

void Controller::onOpenSceneFile(const std::string& fileName, const std::string pathToRenderFile) const
{
	model->openScene(fileName, pathToRenderFile);
	model->updateImage();
}

void Controller::onSaveImage(QString& fileName) const
{
	model->saveCurrentImage(fileName);
}

void Controller::onSaveRenderSettings(std::string pathToFile) const
{
	model->saveCurrentRenderSettings(pathToFile);
}

void Controller::onOpenRenderFile(std::string& pathToRenderFile, RenderType renderType) const
{
	model->openRenderFile(pathToRenderFile, renderType);
	model->updateImage();
}

void Controller::showCoordinatesSystem() const
{
	qDebug("show coordinates system");
	model->showCoordinateSystem();
	model->updateImage();
}

void Controller::updateWorkSpaceSize(int width, int height) const
{
	model->updateWorkSpaceHeight(height);
	model->updateImage();
}

void Controller::toRender(bool checked) const
{
	if (checked)
	{
		model->setRotated(false);
		model->rgbRenderImage();
	}
	else
	{
		model->setRotated(true);
		model->updateImage();
	}
}

void Controller::toSpectrumRender(bool checked) const
{
	if (checked)
	{
		model->setRotated(false);
		model->spectrumRenderImage();
	}
	else
	{
		model->setRotated(true);
		model->updateImage();
	}
}

void Controller::toPolarizationRender(bool checked) const
{
	if (checked)
	{
		model->setRotated(false);
		model->renderWithPolarizationFlatLight(POLARIZATION);
	}
	else
	{
		model->setRotated(true);
		model->updateImage();
	}
}

void Controller::toSpectrumRenderWithSurfaceLightSource(bool checked) const
{
	if (checked)
	{
		model->setRotated(false);
		model->renderWithPolarizationFlatLight(SPECRUM);
	}
	else
	{
		model->setRotated(true);
		model->updateImage();
	}
}


void Controller::rotateScene(double angleX, double angleY) const
{
	if (model->isRotated())
	{
		model->rotateScene(angleX, angleY);
		model->updateImage();
	}
}
