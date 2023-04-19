#pragma once
#include "Headers/objects/Vessel.h"

class Planet;

class SupplyVessel : public Vessel
{
public:
	SupplyVessel();
	SupplyVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire);
	~SupplyVessel();

	void PlanetActionCall() override;

private:
	unsigned int m_Resources;
};