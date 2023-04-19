#pragma once
#include "GeometryModule.h"

class CubeModule : public GeometryModule
{
public:
	CubeModule();
	~CubeModule();

	bool Init() override;
};
