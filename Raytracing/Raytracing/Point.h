#pragma once
class Point
{
public:
	explicit Point(double x = 0, double y = 0);
	~Point();


	double getX() const;
	double getY() const;
private:
	double x;
	double y;
};

