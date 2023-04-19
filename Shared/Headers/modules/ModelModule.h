#pragma once
#include "GeometryModule.h"
#include "Headers/objects/Model.h"
#include "TOL/tiny_obj_loader.h"

class ModelModule : public GeometryModule
{
public:
	ModelModule(const char* a_File);
	~ModelModule();

	bool Init() override;

	void LoadModel(const char* a_ModelFile);

private:
	Model m_Model;
};