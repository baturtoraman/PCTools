#pragma once
#include "LidarPoint.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h> 
#include <algorithm>
#include <sstream>

class InterfacePcData {

public:
	std::vector<InterfaceLidarPoint*> pc;

	virtual void readPoints(std::string) = 0;
	virtual void sortPoints() = 0;
	virtual int getSize() = 0;
	virtual void carPosToOrigin() = 0;
	virtual void computeAngle() = 0;
	virtual void angle2Pix() = 0;
	virtual void setCarPos(double, double, double) = 0;
	virtual void freePC() = 0;

	virtual void rotationMatX(double) = 0;
	virtual void rotationMatY(double) = 0;
	virtual void rotationMatZ(double) = 0;
};