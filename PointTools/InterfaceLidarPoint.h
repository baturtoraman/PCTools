#pragma once
class InterfaceLidarPoint {
public:
	virtual void setX(double) = 0;
	virtual void setY(double) = 0;
	virtual void setZ(double) = 0;

	virtual double getX() = 0;
	virtual double getY() = 0;
	virtual double getZ() = 0;

	virtual void setRedColor(int) = 0;
	virtual void setBlueColor(int) = 0;
	virtual void setGreenColor(int) = 0;

	virtual int getRedColor() = 0;
	virtual int getBlueColor() = 0;
	virtual int getGreenColor() = 0;

	virtual void setImagePix(double, double, double) = 0;

	virtual double getWidth() = 0;
	virtual double getHeight() = 0;
	virtual double getDistance() = 0;
};