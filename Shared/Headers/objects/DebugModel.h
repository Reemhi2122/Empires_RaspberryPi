#pragma once
#include "Headers/ObjectModel.h"

class DebugModel : public ObjectModel
{
public:
	DebugModel();
	DebugModel(Material* a_Material, GeometryModule* a_Module, int a_Size);
	~DebugModel();

	virtual bool Update();
	virtual bool Draw(Camera* a_ActiveCamera);

protected:
	int m_Size;
};