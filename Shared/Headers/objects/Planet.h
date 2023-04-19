#pragma once
#include "../ObjectModel.h"

class Empire;
class String;
class PhysicsObj;
class Vessel;
class SOI;

class Planet : public ObjectModel
{
public:
	Planet(Material* a_Material, GeometryModule* a_Module);
	Planet();
	~Planet();
	
	bool Update() override;
	bool Update(bool a_UpdateResources, bool a_UpdateSOI);
	bool Draw(Camera* a_ActiveCamera) override;

	string GetName();
	void SetName(const string* a_Name);

	Empire* GetOwner();
	bool SetOwner(Empire* a_Empire);

	bool SetTechLevel(int a_Level);
	unsigned int GetTechLevel();

	bool SetResourceLevel(int a_Level);
	unsigned int GetResourceLevel();
	
	unsigned int GetResources();
	void ResetResources();
	void ReduceResources(int a_Amount);
	void IncreaseResources(int a_Amount);

	void UpdateInfluence();
	void UpdatePosition();

	void IncreaseColonizationCounter(unsigned a_Amount, Empire& a_Colonizer);
	unsigned int GetColonizationCounter();

	bool IsOwned();
	void RemoveVessels();
	
	unsigned int GetFighterAmount();
	void SetFighterAmount(unsigned int a_FighterShips);
	void AddFighterAmount(unsigned int a_Amount);

	void AddVessel(Vessel* a_Ship);
	void RemoveVessel(Vessel* a_Ship);

	void SetSeed(unsigned int a_Seed);
	void SetHelperSeed(unsigned int a_Seed);
	void SetNoiseScale(unsigned int a_Scale);
	void SetWaterColor(glm::vec3 a_Color);
	void SetLandColor(glm::vec3 a_Color);

	PhysicsObj* GetPhysicsObj();

	std::vector<Planet*> m_ClosePlanets_;
	
private:
	void UpdateResources();
	
	string m_Name_;
	Empire* m_Owner_ = nullptr;
	SOI* m_SOI;

	unsigned int m_TechLevel_;
	unsigned int m_ResourceLevel_;

	unsigned int m_Resources_;

	unsigned int m_TextureSeed_ = 0;
	unsigned int m_TextureHelperSeed_ = 0;
	unsigned int m_NoiseScale_ = 0;
	glm::vec3 m_TexWaterColor_;
	glm::vec3 m_TexLandColor_;

	unsigned int m_AmountColonized_ = 0;
	bool m_IsColonized_ = false;
	
	PhysicsObj* m_PhysicsObj;
	std::vector<Vessel*> m_CurrentVessels;

	unsigned int m_FigherShips;
};
