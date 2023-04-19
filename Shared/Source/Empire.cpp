#include "Headers/objects/Planet.h"
#include "Headers/Empire.h"
#include "Headers/objects/SupplyVessel.h"
#include "Headers/utilities/Shader.h"
#include "Headers/modules/ModelModule.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/DataStorage.h"

#define DEG2RAD(x) (x*PI)/180
#define RAD2DEG(x) x*(180/PI)

Empire::Empire()
{
	m_Name = "Undefined";
	m_Color = glm::vec4(1);
}

Empire::Empire(std::string a_Name, glm::vec4 a_Color, Planet* a_StartPlanet)
{
	m_Name = a_Name;
	m_Color = a_Color;
	m_Planets.push_back(a_StartPlanet);

	Shader* default_shader = new Shader(fragshader, vertexShader);

	DiscoveryVessel* vessel = new DiscoveryVessel(new Material(*default_shader), new ModelModule(shipModel1), this);
	SetUpVessel(vessel);
	m_DiscoveryVessels.push_back(vessel);
}

Empire::Empire(std::string a_Name, glm::vec4 a_Color, Planet* a_StartPlanet, unsigned int a_BaseAmountOfCredits,
	unsigned int a_BaseTechLevel, unsigned int a_BaseResourceLevel) :
	Empire(a_Name, a_Color, a_StartPlanet)
{
	m_Credits = a_BaseAmountOfCredits;
	m_TechLevel = a_BaseTechLevel;
	m_ResourceLevel = a_BaseResourceLevel;
}

Empire::~Empire()
{}

void Empire::UpdateEmpire(bool a_UpdateResources)
{	
	if (!m_Defeat) {
		if (a_UpdateResources)
			UpdateCredits();

		UpdateShips();
	}
}

unsigned Empire::GetCredits()
{
	return m_Credits;
}

unsigned Empire::GetTechLevel()
{
	return m_TechLevel;
}

void Empire::GenerateCredits(int a_Resources)
{
	m_Credits += static_cast<int>(a_Resources / 50);
}

std::vector<Planet*>* Empire::GetPlanets()
{
	return &m_Planets;
}

void Empire::RemovePlanet(Planet* a_Planet)
{
	for (int i = 0; i < m_Planets.size(); i++)
		if (m_Planets[i] == a_Planet)
			m_Planets.erase(m_Planets.begin() + i);

	if (m_Planets.size() <= 0) {
		m_Defeat = true;
	}
}

std::vector<SupplyVessel*>* Empire::GetSupplyVessels()
{
	return &m_SupplyVessels;
}

std::vector<DiscoveryVessel*>* Empire::GetDiscoveryVessels()
{
	return &m_DiscoveryVessels;
}

DiscoveryVessel* Empire::GetFreeDiscoveryVessel()
{
	DiscoveryVessel* vessel = nullptr;
	for (int i = 0; i < m_DiscoveryVessels.size(); i++)
		if (m_DiscoveryVessels[i]->GetTargetPlanet() == nullptr) return m_DiscoveryVessels[i];

	return vessel;
}

void Empire::RemoveDiscoveryVessel(DiscoveryVessel* a_Vessel)
{
	for (int i = 0; i < m_DiscoveryVessels.size(); i++)
		if (m_DiscoveryVessels[i] == a_Vessel)
			m_DiscoveryVessels.erase(m_DiscoveryVessels.begin() + i);
}

std::vector<CarrierVessel*>* Empire::GetCarrierVessel()
{
	return &m_CarrierVessels;
}

CarrierVessel* Empire::GetFreeCarrierVessel()
{
	CarrierVessel* vessel = nullptr;
	for (int i = 0; i < m_CarrierVessels.size(); i++)
		if (m_CarrierVessels[i]->GetTargetPlanet() == nullptr) return m_CarrierVessels[i];

	return vessel;
}

void Empire::RemoveCarrierVessel(CarrierVessel* a_Vessel)
{
	for (int i = 0; i < m_CarrierVessels.size(); i++)
		if (m_CarrierVessels[i] == a_Vessel)
			m_CarrierVessels.erase(m_CarrierVessels.begin() + i);
}

std::string Empire::ReturnStartPlanet()
{
	return m_Planets[0]->GetName();
}

void Empire::SetSelectedPlanet(Planet* a_Planet)
{
	m_SelectedPlanet = a_Planet;
}

Planet* Empire::GetSelectedPlanet()
{
	return m_SelectedPlanet;
}

void Empire::Explore(Planet* a_ExplorePlanet)
{
	GetFreeDiscoveryVessel()->SetTargetPlanet(a_ExplorePlanet);
}

