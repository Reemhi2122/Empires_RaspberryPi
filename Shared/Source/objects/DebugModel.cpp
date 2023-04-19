#include "Headers/objects/DebugModel.h"
#include "Headers/materials/Material.h"
#include "Headers/utilities/Shader.h"
#include "Headers/modules/GeometryModule.h"
#include "Headers/Camera.h"
#include <glm/gtc/type_ptr.inl>

DebugModel::DebugModel()
{}

DebugModel::DebugModel(Material* a_Material, GeometryModule* a_Module, int a_Size)
{
	m_CurrentMaterial_ = a_Material;
	m_CurrentModule_ = a_Module;
	m_Size = a_Size;
}

DebugModel::~DebugModel()
{}

bool DebugModel::Update()
{
	Scales = glm::vec3(1, 1, 1);
	SetModelMatrix();
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

	return false;
}

bool DebugModel::Draw(Camera* a_ActiveCamera)
{
	glEnable(GL_DEPTH_TEST);
	m_CurrentMaterial_->GetCurrentShader().Use();
	glBindVertexArray(m_CurrentModule_->GetVao());

	GLuint mvpLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "MVP");
	GLuint ambientLocation = glGetUniformLocation(m_CurrentMaterial_->GetCurrentShader().GetProgramID(), "Ambient");

	glm::mat4 mvp = a_ActiveCamera->GetProjection() * a_ActiveCamera->GetView() * ModelMatrix;
	glm::vec4 amb = glm::vec4(0, 1, 0, 1);

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, value_ptr(mvp));
	glUniform4fv(ambientLocation, 1, &amb[0]);

	glDrawArrays(GL_LINES, 0, m_Size / 3);

	glBindVertexArray(0);
	return true;
}
