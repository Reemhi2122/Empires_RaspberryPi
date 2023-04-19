#include "Headers/objects/SOI.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/Shader.h"
#include "Headers/modules/GeometryModule.h"
#include "Headers/Camera.h"
#include <glm/gtc/type_ptr.inl>

SOI::SOI()
{}

SOI::SOI(Material* a_Material, GeometryModule* a_Module)
{
	m_CurrentMaterial_ = a_Material;
	m_CurrentModule_ = a_Module;
}

SOI::~SOI()
{}

bool SOI::Update()
{
	SetModelMatrix();
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

	return false;
}

bool SOI::Draw(Camera* a_ActiveCamera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	m_CurrentMaterial_->GetCurrentShader().Use();
	glBindVertexArray(m_CurrentModule_->GetVao());

	GLuint mvpLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "MVP");
	GLuint ambientLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "Ambient");

	glm::mat4 mvp = a_ActiveCamera->GetProjection() * a_ActiveCamera->GetView() * ModelMatrix;
	glm::vec4 amb = glm::vec4(m_Color, 0.7);

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));
	glUniform4fv(ambientLocation, 1, &amb[0]);

	glDrawArrays(GL_TRIANGLES, 0, 20 * 3);

	glDisable(GL_BLEND);
	glBindVertexArray(0);
	return true;
}

void SOI::SetColor(glm::vec3 a_Color)
{
	m_Color = a_Color;
}