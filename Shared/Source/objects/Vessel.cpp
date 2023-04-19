#include "Headers/objects/Vessel.h"
#include "Headers/modules/GeometryModule.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/Shader.h"
#include "Headers/objects/Planet.h"
#include "Headers/Camera.h"
#include <glm/gtc/type_ptr.inl>

#include "Headers/PhysicsObj.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "Headers/Empire.h"

#include <btBulletDynamicsCommon.h>

Vessel::Vessel()
{
	m_Health = 100;
	m_Armor = 100;
	m_Fuel = 100;

	m_TargetPlanet = nullptr;
}

Vessel::Vessel(Material* a_Material, GeometryModule* a_Module, Empire* a_Empire)
{
	m_CurrentMaterial_ = a_Material;
	m_CurrentModule_ = a_Module;
	m_Empire = a_Empire;

	m_TargetPlanet = nullptr;
}

Vessel::~Vessel()
{
}

bool Vessel::Update()
{
	SetModelMatrix();
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

	UpdateVessel();

	return false;
}

bool Vessel::Draw(Camera* a_ActiveCamera)
{
	glEnable(GL_DEPTH_TEST);
	m_CurrentMaterial_->GetCurrentShader().Use();
	glBindVertexArray(m_CurrentModule_->GetVao());

	GLuint mvpLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "MVP");
	GLuint ColorLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "faction_color");
	GLuint LightDirLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "lightDir");
	
	glm::mat4 mvp = a_ActiveCamera->GetProjection() * a_ActiveCamera->GetView() * ModelMatrix;
	glm::vec3 lightDir = glm::normalize(glm::vec3(0, 0, -10) - WorldPosition);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	glUniform3fv(ColorLocation,1, &color[0]);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));
	glUniform3fv(LightDirLocation, 1, &lightDir[0]);

	glBindTexture(GL_TEXTURE_2D, m_CurrentModule_->GetTextureID());

	glDrawArrays(GL_TRIANGLES, 0, m_CurrentModule_->GetVertexArraySize() / 8);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	return true;
}

void Vessel::UpdateVessel()
{
	if (m_TargetPlanet != nullptr) {
		WorldPosition = LinearInterplation(WorldPosition, m_TargetPlanet->GetPosition(), 0.005f);
		if (glm::distance(m_TargetPlanet->GetPosition(), WorldPosition) < 1) 
			PlanetActionCall();
	}
}

Planet* Vessel::GetTargetPlanet()
{
	return m_TargetPlanet;
}

void Vessel::SetTargetPlanet(Planet* a_TargetPlanet)
{
	if (m_StationaryPlanet != nullptr) {
		m_StationaryPlanet->RemoveVessel(this);
		m_StationaryPlanet = nullptr;
	}
	m_TargetPlanet = a_TargetPlanet;
}

Planet* Vessel::GetStationaryPlanet()
{
	return m_StationaryPlanet;
}

void Vessel::SetStationaryPlanet(Planet* a_StationaryPlanet)
{
	m_StationaryPlanet = a_StationaryPlanet;
}

PhysicsObj* Vessel::GetPhysicsObj()
{
	return m_PhysicsObj;
}