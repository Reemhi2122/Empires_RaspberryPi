#include "modules/DiscModule.h"
#include <cstdio>
#include <glm/glm.hpp>

DiscModule::DiscModule()
{
	m_VertexArrSize_ = 40;
	m_Ebo_ = 40;
	m_Vertices = new std::vector<GLfloat>();
	Init();
}

DiscModule::~DiscModule()
{
}

bool DiscModule::Init()
{
	float size = 20;

	float pi = 3.14159265359;
	for (int i = 0; i < size; i++)
	{
		float radians = 2 * pi / size * i;
		float vertical = glm::sin(radians);
		float horizontal = glm::cos(radians);
		glm::vec3 RotatedPos = glm::vec3(vertical, horizontal, 0);

		m_Vertices->push_back(RotatedPos.x);
		m_Vertices->push_back(RotatedPos.y);
		m_Vertices->push_back(RotatedPos.z);

		m_Vertices->push_back(0);
		m_Vertices->push_back(0);
		m_Vertices->push_back(0);

		radians = i == size - 1 ? 2 * pi / size * 0 : 2 * pi / size * (i + 1);
		vertical = glm::sin(radians);
		horizontal = glm::cos(radians);
		RotatedPos = glm::vec3(vertical, horizontal, 0);

		m_Vertices->push_back(RotatedPos.x);
		m_Vertices->push_back(RotatedPos.y);
		m_Vertices->push_back(RotatedPos.z);
	}

	glGenVertexArrays(1, &m_Vao_);
	glGenBuffers(1, &m_Vbo_);

	glBindVertexArray(m_Vao_);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices->size() * sizeof(float), &m_Vertices->at(0), GL_STATIC_DRAW);

	return true;
}