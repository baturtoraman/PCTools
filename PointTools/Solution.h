#pragma once
#include "DepthImage.h"
#include "PcData.h"

class Solution
{

public:

	void Init(InterfacePcData*, InterfaceDepthImage*);
	void Run();
	void Exit();

private:

	InterfacePcData* pcData = nullptr;
	InterfaceDepthImage* depthImage = nullptr;
};

