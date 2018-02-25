#include <QDebug>
#include <fstream>
#include <filesystem>
#include <thread>
#include <QBuffer>

#include "Model.h"
#include "SceneFileInfo.h"
#include "SceneFileReader.h"
#include "RenderFileReader.h"
#include "Render.h"
#include <QFileDialog>
#include "TaskQueue.h"
#include "RenderWithFlatLightSource.h"
#include "SpectrumRender.h"
#include <QDebug>

Model::Model(): widthWorkspace(0), heightWorkSpace(0)
{
}

Model::~Model()
{
}

void Model::subscribe(std::shared_ptr<IModelListener> listener)
{
	subscribers.push_back(listener);
}

void Model::openScene(std::string pathToSceneFile, std::string pathToRenderFile)
{
	std::string subPath = pathToSceneFile.substr(0, pathToSceneFile.find_last_of('/'));
	std::string renderFileName = pathToSceneFile.substr(pathToSceneFile.find_last_of('/') + 1, pathToSceneFile.size());
	std::string format = renderFileName.substr(renderFileName.find('.') + 1, renderFileName.size());
	renderFileName = renderFileName.substr(0, renderFileName.find_first_of('.'));
	
	if (pathToRenderFile.size() == 0){
		pathToRenderFile = subPath + "/" + renderFileName + ".render";
	}

	if (format == "scene"){
		SceneFileReader sceneFileReader(pathToSceneFile, RGB_RENDER);
		this->sceneFileInfo = sceneFileReader.read();
		
		if (this->sceneFileInfo == nullptr)
		{
			return;
		}

		scene.setSceneFileInfo(this->sceneFileInfo);

		std::ifstream f(pathToRenderFile);

		if (f.good())
		{
			openRenderFile(pathToRenderFile, RGB_RENDER);

			if (this->renderInfo == nullptr)
			{
				return;
			}

		}
		else
		{
			//тут нужно все в init выставлять
			//init();
		}
		this->isValideImage = false;
	}
	else if (format == "sceneSpectrum")
	{
		SceneFileReader sceneFileReader(pathToSceneFile, SPECTRUM_RENDER);
		this->sceneFileInfo = sceneFileReader.read();

		if (this->sceneFileInfo == nullptr)
		{
			return;
		}
		
		scene.setSceneFileInfo(this->sceneFileInfo);
		//std::string pathToRenderFile = subPath + "/" + renderFileName + ".render";
		std::ifstream f(pathToRenderFile);

		if (f.good())
		{
			openRenderFile(pathToRenderFile, SPECTRUM_RENDER);
			
			if (this->renderInfo == nullptr)
			{
				return;
			}
		}
		else
		{
			//тут нужно все в init выставлять
			//init();
		}
		this->isValideImage = false;
	}
}

void Model::openRenderFile(std::string fileName, RenderType renderType)
{
	RenderFileReader renderFileReader(fileName, renderType);
	this->renderInfo = renderFileReader.read();
	scene.setRenderFileInfo(this->renderInfo);
	this->isValideImage = false;
}

void Model::saveCurrentImage(QString& fileName) const
{
	QByteArray ba;
	QString format = fileName.split(".").at(fileName.split(".").size()-1);
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image->save(&buffer, format.toLatin1().data());
	QFile file(fileName);
	file.open(QIODevice::WriteOnly);
	file.write(buffer.buffer());
	file.close();
}
double round(double x) {
	return ((x - floor(x)) >= 0.5) ? ceil(x) : floor(x);
}
void Model::saveCurrentRenderSettings(std::string pathToRenderFile)
{
	std::ofstream fout;
	fout.open(pathToRenderFile);
	int r, g, b;
	if (CURRENT_RENDER_MODE == RGB_RENDER)
	{
		r = round(renderInfo->getBackgroundColor()[0] * 255);
		g = round(renderInfo->getBackgroundColor()[1] * 255);
		b = round(renderInfo->getBackgroundColor()[2] * 255);
	}
	else
	{	
		int size = renderInfo->getBackgroundColor().getSize();
		double* spectr = new double[renderInfo->getBackgroundColor().getSize()];
		for (int i = 0; i < size; i++)
		{
			spectr[i] = renderInfo->getBackgroundColor()[i];
		}
		double red, green, blue;
		convert_spectrum_to_rgb(std::string(sourceSpectrFileName).c_str(), spectr, SPECTRUM_SIZE, &red, &green, &blue);
		r = round(red * 255);
		g = round(green * 255);
		b = round(blue * 255);
		
		delete[] spectr;
	}
	
	int quality = renderInfo->getQuality();
	Camera camera = scene.getCamera();
	Matrix<double> matrix = scene.getRotateMatrix().transpose();

	PointVector3D eye = matrix * camera.getEyePoint();
	PointVector3D view = matrix * camera.getViewPoint();
	PointVector3D up = matrix * camera.getUpVector();

	fout << r << " " << g << " " << b << "\n";
	fout << renderInfo->getGamma() << "\n";
	fout << renderInfo->getDepth() << "\n";

	if (quality == 0)
	{
		fout << "ROUGH" << "\n";
	}
	else if (quality == 1)
	{
		fout << "NORMAL" << "\n";
	}
	else
	{
		fout << "FINE" << "\n";
	}

	fout << eye.getX() << " " << eye.getY() << " " << eye.getZ() << "\n";
	fout << view.getX() << " " << view.getY() << " " << view.getZ() << "\n";
	fout << up.getX() << " " << up.getY() << " " << up.getZ() << "\n";

	fout << renderInfo->getZn() << " " << renderInfo->getZf() << "\n";
	fout << renderInfo->getSw() << " " << renderInfo->getSh() << "\n";

	fout.close();
}

