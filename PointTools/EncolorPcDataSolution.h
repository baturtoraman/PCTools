#pragma once
#include "DepthImage.h"
#include "PcData.h"
#include "ColoredPcData.h"

class EncolorPcDataSolution {

public:
	EncolorPcDataSolution();
	void Init(InterfacePcData*, ColoredPcData*);
	void Run();
	void Exit();

private:
	InterfacePcData* pcData = nullptr;
	InterfaceDepthImage* depthImage = nullptr;
	ColoredPcData* coloredPc = nullptr;
};