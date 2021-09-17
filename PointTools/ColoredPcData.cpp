#include "ColoredPcData.h"

//Constructor
ColoredPcData::ColoredPcData(cv::Mat _image360, double _heading, double _roll, double _pitch, std::string _fileName, std::string _imageName, std::string _fileNameToBeWritten)
	: image360(_image360), heading(_heading), pitch(_pitch), fileName(_fileName), imageName(_imageName), fileNameToBeWritten(_fileNameToBeWritten)
{
}

ColoredPcData::~ColoredPcData()
{
}

//Has-A-Relationship
void ColoredPcData::Init(InterfacePcData* pcData)
{
	this->pcData = pcData;
}

//Printing Lidar Points to Image
void ColoredPcData::printLidarPoints()
{
	int r = 0, g = 0, b = 0;
	int width = 0, height = 0;
	double distance = 0;

	for (int i = 0; i < pcData->getSize(); i++) {

		width = pcData->pc[i]->getWidth();
		height = pcData->pc[i]->getHeight();
		distance = pcData->pc[i]->getDistance();

		image360.at<cv::Vec3b>(height, width).val[0] = (int)(distance * 50000) / (256 * 256);//b
		image360.at<cv::Vec3b>(height, width).val[1] = ((int)(distance * 50000) / 256) % 256;//g
		image360.at<cv::Vec3b>(height, width).val[2] = (int)(distance * 50000) % 256;//r
	}
}

//Picks Colors From 360 Image Channels and Sets Colors on Lidar Point Class
void ColoredPcData::pickColorsFrom360Image()
{
	int width = 0, height = 0;
	int size = pcData->pc.size();

	for (int i = 0; i < size; i++) {
		width = pcData->pc[i]->getWidth();
		height = pcData->pc[i]->getHeight();
		pcData->pc[i]->setBlueColor(image360.at<cv::Vec3b>(height, width)[0]);
		pcData->pc[i]->setGreenColor(image360.at<cv::Vec3b>(height, width)[1]);
		pcData->pc[i]->setRedColor(image360.at<cv::Vec3b>(height, width)[2]);
	}
}

//Writing Colorized Point Cloud to Txt File
void ColoredPcData::writeColoredPcToTextFile()
{
	int size = pcData->pc.size();
	int red = 0, green = 0, blue = 0;
	double x = 0, y = 0, z = 0;

	std::ofstream myfile;

	myfile.open(fileNameToBeWritten);

	for (int i = 0; i < size; i++) {

		x = pcData->pc[i]->getX();
		y = pcData->pc[i]->getY();
		z = pcData->pc[i]->getZ();

		blue = pcData->pc[i]->getBlueColor();
		green = pcData->pc[i]->getGreenColor();
		red = pcData->pc[i]->getRedColor();

		myfile << std::setprecision(25) << x << "," << std::setprecision(25) << y << "," << std::setprecision(25) << z << "," << blue << "," << green << "," << red << ",\n";
	}
	myfile.close();

}

void ColoredPcData::writeColoredPcAsLAS(std::string fileNameToBeSaved)
{
}


void ColoredPcData::setHeading(double heading)
{
	this->heading = heading;
}

void ColoredPcData::setRoll(double roll)
{
	this->roll = roll;
}

void ColoredPcData::setPitch(double pitch)
{
	this->pitch = pitch;
}

void ColoredPcData::setFileName(std::string fileName)
{
	this->fileName = fileName;
}

void ColoredPcData::saveImage()
{
	imwrite(imageName, image360);
}

void ColoredPcData::rotatePcDataAroundX()
{
}

void ColoredPcData::rotatePcDataAroundY()
{
}

void ColoredPcData::rotatePcDataAroundZ()
{
}

void ColoredPcData::setImageName(std::string imageName)
{
	this->imageName = imageName;
}

void ColoredPcData::setFileNameToBeWritten(std::string fileNameToBeWritten)
{
	this->fileNameToBeWritten = fileNameToBeWritten;
}
void ColoredPcData::Exit() {
}