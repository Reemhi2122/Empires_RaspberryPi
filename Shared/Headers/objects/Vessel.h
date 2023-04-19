#pragma once
#include "Headers/ObjectModel.h"

class Planet;
class PhysicsObj;
class Empire;

class Vessel : public ObjectModel
{
public:
	Vessel();
	Vessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire);
	~Vessel();

	virtual bool Update();
	virtual bool Draw(Camera* a_ActiveCamera);

	virtual void UpdateVessel();
	virtual void PlanetActionCall() = 0;

	Planet* GetTargetPlanet();
	void SetTargetPlanet(Planet* a_TargetPlanet);
	
	Planet* GetStationaryPlanet();
	void SetStationaryPlanet(Planet* a_StationaryPlanet);

	PhysicsObj* GetPhysicsObj();

protected:
	unsigned int m_Health;
	unsigned int m_Armor;
	float m_Fuel;

	Empire* m_Empire;
	Planet* m_TargetPlanet;
	Planet* m_StationaryPlanet;

	PhysicsObj* m_PhysicsObj;
};