#pragma once
#include "Headers/ObjectModel.h"

class SOI : public ObjectModel
{
public:
	SOI();
	SOI(Material* a_Material, GeometryModule* a_Module);
	~SOI();

	virtual bool Update();
	virtual bool Draw(Camera* a_ActiveCamera);

	void SetColor(glm::vec3 a_Color);

private:
	glm::vec3 m_Color;
};