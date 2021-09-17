#pragma once

#include "PcData.h"
#include <opencv2/core/types.hpp>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/photo.hpp>

class InterfaceDepthImage {
protected:
	struct pointf
	{
		int x;
		int	y;
		double	depth;
		bool isEmpty;
	};

public:
	virtual void InitPcData(InterfacePcData*) = 0;
	virtual void PrintPixel() = 0;
	virtual void printLidarPoints() = 0;
	virtual void printClosest() = 0;

	virtual double foundOnImage(int, int, cv::Mat) = 0;
	virtual pointf findClosest(int, int, int, cv::Mat) = 0;
	virtual pointf findClosestOnX(int, int, int) = 0;

	virtual bool checkPoint(int, int) = 0;
	virtual void makeInterpolationOnX(int) = 0;
	virtual void saveImage() = 0;
};