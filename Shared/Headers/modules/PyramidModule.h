#pragma once
#include "GeometryModule.h"

class PyramidModule : public GeometryModule
{
public:
	PyramidModule();
	~PyramidModule();
	
	bool Init() override;
};
