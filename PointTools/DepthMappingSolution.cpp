#include "DepthMappingSolution.h"

void DepthMappingSolution::Init(InterfacePcData* pcData, InterfaceDepthImage* depthImage)
{
	this->pcData = pcData;
	this->depthImage = depthImage;
}

//All the Algorithms Here for Depth Mapping
void DepthMappingSolution::Run()
{
	
	pcData->carPosToOrigin();
	pcData->computeAngle();
	pcData->angle2Pix();
	pcData->sortPoints();
	depthImage->InitPcData(pcData);
	//depthImage->PrintPixel();
	depthImage->printLidarPoints();
	depthImage->printClosest();
	depthImage->makeInterpolationOnX(5);
	depthImage->saveImage();

}

//Free Memory
void DepthMappingSolution::Exit()
{
	pcData->freePC();

	delete pcData;
	pcData = NULL;

	delete depthImage;
	depthImage = NULL;
}