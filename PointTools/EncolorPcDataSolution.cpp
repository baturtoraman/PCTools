#include "EncolorPcDataSolution.h"



EncolorPcDataSolution::EncolorPcDataSolution()
{
}

void EncolorPcDataSolution::Init(InterfacePcData* pcData, ColoredPcData* coloredPc)
{
	this->pcData = pcData;
	this->coloredPc = coloredPc;
}

//All the Algorithms Here for Colorization 
void EncolorPcDataSolution::Run()
{
	pcData->carPosToOrigin();
	pcData->computeAngle();
	pcData->angle2Pix();

	coloredPc->Init(pcData);
	//coloredPc->printLidarPoints();
	/*coloredPc->setImageName("print.png");
	coloredPc->saveImage();*/
	coloredPc->pickColorsFrom360Image();
	//coloredPc->writeColoredPcAsLAS("");
	coloredPc->writeColoredPcToTextFile();

}

//Free Memory
void EncolorPcDataSolution::Exit()
{
	pcData->freePC();

	delete pcData;
	pcData = NULL;

	delete coloredPc;
	coloredPc = NULL;

}
