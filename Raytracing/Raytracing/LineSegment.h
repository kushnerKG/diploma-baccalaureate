#pragma once
#include "PointVector3D.h"

class LineSegment
{
public:
	LineSegment(PointVector3D start, PointVector3D finsih);
	~LineSegment();


	const PointVector3D& getStart() const;
	const PointVector3D& getFinish() const;
private:
	const PointVector3D start;
	const PointVector3D finish;
};