void Model::updateWorkSpaceHeight(int height)
{
	this->heightWorkSpace = height;
	this->isValideImage = false;
	this->isValideSize = false;
}

void Model::updateImage()
{
	if (!isValideImage && sceneFileInfo.get()!=nullptr && renderInfo.get() != nullptr)
	{
		if (!isValideSize)
		{
			double sw = renderInfo->getSw();
			double sh = renderInfo->getSh();
			int width = ((sw / sh) * this->heightWorkSpace);
			image = std::make_shared<QImage>(width, heightWorkSpace, QImage::Format_ARGB32_Premultiplied);
		}
		scene.rotateScene(0, 0);
		scene.drawScene(image);
		notifyListeners();
		isValideImage = true;
	}
	else
	{
		//просто посылаем текущее
	}
	
}

void Model::rgbRenderImage()
{
	this->isValideImage = false;
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();
	int width = ((sw / sh) * this->heightWorkSpace);
	image = std::make_shared<QImage>(width, heightWorkSpace, QImage::Format_ARGB32_Premultiplied);
	std::shared_ptr<TaskQueue> taskQueue(new TaskQueue(image->size().height(), std::thread::hardware_concurrency()));
	std::thread render = std::thread(Render(sceneFileInfo, renderInfo, scene.getCamera(), scene.getRotateMatrix(), *this), image, *this, taskQueue);
	render.detach();
}

void Model::spectrumRenderImage()
{
	this->isValideImage = false;
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();
	int width = ((sw / sh) * this->heightWorkSpace);
	image = std::make_shared<QImage>(width, heightWorkSpace, QImage::Format_ARGB32_Premultiplied);
	std::shared_ptr<TaskQueue> taskQueue(new TaskQueue(image->size().height(), std::thread::hardware_concurrency()));
	std::thread render = std::thread(SpectrumRender(sceneFileInfo, renderInfo, scene.getCamera(), scene.getRotateMatrix(), *this), 
		image, *this, taskQueue);
	render.detach();
}

void Model::renderWithPolarizationFlatLight(renderTypeWithFlatLight type)
{
	this->isValideImage = false;
	double sw = renderInfo->getSw();
	double sh = renderInfo->getSh();
	int width = ((sw / sh) * this->heightWorkSpace);
	image = std::make_shared<QImage>(width, heightWorkSpace, QImage::Format_ARGB32_Premultiplied);
	std::shared_ptr<TaskQueue> taskQueue(new TaskQueue(image->size().height(), std::thread::hardware_concurrency()));
	if (type == POLARIZATION){
		std::thread render = std::thread(RenderWithFlatLightSource(sceneFileInfo, renderInfo, scene.getCamera(), scene.getRotateMatrix(), *this),
			image, *this, taskQueue, POLARIZATION);
		render.detach();
	}
	else
	{
		std::thread render = std::thread(RenderWithFlatLightSource(sceneFileInfo, renderInfo, scene.getCamera(), scene.getRotateMatrix(), *this),
			image, *this, taskQueue, SPECRUM);
		render.detach();
	}
}

void Model::rotateScene(double angleX, double angleY)
{
	this->isValideImage = false;
	scene.rotateScene(angleX, angleY);
}

void Model::showCoordinateSystem()
{
	scene.drawCoordinatesSystem();
	this->isValideImage = false;
}

void Model::notifyListeners()
{
	for (std::shared_ptr<IModelListener> listener : subscribers)
	{
		listener->handleUpdateImage(image);
	}
}

void Model::notifyListenerFromQtThread()
{
	for (std::shared_ptr<IModelListener> listener : subscribers)
	{
		QObject* object = dynamic_cast<QObject*>(listener.get());
		
		if (object)
		{
			QMetaObject::invokeMethod(object, "slotRenderImageFromAnotherThread", Qt::QueuedConnection, Q_ARG(QImage, *image.get()));
		}
		else
		{
			qDebug("sdv");
		}
	}
}

void Model::notifyListenerAboutStatusBar(double value)
{
	for (std::shared_ptr<IModelListener> listener : subscribers)
	{
		QObject* object = dynamic_cast<QObject*>(listener.get());

		if (object)
		{
			QMetaObject::invokeMethod(object, "slotUpdateStatusBar", Qt::QueuedConnection, Q_ARG(QString, QString::number(100 * value)));
		}
	}
}

void Model::setRotated(bool rotated)
{
	this->rotated = rotated;
}

bool Model::isRotated() const
{
	return this->rotated;
}
