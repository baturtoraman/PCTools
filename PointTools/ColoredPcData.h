#pragma once
#include "PcData.h"
#include "ColoredPcData.h"

#include <opencv2/core/types.hpp>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/photo.hpp>

class ColoredPcData
{
public:

	ColoredPcData(cv::Mat, double, double, double, std::string, std::string, std::string);
	~ColoredPcData();

	void Init(InterfacePcData*);

	void printLidarPoints();
	void pickColorsFrom360Image();
	void writeColoredPcToTextFile();
	void writeColoredPcAsLAS(std::string);

	void rotatePcDataAroundX();
	void rotatePcDataAroundY();
	void rotatePcDataAroundZ();

	void setHeading(double);
	void setRoll(double);
	void setPitch(double);
	void setFileName(std::string);
	void setImageName(std::string);
	void setFileNameToBeWritten(std::string);

	void saveImage();
	void Exit();

private:

	double heading = 0;
	double roll = 0;
	double pitch = 0;

	std::string fileName = NULL;
	std::string imageName = NULL;
	std::string fileNameToBeWritten = NULL;

	cv::Mat image360;
	InterfacePcData* pcData = nullptr;
};