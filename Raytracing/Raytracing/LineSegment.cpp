#include "LineSegment.h"

LineSegment::LineSegment(PointVector3D start, PointVector3D finsih): start(start), finish(finsih)
{
}

LineSegment::~LineSegment()
{
}

const PointVector3D& LineSegment::getStart() const
{
	return start;
}

const PointVector3D& LineSegment::getFinish() const
{
	return finish;
}
