#include "Headers/objects/Planet.h"
#include "Headers/OGLGraphics.h"
#include "Headers/Camera.h"
#include "Headers/modules/GeometryModule.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/Shader.h"
#include "Headers/objects/SOI.h"
#include "Headers/modules/DiscModule.h"
#include "Headers/PhysicsObj.h"
#include "Headers/Empire.h"

#include <glm/gtc/type_ptr.inl>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

Planet::Planet(Material* a_Material, GeometryModule* a_Module)
{
	m_CurrentMaterial_ = a_Material;
	m_CurrentModule_ = a_Module;
	m_FigherShips = 6;
}

Planet::Planet()
{
	m_CurrentMaterial_ = nullptr;
	m_CurrentModule_ = nullptr;
}

Planet::~Planet()
{
}

bool Planet::Update()
{
	return true;
}

bool Planet::Update(bool a_UpdateResources, bool a_UpdateSOI)
{
	if (a_UpdateResources)
	{
		UpdateResources();
	}
	if (a_UpdateSOI)
	{
		UpdateInfluence();
	}

	m_PhysicsObj->GetRigidBody()->setUserPointer(this);

	return true;
}

bool Planet::Draw(Camera* a_ActiveCamera)
{
	glEnable(GL_DEPTH_TEST);
	m_CurrentMaterial_->GetCurrentShader().Use();
	glBindVertexArray(m_CurrentModule_->GetVao());

	GLuint modelLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "model");
	GLuint viewLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "view");
	GLuint projectionLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "projection");
	GLuint ColorLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "faction_color");
	GLuint TextureSeedLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "u_TexSeed");
	GLuint TexHelperSeedLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "u_TexHelpSeed");
	GLuint NoiseScaleLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "u_NoiseScale");
	GLuint WaterColorLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "v_Water_Color");
	GLuint LandColorLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "v_LandColor");

	// uniform seeds & colors for random texture generation
	glUniform1ui(TextureSeedLocation, m_TextureSeed_);
	glUniform1ui(TexHelperSeedLocation, m_TextureHelperSeed_);
	glUniform1ui(NoiseScaleLocation, m_NoiseScale_);
	glUniform3f(WaterColorLocation, m_TexWaterColor_.x, m_TexWaterColor_.y, m_TexWaterColor_.z);
	glUniform3f(LandColorLocation, m_TexLandColor_.x, m_TexLandColor_.y, m_TexLandColor_.z);
	GLuint LightDirLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "lightDir");

	//glm::mat4 mvp = a_ActiveCamera->GetProjection() * a_ActiveCamera->GetView() * ModelMatrix;
	glm::vec4 amb = glm::vec4(1, 1, 1, 1);
	glm::vec3 lightDir = glm::vec3(0, 0, 0) - WorldPosition;

	// coloring planet in faction color if there is any
	if (GetOwner() != nullptr)
	{
		const glm::vec3& faction_color = GetOwner()->GetColor();
		glUniform3f(ColorLocation, faction_color.x, faction_color.y, faction_color.z);
	}
	else glUniform3f(ColorLocation, 1.0f, 1.0f, 1.0f);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(ModelMatrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(a_ActiveCamera->GetView()));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(a_ActiveCamera->GetProjection()));
	glUniform3fv(LightDirLocation, 1, &lightDir[0]);
	
	glBindTexture(GL_TEXTURE_2D, m_CurrentModule_->GetTextureID());

	glDrawArrays(GL_TRIANGLES, 0, m_CurrentModule_->GetVertexArraySize() / 8);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	if (IsOwned())
		m_SOI->Draw(a_ActiveCamera);
	return true;
}

string Planet::GetName()
{
	return m_Name_;
}

void Planet::SetName(const string* a_Name)
{
	m_Name_ = *a_Name;
}

Empire* Planet::GetOwner()
{
	return m_Owner_;
}

bool Planet::SetOwner(Empire* a_Empire)
{
	if (a_Empire == nullptr)
	{
		printf("Planet::SetOwner() failed! a_Empire must not be a nullptr.\n");
		return false;
	}

	if(m_Owner_ != nullptr) m_Owner_->RemovePlanet(this);
	m_SOI->SetColor(a_Empire->GetColor());
	m_Owner_ = a_Empire;
	m_IsColonized_ = true;
	m_AmountColonized_ = 100;
	return true;
	
}

bool Planet::SetTechLevel(int a_Level)
{
	assert(0 < a_Level && a_Level <= 15);
	if (a_Level < 0 || a_Level >= 15)
	{
		printf("Planet::SetTechLevel() failed! TechLevel must be between 1 and 15. Parameter was: %i \n", a_Level);
		return false;
	}

	m_TechLevel_ = a_Level;
	return true;
	
}

unsigned Planet::GetTechLevel()
{
	return m_TechLevel_;
}

bool Planet::SetResourceLevel(int a_Level)
{
	assert(0 < a_Level && a_Level <= 15);
	if (a_Level < 0 || a_Level >= 15)
	{
		printf("Planet::SetResourceLevel() failed! ResourceLevel must be between 1 and 15. Parameter was: %i \n", a_Level);
		return false;
	}

	m_ResourceLevel_ = a_Level;
	return true;
}

unsigned Planet::GetResourceLevel()
{
	return m_ResourceLevel_;
}

