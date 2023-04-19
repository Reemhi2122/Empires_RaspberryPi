#pragma once
#include "Headers/objects/Vessel.h"

class CarrierVessel : public Vessel {
public:
	CarrierVessel();
	CarrierVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire);
	~CarrierVessel();

	void PlanetActionCall() override;
	void SetAttackPlanet(Planet* a_Planet);

	int GetFighters();
	void SetFighters(int a_AmountOfFighters);

	int GetPickupAmount();
	void SetPickupAmount(int a_PickupAmount);

private:
	Planet* m_AttackPlanet;
	
	int m_PickupAmount;
	int m_Fighters;
	int m_MaxCapactiy;
};