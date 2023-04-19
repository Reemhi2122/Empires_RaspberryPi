#include "utilities/DataStorage.h"
#include "modules/CubeModule.h"
#include "materials/Material.h"
#include "Headers/utilities/Shader.h"
#include "Headers/Empire.h"

DataStorage* DataStorage::m_Instance_ = nullptr;

DataStorage::DataStorage() 
{}

DataStorage::~DataStorage()
{
	ClearData();
}

DataStorage* DataStorage::GetInstance()
{
	if (m_Instance_ == nullptr)
	{
		m_Instance_ = new DataStorage;
	}
	return m_Instance_;
}

void DataStorage::ClearData()
{
	for (int k = 0; k < m_AllModules_.size(); k++)
	{
		delete m_AllModules_[k];
		m_AllModules_.clear();
	}
	for (int k = 0; k < m_AllShaders_.size(); k++)
	{
		delete m_AllShaders_[k];
		m_AllShaders_.clear();
	}
	for (int k = 0; k < m_AllModules_.size(); k++)
	{
		delete m_AllModules_[k];
		m_AllModules_.clear();
	}
	for (int k = 0; k < m_AllEmpires_.size(); k++)
	{
		delete m_AllEmpires_[k];
		m_AllEmpires_.clear();
	}
	
	all_planets.clear();
}

unsigned DataStorage::GetPlanetStorageSize() const
{
	return all_planets.size();
}

unsigned int DataStorage::GetEmpireAIStorageSize() const
{
	return m_AllEmpireAI_.size();
}

btDiscreteDynamicsWorld* DataStorage::GetPhysicsWorld()
{
	return m_PhysicsWorld;
}

void DataStorage::SetPhysicsWorld(btDiscreteDynamicsWorld* a_PhysicsWorld)
{
	m_PhysicsWorld = a_PhysicsWorld;
}

Planet* DataStorage::GetPlanet(unsigned a_Index)
{
	return &all_planets[a_Index];
}

int DataStorage::SetPlanetAt(Planet& a_Planet, int a_Index)
{
	assert(a_Index >= 0 && a_Index < amount_planets);
	if (a_Index < 0 || a_Index > amount_planets)
	{
		printf("DataStorage::SetPlanetAt failed! Index needs to be between 0 and %i", sizeof(all_planets) / sizeof(Planet));
		return - 1;
	}
	
	all_planets[a_Index] = a_Planet;

	return 1;
}

void DataStorage::PushBackPlanet(Planet& a_Planet)
{
	if (all_planets.size() <= amount_planets)
	{
		all_planets.push_back(a_Planet);
	}
}

int DataStorage::PushBackShader(Shader* a_Shader)
{
	if (a_Shader == nullptr)
	{
		#ifdef DEBUG
		printf("DataStorage::PushBackShader failed! Pointer was nullptr");
		assert(a_Shader != nullptr);
		#endif
		return -1;
	}
	
	m_AllShaders_.push_back(a_Shader);
	return m_AllShaders_.size() - 1;
}

bool DataStorage::SetShaderAt(const int a_Index, Shader* a_Shader)
{
	assert(a_Shader != nullptr);
	if (a_Index < 0 || a_Index > m_AllShaders_.size())
	{

		#ifdef DEBUG
		printf("DataStorage::SetShaderAt() failed! Index must be between 0 and %i", m_AllShaders_.size());
		#endif
		
		return false;
	}
	delete m_AllShaders_[a_Index];
	m_AllShaders_[a_Index] = a_Shader;
	return true;
}

int DataStorage::PushBackMaterial(Material* a_Material)
{
	if (a_Material == nullptr)
	{
		#ifdef DEBUG
		printf("DataStorage::PushBackMaterial failed! Pointer was nullptr");
		assert(a_Material != nullptr);
		#endif
		return -1;
	}

	m_AllMaterials_.push_back(a_Material);
	return m_AllMaterials_.size() - 1;;
}


bool DataStorage::SetMaterialAt(const int a_Index, Material* a_Material)
{
	assert(a_Material != nullptr);
	if (a_Index < 0 || a_Index > m_AllShaders_.size())
	{

		#ifdef DEBUG
		printf("DataStorage::SetMaterialAt() failed! Index must be between 0 and %i", m_AllMaterials_.size());
		#endif

		return false;
	}
	delete m_AllMaterials_[a_Index];
	m_AllMaterials_[a_Index] = a_Material;
	return true;
}

int DataStorage::PushBackModule(GeometryModule* a_Module)
{
	if (a_Module == nullptr)
	{
		#ifdef DEBUG
		printf("DataStorage::PushBackModule failed! Pointer was nullptr");
		assert(a_Module != nullptr);
		#endif
		return -1;
	}

	m_AllModules_.push_back(a_Module);
	return m_AllModules_.size() - 1;;
}

bool DataStorage::SetModuleAt(int a_Index, GeometryModule* a_Module)
{
	assert(a_Module != nullptr);
	if (a_Index < 0 || a_Index > m_AllModules_.size())
	{

		#ifdef DEBUG
		printf("DataStorage::SetModuleAt() failed! Index must be between 0 and %i", m_AllModules_.size());
		#endif

		return false;
	}
	delete m_AllModules_[a_Index];
	m_AllModules_[a_Index] = a_Module;
	return true;
}

int DataStorage::PushBackEmpire(Empire* a_Empire)
{
	if (a_Empire == nullptr)
	{
#ifdef DEBUG
		printf("DataStorage::PushBackEmpire failed! Pointer was nullptr");
		assert(a_Empire != nullptr);
#endif
		return -1;
	}

	m_AllEmpires_.push_back(a_Empire);
	return m_AllEmpires_.size() - 1;;
}

Empire* DataStorage::GetEmpire(int a_Index)
{
	if (a_Index < 0 || a_Index > m_AllEmpires_.size())
	{

		#ifdef DEBUG
		printf("DataStorage::GetEmpire() failed! Index must be between 0 and %i", m_AllEmpires_.size());
		#endif

		return nullptr;
	}

	return m_AllEmpires_[a_Index];
}

void DataStorage::RemoveEmpire(Empire* a_Empire)
{
	for (int i = 0; i < m_AllEmpires_.size(); i++)
		if (m_AllEmpires_[i] == a_Empire)
			m_AllEmpires_.erase(m_AllEmpires_.begin() + i);
}

int DataStorage::PushBackEmpireAI(EmpireAI* a_Empire)
{
	m_AllEmpireAI_.push_back(a_Empire);
	return m_AllEmpireAI_.size() - 1;;
}

EmpireAI* DataStorage::GetEmpireAI(int a_Index)
{
	return m_AllEmpireAI_[a_Index];
}

unsigned int DataStorage::GetDiscoveryVesselCost()
{
	return m_DiscoveryVesselCost;
}

unsigned int DataStorage::GetCarrierVesselCost()
{
	return m_CarrierVesselCost;
}

unsigned int DataStorage::GetSupplyVesselCost()
{
	return m_SupplyVesselCost;
}

unsigned int DataStorage::GetFighterShipCost()
{
	return m_FighterShipCost;
}
