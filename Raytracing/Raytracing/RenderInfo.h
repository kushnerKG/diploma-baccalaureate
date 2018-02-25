#pragma once
#include "PointVector3D.h"
#include <ostream>
#include "RgbColor.h"
class RenderInfo
{
public:

	RenderInfo::RenderInfo(Color* backgroundColor, double gamma, int depth, int quality, PointVector3D& eyePoint,
		PointVector3D& viewPoint, PointVector3D& upVector, double zn, double zf, double sw, double sh);

	~RenderInfo();

	Color& getBackgroundColor() const;
	double getGamma() const;
	int getDepth() const;
	int getQuality() const;
	const PointVector3D& getEyePoint() const;
	const PointVector3D& getViewPoint() const;
	const PointVector3D& getUpVector() const;
	double getZn() const;
	double getZf() const;
	double getSw() const;
	double getSh() const;

private:

	Color* backgroundColor;
	const double gamma;
	const int depth;
	const int quality;//rough Ц грубое(0), normal Ц среднее(1), fine Ц высокое(2)
	PointVector3D eyePoint;
	PointVector3D viewPoint;
	PointVector3D upVector;
	const double zn;
	const double zf;
	const double sw;
	const double sh;
};

