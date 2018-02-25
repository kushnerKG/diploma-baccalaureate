#include "SceneFileInfo.h"
#include "RgbColor.h"

SceneFileInfo::SceneFileInfo(Color* ambientColor) : ambientLightColor(ambientColor)
{
}


SceneFileInfo::~SceneFileInfo()
{
	for (Shape* s : shapes)
	{
		delete s;
	}
}

void SceneFileInfo::addLightSource(LightSource& lightSource)
{
	lightSources.push_back(lightSource);
}

void SceneFileInfo::addShape(Shape&& shape)
{
	shapes.push_back(shape.clone());
}

void SceneFileInfo::addShape(Shape& shape)
{
	shapes.push_back(shape.clone());
}

void SceneFileInfo::addLightSource(LightSource&& lightSource)
{
	lightSources.push_back(lightSource);
}

int SceneFileInfo::getLightSourceNumber() const
{
	return lightSources.size();
}

Color& SceneFileInfo::getAmbientColor() const
{
	return *ambientLightColor;
}

const std::vector<LightSource>& SceneFileInfo::getLightsources() const
{
	return lightSources;
}

const std::vector<Shape*>& SceneFileInfo::getShapes() const
{
	return shapes;
}