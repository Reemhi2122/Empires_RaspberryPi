#include "Headers/utilities/Manager.h"
#include "Headers/Camera.h"
#include "Headers/objects/Planet.h"
#include "Headers/utilities/DataStorage.h"
#include "Headers/utilities/Generator.h"
#include "Headers/utilities/Timer.h"
#include <cassert>

Manager::Manager(bool a_Graphicsinitialized, const char* a_FragmentShaderLocation, const char* a_VertexShaderLocation) : m_Camera_(new Camera()),
	m_PlanetTimer_(new Timer()), m_EmpireTimer_(new Timer()), m_SoiTimer_(new Timer())
{
	assert(a_Graphicsinitialized);
	
	m_PlanetTimer_->StartCountDown(std::chrono::seconds(5));
	m_EmpireTimer_->StartCountDown(std::chrono::minutes(1));
	m_SoiTimer_->StartCountDown(std::chrono::seconds(2));
}

Manager::~Manager()
{
	//delete m_Storage_;
	delete m_Camera_;
	delete m_PlanetTimer_;
}

void Manager::Update() const
{
	// Check timer
	const bool UpdatePlanetResource = m_PlanetTimer_->CheckCountDown();
	const bool UpdateEmpireResource = m_EmpireTimer_->CheckCountDown();
	const bool UpdateSOI = m_SoiTimer_->CheckCountDown();
	
	// instruct camera to update
	m_Camera_->Update();
	
	// instruct Planets to Update
	DataStorage* StorageInstance;
	StorageInstance = StorageInstance->GetInstance();

	for (int i = 0; i < StorageInstance->GetPlanetStorageSize(); i++)
	{
		StorageInstance->GetPlanet(i)->Update(UpdatePlanetResource, UpdateSOI);
	}
	
	for (int j = 0; j < StorageInstance->max_num_empires; j++)
	{
		StorageInstance->GetEmpire(j)->UpdateEmpire(UpdateEmpireResource);
	}

	for (int i = 0; i < StorageInstance->GetEmpireAIStorageSize(); i++)
	{
		StorageInstance->GetEmpireAI(i)->UpdateAI();
	}
}

void Manager::Draw(Camera* a_Camera) const
{
	DataStorage* StorageInstance;
	StorageInstance = StorageInstance->GetInstance();

	for (int k = 0; k < StorageInstance->GetPlanetStorageSize(); k++)
	{
		StorageInstance->GetPlanet(k)->Draw(a_Camera);
	}

	for (int i = 0; i < StorageInstance->max_num_empires; i++)
	{
		StorageInstance->GetEmpire(i)->DrawShips(a_Camera);
	}
}
