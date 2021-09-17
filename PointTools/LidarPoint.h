#pragma once
#include "InterfaceLidarPoint.h"

class LidarPoint : public InterfaceLidarPoint
{
public:
	LidarPoint(double, double, double);
	~LidarPoint();

	void setX(double) override;
	void setY(double) override;
	void setZ(double) override;

	double getX() override;
	double getY() override;
	double getZ() override;

	void setRedColor(int) override;
	void setBlueColor(int) override;
	void setGreenColor(int) override;

	int getRedColor() override;
	int getBlueColor() override;
	int getGreenColor() override;

	void setImagePix(double, double, double) override;

	double getWidth() override;
	double getHeight() override;
	double getDistance() override;

private:
	double x = 0, y = 0, z = 0, width = 0, height = 0, distance = 0;
	int r = 0, g = 0, b = 0;
};

