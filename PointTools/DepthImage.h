#pragma once
#include "InterfaceDepthImage.h"

class DepthImage : public InterfaceDepthImage
{

private:
	InterfacePcData* pcData = nullptr;
	cv::Mat imgb;

	const int DENSE = 50000;
	const int imageSizeX = 2047;
	const int imageSizeY = 1023;
	int blackYaxis = (int)(imageSizeY) * 0.82;
	std::string imgName = NULL;

	struct pRGB
	{
		int red, green, blue;
	};

public:

	DepthImage(cv::Mat, std::string);
	~DepthImage();

	void InitPcData(InterfacePcData*) override;
	void PrintPixel() override;
	void printLidarPoints() override;
	void printClosest() override;
	double foundOnImage(int, int, cv::Mat) override;
	pointf findClosest(int, int, int, cv::Mat) override;
	bool checkPoint(int, int) override;
	void makeInterpolationOnX(int) override;
	pointf findClosestOnX(int, int, int) override;
	//void interpolationOnX();
	cv::Mat getImage();
	void saveImage() override;
};