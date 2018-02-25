#pragma once
#include "Shape.h"
#include "Triangle.h"
#include "Quadrangle.h"

class RgbColor;

enum SUBSTRATE_TYPE
{
	CHESS_BOARD, LABIRINTH, CUSTOM
};

class Substrate :
	public Quadrangle
{
public:
	Substrate(RenderType renderType, SUBSTRATE_TYPE type, PointVector3D vertex1, PointVector3D vertex2, PointVector3D vertex3, PointVector3D vertex4, std::shared_ptr<OpticProperties> opticProperties);
	~Substrate();

	Shape* clone() override;
	std::unique_ptr<IntersectionInfo> checkRay(Ray& ray) override;

private:

	std::shared_ptr<OpticProperties> getOpticalPropertiesByPoint(double x, double y);
	void generateOpticPropertiesForColor(std::vector<RgbColor>& colors);
	

	RenderType renderType;
	SUBSTRATE_TYPE type;
	const PointVector3D vertex1;
	const PointVector3D vertex2;
	const PointVector3D vertex3;
	const PointVector3D vertex4;
	std::shared_ptr<OpticProperties> opticProperties;

	Triangle triangle1;
	Triangle triangle2;

	std::vector<LineSegment> lineSegments;
	std::shared_ptr<std::vector<LineSegment>> transformQuadrangle = nullptr;

	std::vector<std::shared_ptr<OpticProperties>> colorsOpticProprties;

	std::vector<char> textureMap;
};