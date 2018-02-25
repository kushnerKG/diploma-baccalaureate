#include "RenderInfo.h"


RenderInfo::RenderInfo(Color* backgroundColor, double gamma, int depth, int quality, PointVector3D& eyePoint,
	PointVector3D& viewPoint, PointVector3D& upVector, double zn, double zf, double sw, double sh):

	backgroundColor(backgroundColor), gamma(gamma), depth(depth), quality(quality), eyePoint(eyePoint), 
	viewPoint(viewPoint), upVector(upVector), zn(zn), zf(zf), sw(sw), sh(sh)
{

}

RenderInfo::~RenderInfo()
{
	delete backgroundColor;
}

Color& RenderInfo::getBackgroundColor() const
{
	return *backgroundColor;
}

double RenderInfo::getGamma() const
{
	return gamma;
}

int RenderInfo::getDepth() const
{
	return depth;
}

int RenderInfo::getQuality() const
{
	return quality;
}

const PointVector3D& RenderInfo::getEyePoint() const
{
	return eyePoint;
}

const PointVector3D& RenderInfo::getViewPoint() const
{
	return viewPoint;
}

const PointVector3D& RenderInfo::getUpVector() const
{
	return upVector;
}

double RenderInfo::getZn() const
{
	return zn;
}

double RenderInfo::getZf() const
{
	return zf;
}

double RenderInfo::getSw() const
{
	return sw;
}

double RenderInfo::getSh() const
{
	return sh;
}
