#pragma once
#include "GeometryModule.h"
#include "Headers/objects/Model.h"
#include "TOL/tiny_obj_loader.h"
#include <vector>
#include "OGLGraphics.h"

class DebugModule : public GeometryModule
{
public:
	DebugModule(std::vector<GLfloat>* a_Vertices);
	~DebugModule();

	bool Init() override;

private:
	std::vector<GLfloat>* m_Vertices;
};