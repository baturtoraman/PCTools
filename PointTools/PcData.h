#pragma once
#include "InterfacePcData.h"

constexpr auto PI = 3.14159265;

class PcData : public InterfacePcData
{
public:

	void readPoints(std::string) override;
	void sortPoints() override;
	int getSize() override;

	void setCarPos(double, double, double) override;
	void carPosToOrigin() override;

	void computeAngle() override;
	void angle2Pix() override;
	void freePC() override;

	void rotationMatX(double) override;
	void rotationMatY(double) override;
	void rotationMatZ(double) override;

private:

	std::vector<double> carPos = { 0, 0, 0 };

};