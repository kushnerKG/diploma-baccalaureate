#pragma once
#include "LightSource.h"
#include "Box.h"
#include <vector>
#include <ostream>
class SceneFileInfo
{
public:
	SceneFileInfo(Color* ambientColor);
	~SceneFileInfo();

	void addLightSource(LightSource& lightSource);
	void addShape(Shape&& shape);
	
	void addShape(Shape& shape);

	void addLightSource(LightSource&& lightSource);

	int getLightSourceNumber() const;
	Color& getAmbientColor() const;

	const std::vector<LightSource>& getLightsources() const;
	const std::vector<Shape*>& getShapes() const;

	//friend std::ostream& operator<<(std::ostream& os, const SceneFileInfo& obj);

private:
	Color* ambientLightColor;
	std::vector<LightSource> lightSources;

	std::vector<Shape*> shapes;
};

