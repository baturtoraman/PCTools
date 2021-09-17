#pragma once

#include "Solution.h"

class DepthMappingSolution : public Solution
{
public:

	void Init(InterfacePcData*, InterfaceDepthImage*);
	void Run();
	void Exit();

private:

	InterfacePcData* pcData = nullptr;
	InterfaceDepthImage* depthImage = nullptr;

};