unsigned Planet::GetResources()
{
	return m_Resources_;
}

void Planet::ResetResources()
{
	m_Resources_ = 0;
}

void Planet::ReduceResources(const int a_Amount)
{
	m_Resources_ -= a_Amount;
}

void Planet::IncreaseResources(const int a_Amount)
{
	m_Resources_ += a_Amount;
}

void Planet::UpdateInfluence()
{
	if (IsOwned())
	{
		for (int i = 0; i < m_ClosePlanets_.size(); i++)
		{
			if (!m_ClosePlanets_[i]->IsOwned() && glm::distance(WorldPosition, m_ClosePlanets_[i]->GetPosition()) < 5)
			{
				m_ClosePlanets_[i]->IncreaseColonizationCounter(m_TechLevel_, *GetOwner());
			}
		}
	}
}

void Planet::IncreaseColonizationCounter(unsigned a_Amount, Empire& a_Colonizer)
{
	if (m_AmountColonized_ + a_Amount >= 100 && !m_IsColonized_)
	{
		m_AmountColonized_ = 100;
		SetOwner(&a_Colonizer);
		a_Colonizer.GetPlanets()->push_back(this);
		return;
	}

	m_AmountColonized_ += a_Amount;
}

unsigned int Planet::GetColonizationCounter()
{
	return m_AmountColonized_;
}

bool Planet::IsOwned()
{
	if (GetOwner() != nullptr && m_AmountColonized_ >= 100 && m_IsColonized_)
	{
		return true;
	}

	return false;
}

void Planet::RemoveVessels()
{
	if (m_CurrentVessels.size() > 0) {
		for (int i = 0; i < m_CurrentVessels.size(); i++)
		{
			if (m_Owner_->GetPlanets()->size() > 0)
				m_CurrentVessels[i]->SetTargetPlanet(m_Owner_->GetPlanets()->at(0));
		}
	}
}

unsigned int Planet::GetFighterAmount()
{
	return m_FigherShips;
}

void Planet::SetFighterAmount(unsigned int a_FighterShips)
{
	m_FigherShips = a_FighterShips;
}

void Planet::AddFighterAmount(unsigned int a_Amount)
{
	m_FigherShips += a_Amount;
}

void Planet::AddVessel(Vessel* a_Ship)
{
	m_CurrentVessels.push_back(a_Ship);
	for (int i = 0; i < m_CurrentVessels.size(); i++)
	{
		float radians = 2 * 3.14159265359 / m_CurrentVessels.size() * i;
		float vertical = glm::sin(radians);
		float horizontal = glm::cos(radians);
		glm::vec3 RotatedPos = glm::vec3(vertical, horizontal, 0);
		m_CurrentVessels.at(i)->SetPosition(RotatedPos + WorldPosition);
	}
}

void Planet::RemoveVessel(Vessel* a_Ship)
{
	for (int i = 0; i < m_CurrentVessels.size(); i++)
		if(m_CurrentVessels[i] == a_Ship) 
			m_CurrentVessels.erase(m_CurrentVessels.begin() + i);
}

void Planet::SetSeed(unsigned a_Seed)
{
	if (m_TextureSeed_ == 0)
	{
		m_TextureSeed_ = a_Seed;
	}
}

void Planet::SetHelperSeed(unsigned a_Seed)
{
	if (m_TextureHelperSeed_ == 0)
	{
		m_TextureHelperSeed_ = a_Seed;
	}
}

void Planet::SetNoiseScale(unsigned a_Scale)
{
	if(m_NoiseScale_ == 0)
	{
		m_NoiseScale_ = a_Scale;
	}
}

void Planet::SetWaterColor(const glm::vec3 a_Color)
{
	m_TexWaterColor_ = a_Color;
}

void Planet::SetLandColor(const glm::vec3 a_Color)
{
	m_TexLandColor_ = a_Color;
}

void Planet::UpdateResources()
{
	m_Resources_ += 20 * m_ResourceLevel_ + m_ResourceLevel_ * m_TechLevel_;
}

void Planet::UpdatePosition()
{
	m_PhysicsObj = new PhysicsObj("Planet", 0, false, btVector3(WorldPosition.x, WorldPosition.y, WorldPosition.z), btQuaternion(Rotations.x, Rotations.y, Rotations.z), new btSphereShape(1));
	m_PhysicsObj->GetRigidBody()->setUserPointer(this);
	SetModelMatrix();
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

#ifdef _RASPARM
	Shader* default_shader = new Shader("../../../HelloCubes/Resources/Shaders/debugshader.frag", "../../../HelloCubes/Resources/Shaders/debugshader.vert");
#else
	Shader* default_shader = new Shader("../Empires(Windows)/Resources/Shaders/debugshader.frag", "../Empires(Windows)/Resources/Shaders/debugshader.vert");
#endif // _RASPARM

	Material* default_material = new Material(*default_shader);
	GeometryModule* default_module = new DiscModule();
	m_SOI = new SOI(default_material, default_module);
	m_SOI->SetPosition(WorldPosition);
	m_SOI->Scales = glm::vec3(5, 5, 5);
	m_SOI->Update();
}

PhysicsObj* Planet::GetPhysicsObj()
{
	return m_PhysicsObj;
}