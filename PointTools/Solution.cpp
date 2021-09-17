#include "Solution.h"


//Has-A-Relationship
void Solution::Init(InterfacePcData* pcData, InterfaceDepthImage* depthImage)
{
	this->pcData = pcData;
	this->depthImage = depthImage;
}

//Calculations are made here
void Solution::Run()
{

	pcData->carPosToOrigin();
	pcData->computeAngle();
	pcData->angle2Pix();
	depthImage->InitPcData(pcData);
	depthImage->PrintPixel();
	depthImage->printLidarPoints();
	depthImage->printClosest();
	depthImage->makeInterpolationOnX(5);
	depthImage->saveImage();

}


//Destroying the objects
void Solution::Exit()
{
	pcData->freePC();

	delete pcData;
	pcData = NULL;

	delete depthImage;
	depthImage = NULL;
}