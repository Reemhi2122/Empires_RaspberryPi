#include "Headers/objects/SupplyVessel.h"
#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"

SupplyVessel::SupplyVessel()
{
	m_CurrentMaterial_ = nullptr;
	m_CurrentModule_ = nullptr;
	m_TargetPlanet = nullptr;
}

SupplyVessel::SupplyVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire) : Vessel(a_Material, a_Module, a_Empire)
{
	m_Resources = 1;
	if(m_Empire->GetPlanets()->size() > 1) PlanetActionCall();
}

SupplyVessel::~SupplyVessel()
{}

void SupplyVessel::PlanetActionCall()
{
	if (m_Resources > 0) {
		m_Empire->GenerateCredits(m_Resources);
		m_Resources = 0;

		int m_HighestResource = 0;
		int index;
		for (int i = 1; i < m_Empire->GetPlanets()->size(); i++)
		{
			int m_CurResources = m_Empire->GetPlanets()->at(i)->GetResources();
			if (m_CurResources > m_HighestResource) {
				m_TargetPlanet = m_Empire->GetPlanets()->at(i);
				m_HighestResource = m_CurResources;
			}
		}
		return;
	}

	m_Resources = m_TargetPlanet->GetResources();
	m_TargetPlanet->ResetResources();
	m_TargetPlanet = m_Empire->GetPlanets()->at(0);
}