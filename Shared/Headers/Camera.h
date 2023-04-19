#pragma once

#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	/// <summary>
	/// Setup the camera with a view and projection
	/// </summary>
	Camera();
	~Camera();

	/// <summary>
	///	Updating the camera after setting the position
	/// </summary>
	void Update();

	glm::mat4 GetView();
	void SetView(glm::mat4 a_View);

	glm::mat4 GetProjection();
	void SetProjection(glm::mat4 a_Projection);

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 a_Position);

	void TransformPosition(glm::vec3 a_Vec3);
	void ResetTranform();

private:
	glm::vec3 m_CamPos = glm::vec3(0, 0, -500);
	glm::vec3 m_CamFront = glm::vec3(0, 0, 1);
	glm::vec3 m_CamUp = glm::vec3(0, 1, 0);

	glm::vec3 m_MainPos = glm::vec3(0, 0, -500);

	glm::mat4 m_View;
	glm::mat4 m_Projection;
};