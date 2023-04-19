#include "Headers/objects/DiscoveryVessel.h"
#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"

DiscoveryVessel::DiscoveryVessel() : Vessel()
{
	m_CurrentMaterial_ = nullptr;
	m_CurrentModule_ = nullptr;
	m_TargetPlanet = nullptr;
}

DiscoveryVessel::DiscoveryVessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire) : Vessel(a_Material, a_Module, a_Empire)
{
}

DiscoveryVessel::~DiscoveryVessel()
{}

void DiscoveryVessel::PlanetActionCall()
{
	if (m_TargetPlanet->GetOwner() != m_Empire) {
		if (m_TargetPlanet->IsOwned())
			m_Empire->RemoveDiscoveryVessel(this);

		m_TargetPlanet->RemoveVessels();
		m_TargetPlanet->SetOwner(m_Empire);
		m_Empire->GetPlanets()->push_back(m_TargetPlanet);
		m_TargetPlanet->AddVessel(this);
		m_StationaryPlanet = m_TargetPlanet;
		m_TargetPlanet = nullptr;
	}

	m_StationaryPlanet = m_TargetPlanet;
	m_TargetPlanet = nullptr;
}