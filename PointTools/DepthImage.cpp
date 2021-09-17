#include "DepthImage.h"
#include "qapplication.h"

//Constructor
DepthImage::DepthImage(cv::Mat _imgb, std::string _imgName) : imgb(_imgb), imgName(_imgName)
{
}

DepthImage::~DepthImage()
{
}

//Has-A-Relationship
void DepthImage::InitPcData(InterfacePcData* pcData)
{
	this->pcData = pcData;
}

void DepthImage::PrintPixel()
{
	for (int i = 0; i < pcData->getSize(); i++)
		std::cout << "x: " << pcData->pc[i]->getX() << " y: " << pcData->pc[i]->getY() << " z: " << pcData->pc[i]->getZ() << " distance: " << pcData->pc[i]->getDistance() << std::endl;
}

void DepthImage::printLidarPoints()
{
	int r = 0, g = 0, b = 0;
	int width = 0, length = 0;
	double distance = 0;

	for (int i = 0; i < pcData->getSize(); i++) {

		width = pcData->pc[i]->getWidth();
		length = pcData->pc[i]->getHeight();
		distance = pcData->pc[i]->getDistance();

		if (width > 0 && width < imageSizeX && length > 0 && length < blackYaxis)
		{
			imgb.at<cv::Vec3b>(length, width).val[0] = (int)(distance * 50000) / (256 * 256);//b
			imgb.at<cv::Vec3b>(length, width).val[1] = ((int)(distance * 50000) / 256) % 256;//g
			imgb.at<cv::Vec3b>(length, width).val[2] = (int)(distance * 50000) % 256;//r
		}
	}
	cv::imwrite("printed.png", imgb);
}

void DepthImage::printClosest()
{
	cv::Mat img_copy = imgb.clone();
	pointf closest;
	for (int i = 0; i < 3; i++) {
		for (int y = 0; y < blackYaxis; y++)
		{
			for (int x = 0; x < imageSizeX; x++)
			{
				closest = findClosest(x, y, 3, imgb);
				img_copy.at<cv::Vec3b>(y, x)[0] = (int)closest.depth / (256 * 256);
				img_copy.at<cv::Vec3b>(y, x)[1] = (int)(closest.depth / 256) % 256;
				img_copy.at<cv::Vec3b>(y, x)[2] = (int)closest.depth % 256;
			}
		}
		imgb = img_copy.clone();
	}
}

double DepthImage::foundOnImage(int x, int y, cv::Mat img)
{
	if (x < 0 || x > imageSizeX || y < 0 || y >blackYaxis)
	{
		return 0;
	}

	int b = img.at<cv::Vec3b>(y, x)[0];
	int g = img.at<cv::Vec3b>(y, x)[1];
	int r = img.at<cv::Vec3b>(y, x)[2];
	r = r + g * 256.0 + b * 256.0 * 256.0;
	return r;
}

DepthImage::pointf DepthImage::findClosest(int x, int y, int maxDistance, cv::Mat img)
{
	QApplication::processEvents();
	pointf zeropoint;
	zeropoint.depth = 0;
	zeropoint.x = 0;
	zeropoint.y = 0;
	zeropoint.isEmpty = true;

	pointf closest = zeropoint;

	if (x < 0 || x>imageSizeX || y < 0 || y>blackYaxis)
	{
		return zeropoint;
	}

	bool found = checkPoint(x, y);
	double distance = 0.0;

	if (!found)
	{
		for (int d = 1; d < maxDistance; d++)
		{
			for (int i = 0; i < d + 1; i++)
			{
				int x1 = x - d + i;
				int y1 = y - i;

				found = checkPoint(x1, y1);
				if (found) {
					closest.depth /*distance*/ = foundOnImage(x1, y1, img);
					closest.x = x1;
					closest.y = y1;
					closest.isEmpty = false;
					return closest/*distance*/;
				}

				int x2 = x + d - i;
				int y2 = y + i;

				found = checkPoint(x2, y2);
				if (found) {
					closest.depth = foundOnImage(x2, y2, img);
					closest.x = x2;
					closest.y = y2;
					closest.isEmpty = false;
					return closest/*distance*/;
				}
			}

			for (int i = 1; i < d; i++)
			{
				int x1 = x - i;
				int y1 = y + d - i;

				found = checkPoint(x1, y1);
				if (found) {
					closest.depth = foundOnImage(x1, y1, img);
					closest.x = x1;
					closest.y = y1;
					closest.isEmpty = false;
					return  closest/*distance*/;
				}

				int x2 = x + i;
				int y2 = y - d + i;

				found = checkPoint(x2, y2);
				if (found) {
					closest.depth = foundOnImage(x2, y2, img);
					closest.x = x2;
					closest.y = y2;
					closest.isEmpty = false;
					return closest;
				}
			}
		}
	}
	else {
		closest.depth = foundOnImage(x, y, img);
		closest.x = x;
		closest.y = y;
		closest.isEmpty = false;
	}
	return closest;
}

