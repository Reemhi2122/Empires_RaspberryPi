#pragma once
#include "GeometryModule.h"
#include "Headers/objects/Model.h"
#include "TOL/tiny_obj_loader.h"
#include <vector>
#include "glm/glm.hpp"

class InstanceModelModule : public GeometryModule
{
public:
	InstanceModelModule(const char* a_File, std::vector<glm::mat4>* a_Positions);
	~InstanceModelModule();

	bool Init() override;

	void LoadModel(const char* a_ModelFile);

private:
	Model m_Model;
	unsigned int buffer;
	std::vector<glm::mat4>* positions;
};