void Empire::Attack(Planet* a_AttackPlanet, int a_AmountOfFighters)
{
	CarrierVessel* vessel = GetFreeCarrierVessel();
	vessel->SetPickupAmount(a_AmountOfFighters);
	vessel->SetAttackPlanet(a_AttackPlanet);
	vessel->SetTargetPlanet(m_SelectedPlanet);
}

bool Empire::IsDefeat()
{
	return m_Defeat;
}

void Empire::UpdateCredits()
{
	// this takes 20% of every planet associated with the empire
	// and converts it into credits
	for (int i = 0; i < m_Planets.size(); i++)
	{
		// calculating 20% of the current resources of a planet
		float temp = m_Planets[i]->GetResources() / 100.f * 20.f;
		m_Planets[i]->ReduceResources(static_cast<int>(temp));

		// convert into credits and add to Empire credits
		m_Credits += static_cast<int>(temp * m_TechLevel) / 10;
	}
}

void Empire::ReceiveResources(int a_Amount)
{
	m_Credits += a_Amount * (m_TechLevel / 10);
}

bool Empire::CreateShip(ShipClass a_ShipClass)
{
	DataStorage* storage;
	storage = storage->GetInstance();
	unsigned int cost;
	switch (a_ShipClass)
	{
	case EFighterShip:
		cost = storage->GetFighterShipCost();
		if (m_Credits > cost) {
			m_Credits -= cost;
			return true;
		}
		break;
	case EDiscoveryVessel:
		if (GetDiscoveryVessels()->size() >= 3) return false;
		cost = storage->GetDiscoveryVesselCost();
		if (m_Credits > cost) {
			Shader* default_shader = new Shader(fragshader, vertexShader);

			DiscoveryVessel* vessel = new DiscoveryVessel(new Material(*default_shader), new ModelModule(shipModel1), this);
			SetUpVessel(vessel);
			m_DiscoveryVessels.push_back(vessel);
			m_Credits -= cost;
			return true;
		}
		break;
	case ECarrierVessel:
		if (GetCarrierVessel()->size() >= 3) return false;
		cost = storage->GetCarrierVesselCost();
		if (m_Credits > cost) {
			Shader* default_shader = new Shader(fragshader, vertexShader);

			CarrierVessel* vessel = new CarrierVessel(new Material(*default_shader), new ModelModule(shipModel2), this);
			SetUpVessel(vessel);
			m_CarrierVessels.push_back(vessel);
			m_Credits -= cost;
			return true;
		}
		break;
	case ESupplyVessel:
		if (GetSupplyVessels()->size() >= 2) return false;
		cost = storage->GetSupplyVesselCost();
		if (m_Credits > cost) {
			Shader* default_shader = new Shader(fragshader, vertexShader);
			
			SupplyVessel* vessel = new SupplyVessel(new Material(*default_shader), new ModelModule(shipModel3), this);
			SetUpVessel(vessel);
			m_SupplyVessels.push_back(vessel);
			m_Credits -= cost;
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

void Empire::SetUpVessel(Vessel* a_Vessel)
{
	a_Vessel->Scales = glm::vec3(0.1f, 0.1f, 0.1f);
	a_Vessel->WorldPosition = m_Planets[0]->GetPosition() + glm::vec3(0, 1.5f, 0);
	m_Planets[0]->AddVessel(a_Vessel);
	a_Vessel->SetStationaryPlanet(m_Planets[0]);
	
	a_Vessel->DegreeRotations.x -= 90.f;
	a_Vessel->Rotations.x = DEG2RAD(a_Vessel->DegreeRotations.x);
}

void Empire::UpdateShips()
{
	for (int i = 0; i < m_SupplyVessels.size(); i++)
		m_SupplyVessels[i]->Update();
	for (int i = 0; i < m_CarrierVessels.size(); i++)
		m_CarrierVessels[i]->Update();
	for (int i = 0; i < m_DiscoveryVessels.size(); i++)
		m_DiscoveryVessels[i]->Update();
}

void Empire::DrawShips(Camera* a_Camera)
{
	if (m_Defeat) return;
	for (int i = 0; i < m_SupplyVessels.size(); i++)
		m_SupplyVessels[i]->Draw(a_Camera);
	for (int i = 0; i < m_CarrierVessels.size(); i++)
		m_CarrierVessels[i]->Draw(a_Camera);
	for (int i = 0; i < m_DiscoveryVessels.size(); i++)
		m_DiscoveryVessels[i]->Draw(a_Camera);
}
