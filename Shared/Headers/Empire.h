#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "Headers/objects/SupplyVessel.h"
#include "Headers/objects/DiscoveryVessel.h"
#include "Headers/objects/CarrierVessel.h"

class Planet;

enum ShipClass {
	EFighterShip,
	ESupplyVessel,
	EDiscoveryVessel,
	ECarrierVessel
};

class Empire
{
public:
	Empire();
	Empire(std::string a_Name, glm::vec4 a_Color, Planet* a_StartPlanet);
	Empire(std::string a_Name, glm::vec4 a_Color, Planet* a_StartPlanet, unsigned int a_BaseAmountOfCredits, unsigned int a_BaseTechLevel, unsigned int a_BaseResourceLevel);
	~Empire();

	void UpdateEmpire(bool a_UpdateResources);
	
	bool CreateShip(ShipClass a_ShipClass);
	void UpdateShips();
	void DrawShips(Camera* a_Camera);

	std::string GetName();
	glm::vec4 GetColor();

	unsigned int GetCredits();
	unsigned int GetTechLevel();
	void GenerateCredits(int a_Resources);

	std::vector<Planet*>* GetPlanets();
	void RemovePlanet(Planet* a_Planet);

	std::vector<SupplyVessel*>* GetSupplyVessels();
	std::vector<DiscoveryVessel*>* GetDiscoveryVessels();
	DiscoveryVessel* GetFreeDiscoveryVessel();
	void RemoveDiscoveryVessel(DiscoveryVessel* a_Vessel);

	std::vector<CarrierVessel*>* GetCarrierVessel();	
	CarrierVessel* GetFreeCarrierVessel();
	void RemoveCarrierVessel(CarrierVessel* a_Vessel);

	std::string ReturnStartPlanet();

	void SetSelectedPlanet(Planet* a_Planet);
	Planet* GetSelectedPlanet();

	void Explore(Planet* a_ExplorePlanet);
	void Attack(Planet* a_AttackPlanet, int a_AmountOfFighters);

	bool IsDefeat();

private:
	void UpdateCredits();

	void ReceiveResources(int a_Amount);
	void SetUpVessel(Vessel* a_Vessel);

	std::string m_Name = "Undefined";
	glm::vec4 m_Color = glm::vec4(1);
	
	unsigned int m_Credits = 0;
	unsigned int m_TechLevel = 0;
	unsigned int m_ResourceLevel = 0;
	bool m_Defeat = false;

	std::vector<Planet*> m_Planets;
	std::vector<SupplyVessel*> m_SupplyVessels;
	std::vector<DiscoveryVessel*> m_DiscoveryVessels;
	std::vector<CarrierVessel*> m_CarrierVessels;

#ifdef _RASPARM
	const char* fragshader = "../../../HelloCubes/Resources/Shaders/modelshader.frag";
	const char* vertexShader = "../../../HelloCubes/Resources/Shaders/modelshader.vert";
	const char* shipModel1 = "../../../Shared/Resources/Objects/brian_01.obj";
	const char* shipModel2 = "../../../Shared/Resources/Objects/brian_02.obj";
	const char* shipModel3 = "../../../Shared/Resources/Objects/brian_03.obj";
#else
	const char* fragshader = "../Empires(Windows)/Resources/Shaders/modelshader.frag";
	const char* vertexShader = "../Empires(Windows)/Resources/Shaders/modelshader.vert";
	const char* shipModel1 = "../Shared/Resources/Objects/brian_01.obj";
	const char* shipModel2 = "../Shared/Resources/Objects/brian_02.obj";
	const char* shipModel3 = "../Shared/Resources/Objects/brian_03.obj";
#endif

	Planet* m_SelectedPlanet;
};

inline std::string Empire::GetName() {
	return m_Name;
}

inline glm::vec4 Empire::GetColor() {
	return m_Color;
}