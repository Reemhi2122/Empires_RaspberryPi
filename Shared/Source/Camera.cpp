#include "Headers/Camera.h"

Camera::Camera()
{
	m_View = glm::lookAt(m_CamPos, m_CamFront, m_CamUp);
	m_Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
}

Camera::~Camera()
{}

void Camera::Update() {
	m_CamFront = m_CamPos + glm::vec3(0, 0, 1);
	m_View = glm::lookAt(m_CamPos, m_CamFront, m_CamUp);
}

glm::mat4 Camera::GetView()
{
	return m_View;
}

void Camera::SetView(glm::mat4 a_View)
{
	m_View = a_View;
}

glm::mat4 Camera::GetProjection()
{
	return m_Projection;
}

void Camera::SetProjection(glm::mat4 a_Projection)
{
	m_Projection = a_Projection;
}

glm::vec3 Camera::GetPosition()
{
	return m_CamPos;
}

void Camera::SetPosition(glm::vec3 a_Position)
{
	m_CamPos = a_Position;
}

void Camera::TransformPosition(glm::vec3 a_Vec3)
{
	m_CamPos += a_Vec3;
}

void Camera::ResetTranform()
{
	m_CamPos = m_MainPos;
}
