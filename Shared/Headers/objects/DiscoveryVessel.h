#pragma once
#include "Headers/objects/Vessel.h"

class DiscoveryVessel : public Vessel {
public:
	DiscoveryVessel();
	DiscoveryVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire);
	~DiscoveryVessel();

	void PlanetActionCall() override;

private:

};