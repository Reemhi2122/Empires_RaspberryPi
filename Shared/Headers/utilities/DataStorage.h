#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "objects/Planet.h"
#include "Headers/Empire.h"
#include "Headers/EmpireAI.h"

#ifdef _RASPARM
#include <bullet/btBulletDynamicsCommon.h>
#else
#include "../Empires(Windows)/PyBullet/src/btBulletDynamicsCommon.h"
#endif

//class QuadTree;
class Shader;

class DataStorage
{
public:
	/// <summary>
	/// A singleton used for central data storage. It is mainly used by the generator and houses
	/// all of the planets, modules and materials that can be accessed via their respective indices.
	/// 
	/// </summary>
	static DataStorage* GetInstance();

	int amount_planets = 8192;
	unsigned int max_num_empires = 5;

	void ClearData();
	
	unsigned int GetPlanetStorageSize() const;
	unsigned int GetEmpireAIStorageSize() const;

	btDiscreteDynamicsWorld* GetPhysicsWorld();
	void SetPhysicsWorld(btDiscreteDynamicsWorld* a_PhysicsWorld);
	
	Planet* GetPlanet(unsigned int a_Index);
	
	void SetStarPosition(unsigned int a_Index, glm::vec3& a_Position);
	void SetPlanetPosition(unsigned int a_Index, glm::vec3& a_Position);

	int SetPlanetAt(Planet& a_Planet, int a_Index);

	void PushBackPlanet(Planet& a_Planet);

	int PushBackShader(Shader* a_Shader);
	bool SetShaderAt(int a_Index, Shader* a_Shader);
	
	int PushBackMaterial(Material* a_Material);
	bool SetMaterialAt(int a_Index, Material* a_Material);

	int PushBackModule(GeometryModule* a_Module);
	bool SetModuleAt(int a_Index, GeometryModule* a_Module);

	int PushBackEmpire(Empire* a_Empire);
	Empire* GetEmpire(int a_Index);
	void RemoveEmpire(Empire* a_Empire);

	int PushBackEmpireAI(EmpireAI* a_Empire);
	EmpireAI* GetEmpireAI(int a_Index);

	unsigned int GetDiscoveryVesselCost();
	unsigned int GetCarrierVesselCost();
	unsigned int GetSupplyVesselCost();
	unsigned int GetFighterShipCost();

private:
	DataStorage();
	~DataStorage();

	static DataStorage* m_Instance_;
	
	vector<Shader*> m_AllShaders_;
	vector<Material*> m_AllMaterials_;
	vector<GeometryModule*> m_AllModules_;
	vector<Empire*> m_AllEmpires_;
	vector<EmpireAI*> m_AllEmpireAI_;
	vector<Planet> all_planets;

	btDiscreteDynamicsWorld* m_PhysicsWorld;

	unsigned int m_DiscoveryVesselCost = 2;
	unsigned int m_CarrierVesselCost = 2;
	unsigned int m_SupplyVesselCost = 2;
	unsigned int m_FighterShipCost = 2;

	//unsigned int m_DiscoveryVesselCost = 1000;
	//unsigned int m_CarrierVesselCost = 2000;
	//unsigned int m_SupplyVesselCost = 4000;
	//unsigned int m_FighterShipCost = 50;
};
