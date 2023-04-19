#include "Headers/objects/CarrierVessel.h"
#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"

CarrierVessel::CarrierVessel()
{
	m_CurrentMaterial_ = nullptr;
	m_CurrentModule_ = nullptr;
	m_TargetPlanet = nullptr;
}

CarrierVessel::CarrierVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire) : Vessel(a_Material, a_Module, a_Empire)
{}

CarrierVessel::~CarrierVessel()
{}

void CarrierVessel::PlanetActionCall()
{
	if (m_AttackPlanet != nullptr) {
		this->m_Fighters = m_PickupAmount;
		m_TargetPlanet->AddFighterAmount(-m_PickupAmount);
		m_PickupAmount = 0;
		SetTargetPlanet(m_AttackPlanet);
		m_AttackPlanet = nullptr;
		return;
	}

	if (m_TargetPlanet->GetOwner() != m_Empire) {
		if (m_TargetPlanet->GetFighterAmount() < m_Fighters) {
			m_TargetPlanet->SetFighterAmount(m_Fighters - m_TargetPlanet->GetFighterAmount());
			m_Fighters = 0;
			m_TargetPlanet->SetOwner(m_Empire);
			m_Empire->GetPlanets()->push_back(m_TargetPlanet);
			m_StationaryPlanet = m_TargetPlanet;
			m_TargetPlanet = nullptr;
			return;
		}
		else {
			m_TargetPlanet->SetFighterAmount(m_Fighters - m_TargetPlanet->GetFighterAmount());
			m_Empire->RemoveCarrierVessel(this);
		}
	}
	else {
		m_Empire->GetPlanets()->push_back(m_TargetPlanet);
		m_StationaryPlanet = m_TargetPlanet;
		m_TargetPlanet = nullptr;
	}
}

void CarrierVessel::SetAttackPlanet(Planet* a_Planet)
{
	m_AttackPlanet = a_Planet;
}

int CarrierVessel::GetFighters()
{
	return m_Fighters;
}

void CarrierVessel::SetFighters(int a_AmountOfFighters)
{
	m_Fighters = a_AmountOfFighters;
}

int CarrierVessel::GetPickupAmount()
{
	return m_PickupAmount;
}

void CarrierVessel::SetPickupAmount(int a_PickupAmount)
{
	m_PickupAmount = a_PickupAmount;
}