bool DepthImage::checkPoint(int xf, int yf)
{
	if (xf < 0 || xf>imageSizeX || yf < 0 || yf>blackYaxis) {
		return true;
	}

	int v1 = (int)imgb.at<cv::Vec3b>(yf, xf)[0];
	int v2 = (int)imgb.at<cv::Vec3b>(yf, xf)[1];
	int v3 = (int)imgb.at<cv::Vec3b>(yf, xf)[2];

	if (v1 != 0 || v2 != 0 || v3 != 0) {

		return true;
	}

	return false;
}

void DepthImage::makeInterpolationOnX(int d) {
	int x = 0, y = 0;
	double depth = 0.0;
	int distance_p1, distance_p2, total_distance;
	pointf temp;
	temp.x = 0;
	temp.y = 0;
	temp.depth = 0.0;

	pRGB p1, p2;

	for (int j = 0; j < 1023; j++) {
		for (int i = 0; i < imageSizeX; i++) {
			

			p1.red = (int)imgb.at<cv::Vec3b>(j, i)[2];
			p1.green = (int)imgb.at<cv::Vec3b>(j, i)[1];
			p1.blue = (int)imgb.at<cv::Vec3b>(j, i)[0];

			if (p1.red != 0 || p1.green != 0 || p1.blue != 0) {
				temp = findClosestOnX(i, j, d);
				x = temp.x;
				y = temp.y;
				depth = temp.depth;

				p2.red = (int)depth % 256;
				p2.green = (int)(depth / 256) % 256;
				p2.blue = (int)depth / (256 * 256);

				int count = 0;
				if (depth > 0) {
					for (int t = 1; t < x - i - 1; t++)
					{
						count++;
						distance_p1 = t;
						distance_p2 = x - (i + t);
						total_distance = distance_p1 + distance_p2;

						if (x + t < imageSizeX) {

							imgb.at<cv::Vec3b>(y, x + t)[2] = (int)((p1.red * distance_p1 + p2.red * distance_p2) / total_distance);
							imgb.at<cv::Vec3b>(y, x + t)[1] = (int)((p1.green * distance_p1 + p2.green * distance_p2) / total_distance);
							imgb.at<cv::Vec3b>(y, x + t)[0] = (int)((p1.blue * distance_p1 + p2.blue * distance_p2) / total_distance);
						}
					}
				}

				i = temp.x;
			}
		}
	}
}

DepthImage::pointf DepthImage::findClosestOnX(int x, int y, int d) {

	pointf closest_int;
	pointf zeropoint;
	zeropoint.x = x + d, zeropoint.y = y, zeropoint.depth = 0;

	bool found = false;

	for (int i = 2; i < d; i++)
	{
		x = x + i;

		found = checkPoint(x, y);

		if (found) {
			closest_int.depth = foundOnImage(x, y, imgb);
			closest_int.x = x;
			closest_int.y = y;
			return  closest_int/*distance*/;
		}

	}

	return zeropoint;
}

cv::Mat DepthImage::getImage()
{
	return imgb;
}

void DepthImage::saveImage()
{
	imwrite(imgName, imgb);
}
