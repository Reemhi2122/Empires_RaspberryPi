#pragma once
#include "GeometryModule.h"
#include <vector>
#include "OGLGraphics.h"

class DiscModule : public GeometryModule
{
public:
	DiscModule();
	~DiscModule();

	bool Init() override;

private:
	std::vector<GLfloat>* m_Vertices;
};